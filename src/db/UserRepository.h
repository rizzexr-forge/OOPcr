#pragma once
#include <memory>
#include <string>
#include "db/Database.h"
#include "models/User.h"

// Работа с таблицей users
class UserRepository {
public:
    explicit UserRepository(Database& db) : db_(db) {}

    // nullptr, если пользователя с таким логином нет
    std::unique_ptr<User> findByLogin(const std::string& login);

    // false, если логин уже занят
    bool add(const std::string& role, const std::string& login, const std::string& password);

    int count();

private:
    Database& db_;
};
