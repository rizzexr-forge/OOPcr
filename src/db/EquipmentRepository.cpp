#include "db/EquipmentRepository.h"
#include "models/EquipmentFactory.h"

static const char* SELECT_EQUIPMENT =
    "SELECT id, type, name, inventory_number, status, location, specs FROM equipment ";

// Строка БД -> объект нужного класса (через фабрику)
std::unique_ptr<Equipment> EquipmentRepository::fromRow(const Statement& st) {
    auto item = EquipmentFactory::create(st.getText(1));
    if (!item) return nullptr;   // неизвестный тип - пропускаем
    item->setId(st.getInt(0));
    item->setName(st.getText(2));
    item->setInventoryNumber(st.getText(3));
    item->setStatus(statusFromDb(st.getText(4)));
    item->setLocation(st.getText(5));
    item->setSpecs(st.getText(6));
    return item;
}

EquipmentList EquipmentRepository::query(const std::string& tail) {
    EquipmentList result;
    Statement st = db_.prepare(std::string(SELECT_EQUIPMENT) + tail);
    while (st.step()) {
        auto item = fromRow(st);
        if (item) result.push_back(std::move(item));
    }
    return result;
}

EquipmentList EquipmentRepository::getAll() {
    return query("ORDER BY id");
}

EquipmentList EquipmentRepository::getAvailable() {
    return query("WHERE status = 'in_stock' ORDER BY id");
}

std::unique_ptr<Equipment> EquipmentRepository::getById(int id) {
    Statement st = db_.prepare(std::string(SELECT_EQUIPMENT) + "WHERE id = ?");
    st.bind(1, id);
    if (st.step()) return fromRow(st);
    return nullptr;
}

int EquipmentRepository::add(const Equipment& e) {
    Statement st = db_.prepare(
        "INSERT INTO equipment (type, name, inventory_number, status, location, specs) "
        "VALUES (?, ?, ?, ?, ?, ?)");
    st.bind(1, e.getType())
      .bind(2, e.name())
      .bind(3, e.inventoryNumber())
      .bind(4, statusToDb(e.status()))
      .bind(5, e.location())
      .bind(6, e.getSpecs());
    st.run();
    return static_cast<int>(db_.lastInsertId());
}

bool EquipmentRepository::inventoryNumberExists(const std::string& number) {
    Statement st = db_.prepare("SELECT 1 FROM equipment WHERE inventory_number = ?");
    st.bind(1, number);
    return st.step();
}

bool EquipmentRepository::remove(int id) {
    auto item = getById(id);
    if (!item || item->status() == Status::Rented) return false;
    Statement st = db_.prepare("DELETE FROM equipment WHERE id = ?");
    st.bind(1, id);
    st.run();
    return db_.changes() > 0;
}

bool EquipmentRepository::setStatus(int id, Status status) {
    Statement st = db_.prepare("UPDATE equipment SET status = ? WHERE id = ?");
    st.bind(1, statusToDb(status)).bind(2, id);
    st.run();
    return db_.changes() > 0;
}

int EquipmentRepository::count() {
    Statement st = db_.prepare("SELECT COUNT(*) FROM equipment");
    st.step();
    return st.getInt(0);
}
