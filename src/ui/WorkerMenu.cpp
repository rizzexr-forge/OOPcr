#include "ui/WorkerMenu.h"
#include "core/Console.h"
#include "core/Date.h"
#include <iostream>

void WorkerMenu::run() {
    while (true) {
        console::title("Работник: " + user_.login());
        std::cout << "1. Доступные приборы\n"
                  << "2. Выдать прибор\n"
                  << "3. Принять возврат\n"
                  << "4. Приборы в аренде\n"
                  << "5. Просроченные\n"
                  << "0. Выход\n\n";
        int choice = console::readInt("Ваш выбор: ");

        if (choice == 0) return;
        switch (choice) {
            case 1: showAvailable(); break;
            case 2: issueEquipment(); break;
            case 3: returnEquipment(); break;
            case 4: showActive(); break;
            case 5: showOverdue(); break;
            default:
                std::cout << console::RED << "Нет такого пункта!" << console::RESET << "\n";
        }
        console::pause();
    }
}

void WorkerMenu::printRental(const Rental& r) {
    std::cout << "#" << r.id << " " << r.equipmentName << " (инв. " << r.inventoryNumber << ")"
              << " | заказчик: " << r.clientLogin
              << " | выдал: " << r.workerLogin
              << " | " << r.dateFrom << " - " << r.dateTo << " | ";
    if (r.returned) {
        std::cout << console::GREEN << "возвращён" << console::RESET;
    } else if (r.dateTo < date::today()) {
        std::cout << console::RED << "ПРОСРОЧЕНО" << console::RESET;
    } else {
        std::cout << console::YELLOW << "в аренде" << console::RESET;
    }
    std::cout << "\n";
}

void WorkerMenu::issueEquipment() {
    console::title("Выдача прибора");
    auto list = equipment_.getAvailable();
    if (list.empty()) {
        std::cout << console::YELLOW << "Свободных приборов нет." << console::RESET << "\n";
        return;
    }
    printList(list);

    int id = console::readInt("\nID прибора (0 - отмена): ");
    if (id == 0) return;

    auto item = equipment_.getById(id);
    if (!item || item->status() != Status::InStock) {
        std::cout << console::RED << "Такого свободного прибора нет!" << console::RESET << "\n";
        return;
    }

    std::string login = console::readNonEmpty("Логин заказчика: ");
    auto client = users_.findByLogin(login);
    if (!client || client->getRole() != "client") {
        std::cout << console::RED << "Заказчик с таким логином не найден!" << console::RESET << "\n";
        return;
    }

    std::string today = date::today();
    std::string to = console::readLine("Вернуть до (ГГГГ-ММ-ДД): ");
    if (!date::isValid(to)) {
        std::cout << console::RED << "Неверная дата! Нужен формат ГГГГ-ММ-ДД." << console::RESET << "\n";
        return;
    }
    if (to < today) {
        std::cout << console::RED << "Дата возврата не может быть в прошлом!" << console::RESET << "\n";
        return;
    }

    rentals_.issue(id, client->id(), user_.id(), today, to);
    std::cout << console::GREEN << "Выдано: " << item->name() << " -> " << login
              << " до " << to << console::RESET << "\n";
}

void WorkerMenu::returnEquipment() {
    console::title("Приём возврата");
    auto active = rentals_.getActive();
    if (active.empty()) {
        std::cout << console::YELLOW << "Сейчас ничего не в аренде." << console::RESET << "\n";
        return;
    }
    for (const auto& r : active) {
        std::cout << "[прибор " << r.equipmentId << "] ";
        printRental(r);
    }

    int id = console::readInt("\nID прибора, который вернули (0 - отмена): ");
    if (id == 0) return;

    if (rentals_.closeRental(id)) {
        std::cout << console::GREEN << "Возврат принят, прибор снова на складе."
                  << console::RESET << "\n";
    } else {
        std::cout << console::RED << "У этого прибора нет активной аренды!" << console::RESET << "\n";
    }
}

void WorkerMenu::showActive() {
    console::title("Приборы в аренде");
    auto active = rentals_.getActive();
    if (active.empty()) {
        std::cout << console::GREEN << "Ничего не в аренде." << console::RESET << "\n";
        return;
    }
    for (const auto& r : active) printRental(r);
}

void WorkerMenu::showOverdue() {
    console::title("Просроченные аренды");
    auto overdue = rentals_.getOverdue(date::today());
    if (overdue.empty()) {
        std::cout << console::GREEN << "Просроченных нет." << console::RESET << "\n";
        return;
    }
    for (const auto& r : overdue) printRental(r);
}
