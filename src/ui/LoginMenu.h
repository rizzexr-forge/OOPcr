#pragma once
#include <memory>
#include "db/UserRepository.h"
#include "models/User.h"

// Экран входа и регистрации
class LoginMenu {
public:
    explicit LoginMenu(UserRepository& users) : users_(users) {}

    // Возвращает вошедшего пользователя или nullptr, если выбран выход из программы.
    std::unique_ptr<User> show();

private:
    std::unique_ptr<User> login();
    void registerClient();

    UserRepository& users_;
};
