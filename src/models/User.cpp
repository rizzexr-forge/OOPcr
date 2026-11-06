#include "models/User.h"

std::unique_ptr<User> createUser(const std::string& role, int id,
                                 const std::string& login, const std::string& password) {
    if (role == "client") return std::make_unique<Client>(id, login, password);
    if (role == "worker") return std::make_unique<Worker>(id, login, password);
    if (role == "admin")  return std::make_unique<Admin>(id, login, password);
    return nullptr;
}
