#include <iostream>
#include <memory>
#include "core/Console.h"
#include "db/Database.h"
#include "db/EquipmentRepository.h"
#include "db/RentalRepository.h"
#include "db/UserRepository.h"
#include "models/EquipmentFactory.h"
#include "ui/AdminMenu.h"
#include "ui/ClientMenu.h"
#include "ui/LoginMenu.h"
#include "ui/WorkerMenu.h"

// Добавляет тестовые данные при самом первом запуске (пока таблицы пустые)
static void seedIfEmpty(UserRepository& users, EquipmentRepository& equipment) {
    if (users.count() == 0) {
        users.add("admin", "admin", "admin");
        users.add("worker", "worker", "worker");
        users.add("client", "client", "client");
    }
    if (equipment.count() == 0) {
        struct Sample { const char* type; const char* name; const char* inv;
                        const char* place; const char* specs; };
        const Sample samples[] = {
            {"spotlight",  "Прожектор PAR-64",    "SP-001", "Стеллаж A1", "power=300;angle=25"},
            {"spotlight",  "Прожектор Fresnel",   "SP-002", "Стеллаж A1", "power=650;angle=40"},
            {"movinghead", "Robe Spot 250",       "MH-001", "Стеллаж B2", "pan=540;tilt=270;gobo=8"},
            {"ledpanel",   "LED-панель 60x60",    "LP-001", "Стеллаж C1", "size=60;leds=3600;rgb=1"},
            {"fogmachine", "Дым-машина Antari",   "FM-001", "Стеллаж D4", "output=300;tank=2"},
        };
        for (const auto& s : samples) {
            auto item = EquipmentFactory::create(s.type);
            item->setName(s.name);
            item->setInventoryNumber(s.inv);
            item->setLocation(s.place);
            item->setSpecs(s.specs);
            equipment.add(*item);
        }
    }
}

// Выбирает меню по роли: полиморфизм - дальше main работает только с Menu
static std::unique_ptr<Menu> createMenu(const User& user, EquipmentRepository& equipment,
                                        RentalRepository& rentals, UserRepository& users) {
    if (user.getRole() == "admin")
        return std::make_unique<AdminMenu>(equipment, rentals, users, user);
    if (user.getRole() == "worker")
        return std::make_unique<WorkerMenu>(equipment, rentals, users, user);
    return std::make_unique<ClientMenu>(equipment, rentals, user);
}

int main() {
    console::init();
    try {
        Database db("rental.db");
        db.initSchema();

        UserRepository users(db);
        EquipmentRepository equipment(db);
        RentalRepository rentals(db);
        seedIfEmpty(users, equipment);

        LoginMenu loginMenu(users);
        while (true) {
            auto user = loginMenu.show();
            if (!user) break;   // выбран выход

            auto menu = createMenu(*user, equipment, rentals, users);
            menu->run();
        }
    } catch (const console::InputClosed&) {
        // ввод закрыт (Ctrl+Z / Ctrl+D) - просто выходим
    } catch (const std::exception& e) {
        std::cerr << console::RED << "Ошибка: " << e.what() << console::RESET << "\n";
        console::pause();
        return 1;
    }
    return 0;
}
