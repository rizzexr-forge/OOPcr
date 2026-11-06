#pragma once
#include <memory>
#include <string>
#include <vector>
#include "db/Database.h"
#include "models/Equipment.h"

using EquipmentList = std::vector<std::unique_ptr<Equipment>>;

// Работа с таблицей equipment
class EquipmentRepository {
public:
    explicit EquipmentRepository(Database& db) : db_(db) {}

    EquipmentList getAll();
    EquipmentList getAvailable();                     // только "на складе"
    std::unique_ptr<Equipment> getById(int id);       // nullptr, если нет такого

    int add(const Equipment& e);                      // вернёт id нового прибора
    bool inventoryNumberExists(const std::string& number);
    bool remove(int id);                              // false: нет такого или он в аренде
    bool setStatus(int id, Status status);
    int count();

private:
    EquipmentList query(const std::string& tail);
    static std::unique_ptr<Equipment> fromRow(const Statement& st);

    Database& db_;
};
