#pragma once

// Абстрактное меню: каждая роль реализует свой run()
class Menu {
public:
    virtual ~Menu() = default;
    virtual void run() = 0;   // крутится, пока пользователь не выберет "Выход"
};
