#pragma once
#include "db/UserRepository.h"
#include "ui/ClientMenu.h"

// Меню работника: умеет всё, что заказчик, плюс выдача и приём приборов.
// От него наследуется AdminMenu.
class WorkerMenu : public ClientMenu {
public:
    WorkerMenu(EquipmentRepository& equipment, RentalRepository& rentals,
               UserRepository& users, const User& user)
        : ClientMenu(equipment, rentals, user), users_(users) {}

    void run() override;

protected:
    void issueEquipment();    // выдать прибор заказчику
    void returnEquipment();   // принять прибор обратно
    void showActive();        // что сейчас в аренде и до какого числа
    void showOverdue();       // просроченные

    static void printRental(const Rental& r);

    UserRepository& users_;
};
