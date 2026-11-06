#pragma once
#include <string>

// Запись об аренде (простая структура данных)
struct Rental {
    int id = 0;
    int equipmentId = 0;
    int clientId = 0;
    int workerId = 0;
    std::string dateFrom;   // ГГГГ-ММ-ДД
    std::string dateTo;     // ГГГГ-ММ-ДД
    bool returned = false;

    // поля для вывода (подтягиваются JOIN-ом из других таблиц)
    std::string equipmentName;
    std::string inventoryNumber;
    std::string clientLogin;
    std::string workerLogin;
};
