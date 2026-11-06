#pragma once
#include <memory>
#include <string>
#include <vector>
#include "models/Equipment.h"

// Создаёт нужный наследник Equipment по строке типа из БД.
class EquipmentFactory {
public:
    // вернёт nullptr, если тип неизвестен
    static std::unique_ptr<Equipment> create(const std::string& type);

    // все типы, которые умеет создавать фабрика (для меню выбора)
    static const std::vector<std::string>& types();
};
