#pragma once
#include "ui/WorkerMenu.h"

// Меню администратора: всё, что умеет работник, плюс
// добавление и удаление приборов, история и "где что находится".
class AdminMenu : public WorkerMenu {
public:
    using WorkerMenu::WorkerMenu;

    void run() override;

private:
    void showLocations();    // где какой прибор находится
    void addEquipment();
    void removeEquipment();
    void showHistory();
};
