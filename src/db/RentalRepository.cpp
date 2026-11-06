#include "db/RentalRepository.h"
#include <stdexcept>

// LEFT JOIN: история остаётся, даже если прибор потом удалили
static const char* SELECT_RENTALS =
    "SELECT r.id, r.equipment_id, r.client_id, r.worker_id, r.date_from, r.date_to, r.returned, "
    "       COALESCE(e.name, '(удалён)'), COALESCE(e.inventory_number, '-'), "
    "       COALESCE(c.login, '?'), COALESCE(w.login, '?') "
    "FROM rentals r "
    "LEFT JOIN equipment e ON e.id = r.equipment_id "
    "LEFT JOIN users c ON c.id = r.client_id "
    "LEFT JOIN users w ON w.id = r.worker_id ";

std::vector<Rental> RentalRepository::query(const std::string& tail, const std::string& arg) {
    std::vector<Rental> result;
    Statement st = db_.prepare(std::string(SELECT_RENTALS) + tail);
    if (!arg.empty()) st.bind(1, arg);
    while (st.step()) {
        Rental r;
        r.id = st.getInt(0);
        r.equipmentId = st.getInt(1);
        r.clientId = st.getInt(2);
        r.workerId = st.getInt(3);
        r.dateFrom = st.getText(4);
        r.dateTo = st.getText(5);
        r.returned = st.getInt(6) != 0;
        r.equipmentName = st.getText(7);
        r.inventoryNumber = st.getText(8);
        r.clientLogin = st.getText(9);
        r.workerLogin = st.getText(10);
        result.push_back(std::move(r));
    }
    return result;
}

void RentalRepository::issue(int equipmentId, int clientId, int workerId,
                             const std::string& dateFrom, const std::string& dateTo) {
    db_.exec("BEGIN");
    try {
        {
            // меняем статус, только если прибор действительно на складе
            Statement upd = db_.prepare(
                "UPDATE equipment SET status = 'rented' WHERE id = ? AND status = 'in_stock'");
            upd.bind(1, equipmentId);
            upd.run();
            if (db_.changes() == 0) {
                throw std::runtime_error("Прибор недоступен для выдачи");
            }
        }
        {
            Statement ins = db_.prepare(
                "INSERT INTO rentals (equipment_id, client_id, worker_id, date_from, date_to, returned) "
                "VALUES (?, ?, ?, ?, ?, 0)");
            ins.bind(1, equipmentId).bind(2, clientId).bind(3, workerId)
               .bind(4, dateFrom).bind(5, dateTo);
            ins.run();
        }
        db_.exec("COMMIT");
    } catch (...) {
        db_.exec("ROLLBACK");
        throw;
    }
}

bool RentalRepository::closeRental(int equipmentId) {
    db_.exec("BEGIN");
    try {
        bool closed = false;
        {
            Statement upd = db_.prepare(
                "UPDATE rentals SET returned = 1 WHERE equipment_id = ? AND returned = 0");
            upd.bind(1, equipmentId);
            upd.run();
            closed = db_.changes() > 0;
        }
        if (closed) {
            Statement st = db_.prepare("UPDATE equipment SET status = 'in_stock' WHERE id = ?");
            st.bind(1, equipmentId);
            st.run();
        }
        db_.exec("COMMIT");
        return closed;
    } catch (...) {
        db_.exec("ROLLBACK");
        throw;
    }
}

std::vector<Rental> RentalRepository::getActive() {
    return query("WHERE r.returned = 0 ORDER BY r.date_to, r.id");
}

std::vector<Rental> RentalRepository::getOverdue(const std::string& today) {
    return query("WHERE r.returned = 0 AND r.date_to < ? ORDER BY r.date_to, r.id", today);
}

std::vector<Rental> RentalRepository::getAllHistory() {
    return query("ORDER BY r.id DESC");
}
