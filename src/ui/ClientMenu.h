#pragma once
#include "db/EquipmentRepository.h"
#include "db/RentalRepository.h"
#include "models/User.h"
#include "ui/Menu.h"

// Меню заказчика: смотреть доступные приборы.
// От него наследуются WorkerMenu и AdminMenu, поэтому общие вещи protected.
class ClientMenu : public Menu {
public:
    ClientMenu(EquipmentRepository& equipment, RentalRepository& rentals, const User& user)
        : equipment_(equipment), rentals_(rentals), user_(user) {}

    void run() override;

protected:
    void showAvailable();   // приборы на складе
    void showBusy();        // приборы в аренде и до какого числа

    static void printList(const EquipmentList& list);

    EquipmentRepository& equipment_;
    RentalRepository& rentals_;
    const User& user_;
};
