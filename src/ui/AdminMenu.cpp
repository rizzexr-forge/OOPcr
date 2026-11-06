#include "ui/AdminMenu.h"
#include "core/Console.h"
#include "models/EquipmentFactory.h"
#include <iostream>
#include <map>

void AdminMenu::run() {
    while (true) {
        console::title("Администратор: " + user_.login());
        std::cout << " 1. Где какой прибор находится\n"
                  << " 2. Добавить прибор\n"
                  << " 3. Удалить прибор\n"
                  << " 4. История всех аренд\n"
                  << " 5. Доступные приборы\n"
                  << " 6. Выдать прибор\n"
                  << " 7. Принять возврат\n"
                  << " 8. Приборы в аренде\n"
                  << " 9. Просроченные\n"
                  << " 0. Выход\n\n";
        int choice = console::readInt("Ваш выбор: ");

        if (choice == 0) return;
        switch (choice) {
            case 1: showLocations(); break;
            case 2: addEquipment(); break;
            case 3: removeEquipment(); break;
            case 4: showHistory(); break;
            case 5: showAvailable(); break;
            case 6: issueEquipment(); break;
            case 7: returnEquipment(); break;
            case 8: showActive(); break;
            case 9: showOverdue(); break;
            default:
                std::cout << console::RED << "Нет такого пункта!" << console::RESET << "\n";
        }
        console::pause();
    }
}

void AdminMenu::showLocations() {
    console::title("Где какой прибор находится");
    auto list = equipment_.getAll();
    if (list.empty()) {
        std::cout << console::YELLOW << "Приборов нет." << console::RESET << "\n";
        return;
    }

    // активные аренды по id прибора
    std::map<int, Rental> active;
    for (const auto& r : rentals_.getActive()) active[r.equipmentId] = r;

    for (const auto& item : list) {
        std::cout << console::CYAN << "[" << item->id() << "] " << console::RESET
                  << item->getTypeName() << " | " << item->name()
                  << " (инв. " << item->inventoryNumber() << ") - ";
        auto it = active.find(item->id());
        if (it != active.end()) {
            std::cout << console::YELLOW << "у заказчика " << it->second.clientLogin
                      << " до " << it->second.dateTo << console::RESET;
        } else {
            std::cout << console::GREEN << "на складе: " << item->location() << console::RESET;
        }
        std::cout << "\n";
    }
}

void AdminMenu::addEquipment() {
    console::title("Добавление прибора");

    const auto& types = EquipmentFactory::types();
    for (size_t i = 0; i < types.size(); ++i) {
        std::cout << (i + 1) << ". " << EquipmentFactory::create(types[i])->getTypeName() << "\n";
    }
    int t = console::readInt("\nТип прибора (0 - отмена): ");
    if (t == 0) return;
    if (t < 1 || t > static_cast<int>(types.size())) {
        std::cout << console::RED << "Нет такого типа!" << console::RESET << "\n";
        return;
    }

    auto item = EquipmentFactory::create(types[t - 1]);

    item->setName(console::readNonEmpty("Название: "));

    std::string inv = console::readNonEmpty("Инвентарный номер: ");
    if (equipment_.inventoryNumberExists(inv)) {
        std::cout << console::RED << "Такой инвентарный номер уже есть!" << console::RESET << "\n";
        return;
    }
    item->setInventoryNumber(inv);
    item->setLocation(console::readNonEmpty("Место на складе (например, Стеллаж A3): "));

    // характеристики спрашиваем у самого прибора - у каждого типа свои
    std::string specs;
    for (const auto& [key, label] : item->specFields()) {
        int value = console::readInt(label + ": ");
        if (!specs.empty()) specs += ";";
        specs += key + "=" + std::to_string(value);
    }
    item->setSpecs(specs);

    int id = equipment_.add(*item);
    std::cout << console::GREEN << "Прибор добавлен, ID = " << id << console::RESET << "\n";
}

void AdminMenu::removeEquipment() {
    console::title("Удаление прибора");
    auto list = equipment_.getAll();
    if (list.empty()) {
        std::cout << console::YELLOW << "Приборов нет." << console::RESET << "\n";
        return;
    }
    printList(list);

    int id = console::readInt("\nID прибора для удаления (0 - отмена): ");
    if (id == 0) return;

    auto item = equipment_.getById(id);
    if (!item) {
        std::cout << console::RED << "Такого прибора нет!" << console::RESET << "\n";
        return;
    }
    if (item->status() == Status::Rented) {
        std::cout << console::RED << "Прибор сейчас в аренде, сначала примите возврат!"
                  << console::RESET << "\n";
        return;
    }

    std::string answer = console::readLine("Удалить \"" + item->name() + "\"? (да/нет): ");
    if (answer != "да" && answer != "y" && answer != "Y") {
        std::cout << "Отменено.\n";
        return;
    }

    if (equipment_.remove(id)) {
        std::cout << console::GREEN << "Прибор удалён." << console::RESET << "\n";
    } else {
        std::cout << console::RED << "Не удалось удалить." << console::RESET << "\n";
    }
}

void AdminMenu::showHistory() {
    console::title("История всех аренд");
    auto history = rentals_.getAllHistory();
    if (history.empty()) {
        std::cout << console::YELLOW << "Аренд ещё не было." << console::RESET << "\n";
        return;
    }
    for (const auto& r : history) printRental(r);
}
