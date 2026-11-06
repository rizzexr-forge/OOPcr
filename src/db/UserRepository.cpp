#include "db/UserRepository.h"

std::unique_ptr<User> UserRepository::findByLogin(const std::string& login) {
    Statement st = db_.prepare("SELECT id, login, password, role FROM users WHERE login = ?");
    st.bind(1, login);
    if (!st.step()) return nullptr;
    return createUser(st.getText(3), st.getInt(0), st.getText(1), st.getText(2));
}

bool UserRepository::add(const std::string& role, const std::string& login,
                         const std::string& password) {
    if (findByLogin(login)) return false;
    Statement st = db_.prepare("INSERT INTO users (login, password, role) VALUES (?, ?, ?)");
    st.bind(1, login).bind(2, password).bind(3, role);
    st.run();
    return true;
}

int UserRepository::count() {
    Statement st = db_.prepare("SELECT COUNT(*) FROM users");
    st.step();
    return st.getInt(0);
}
