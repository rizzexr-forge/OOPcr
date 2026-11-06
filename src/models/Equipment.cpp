#include "models/Equipment.h"
#include "core/Console.h"
#include <iostream>
#include <sstream>

std::map<std::string, std::string> Equipment::parseSpecs(const std::string& specs) {
    std::map<std::string, std::string> result;
    std::stringstream ss(specs);
    std::string part;
    while (std::getline(ss, part, ';')) {
        size_t eq = part.find('=');
        if (eq != std::string::npos) {
            result[part.substr(0, eq)] = part.substr(eq + 1);
        }
    }
    return result;
}

int Equipment::getInt(const std::map<std::string, std::string>& m, const std::string& key) {
    auto it = m.find(key);
    if (it == m.end()) return 0;
    try { return std::stoi(it->second); } catch (...) { return 0; }
}

std::string Equipment::getStr(const std::map<std::string, std::string>& m, const std::string& key) {
    auto it = m.find(key);
    return it == m.end() ? "" : it->second;
}

void Equipment::print() const {
    std::cout << console::CYAN << "[" << id_ << "] " << console::RESET
              << getTypeName() << " | " << name_
              << " | инв. " << inventoryNumber_ << " | ";
    if (status_ == Status::InStock)
        std::cout << console::GREEN << "на складе" << console::RESET;
    else
        std::cout << console::YELLOW << "в аренде" << console::RESET;
    std::cout << "\n    " << describe() << "\n";
}
