#include "ui/ClientMenu.h"
#include "core/Console.h"
#include <iostream>

void ClientMenu::run() {
    while (true) {
        console::title("Заказчик: " + user_.login());
        std::cout << "1. Доступные приборы\n"
                  << "2. Занятые приборы (когда освободятся)\n"
                  << "0. Выход\n\n";
        int choice = console::readInt("Ваш выбор: ");

        if (choice == 0) return;
        if (choice == 1) showAvailable();
        else if (choice == 2) showBusy();
        else std::cout << console::RED << "Нет такого пункта!" << console::RESET << "\n";
        console::pause();
    }
}

void ClientMenu::printList(const EquipmentList& list) {
    for (const auto& item : list) {
        item->print();   // полиморфизм: у каждого типа своё описание
    }
}

void ClientMenu::showAvailable() {
    console::title("Доступные приборы");
    auto list = equipment_.getAvailable();
    if (list.empty()) {
        std::cout << console::YELLOW << "Свободных приборов нет." << console::RESET << "\n";
        return;
    }
    printList(list);
}

void ClientMenu::showBusy() {
    console::title("Занятые приборы");
    auto active = rentals_.getActive();
    if (active.empty()) {
        std::cout << console::GREEN << "Все приборы свободны." << console::RESET << "\n";
        return;
    }
    for (const auto& r : active) {
        std::cout << r.equipmentName << " (инв. " << r.inventoryNumber << ") - "
                  << console::YELLOW << "освободится " << r.dateTo << console::RESET << "\n";
    }
}
