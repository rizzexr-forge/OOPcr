#include "ui/LoginMenu.h"
#include "core/Console.h"
#include <iostream>

std::unique_ptr<User> LoginMenu::show() {
    while (true) {
        console::title("Прокат светового оборудования");
        std::cout << "1. Войти\n"
                  << "2. Регистрация (заказчик)\n"
                  << "0. Выход\n\n";
        int choice = console::readInt("Ваш выбор: ");

        if (choice == 0) return nullptr;
        if (choice == 1) {
            auto user = login();
            if (user) return user;
        } else if (choice == 2) {
            registerClient();
        } else {
            std::cout << console::RED << "Нет такого пункта!" << console::RESET << "\n";
            console::pause();
        }
    }
}

std::unique_ptr<User> LoginMenu::login() {
    console::title("Вход");
    std::string login = console::readNonEmpty("Логин: ");
    std::string password = console::readLine("Пароль: ");

    auto user = users_.findByLogin(login);
    if (!user || user->password() != password) {
        std::cout << console::RED << "Неверный логин или пароль!" << console::RESET << "\n";
        console::pause();
        return nullptr;
    }
    return user;
}

void LoginMenu::registerClient() {
    console::title("Регистрация заказчика");
    std::string login = console::readNonEmpty("Придумайте логин: ");
    std::string password = console::readNonEmpty("Придумайте пароль: ");

    if (users_.add("client", login, password)) {
        std::cout << console::GREEN << "Вы зарегистрированы! Теперь можно войти."
                  << console::RESET << "\n";
    } else {
        std::cout << console::RED << "Такой логин уже занят!" << console::RESET << "\n";
    }
    console::pause();
}
