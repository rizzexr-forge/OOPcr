#pragma once
#include <string>
#include <vector>
#include "db/Database.h"
#include "models/Rental.h"

// Работа с таблицей rentals
class RentalRepository {
public:
    explicit RentalRepository(Database& db) : db_(db) {}

    // выдать прибор: создаёт запись и переводит прибор в статус "в аренде".
    // Бросает runtime_error, если прибор уже не на складе.
    void issue(int equipmentId, int clientId, int workerId,
               const std::string& dateFrom, const std::string& dateTo);

    // принять возврат: закрывает аренду и возвращает прибор на склад.
    // false, если у этого прибора нет активной аренды.
    bool closeRental(int equipmentId);

    std::vector<Rental> getActive();                          // сейчас в аренде
    std::vector<Rental> getOverdue(const std::string& today); // срок вышел, не возвращены
    std::vector<Rental> getAllHistory();                      // вся история

private:
    std::vector<Rental> query(const std::string& tail, const std::string& arg = "");

    Database& db_;
};
