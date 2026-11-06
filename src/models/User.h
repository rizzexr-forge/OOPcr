#pragma once
#include <memory>
#include <string>

// Базовый пользователь
class User {
public:
    User(int id, std::string login, std::string password)
        : id_(id), login_(std::move(login)), password_(std::move(password)) {}
    virtual ~User() = default;

    int id() const { return id_; }
    const std::string& login() const { return login_; }
    const std::string& password() const { return password_; }

    virtual std::string getRole() const = 0;      // для БД: "client"
    virtual std::string getRoleName() const = 0;  // для экрана: "Заказчик"

private:
    int id_;
    std::string login_;
    std::string password_;
};

class Client : public User {
public:
    using User::User;
    std::string getRole() const override { return "client"; }
    std::string getRoleName() const override { return "Заказчик"; }
};

class Worker : public User {
public:
    using User::User;
    std::string getRole() const override { return "worker"; }
    std::string getRoleName() const override { return "Работник"; }
};

class Admin : public User {
public:
    using User::User;
    std::string getRole() const override { return "admin"; }
    std::string getRoleName() const override { return "Администратор"; }
};

// создаёт Client / Worker / Admin по строке роли (nullptr, если роль неизвестна)
std::unique_ptr<User> createUser(const std::string& role, int id,
                                 const std::string& login, const std::string& password);
