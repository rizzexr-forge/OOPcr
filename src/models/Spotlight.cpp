#include "models/Spotlight.h"

std::string Spotlight::getSpecs() const {
    return "power=" + std::to_string(power_) + ";angle=" + std::to_string(angle_);
}

void Spotlight::setSpecs(const std::string& specs) {
    auto m = parseSpecs(specs);
    power_ = getInt(m, "power");
    angle_ = getInt(m, "angle");
}

std::string Spotlight::describe() const {
    return "Мощность: " + std::to_string(power_) + " Вт, угол луча: " + std::to_string(angle_) + "°";
}

std::vector<std::pair<std::string, std::string>> Spotlight::specFields() const {
    return {{"power", "Мощность, Вт"}, {"angle", "Угол луча, градусы"}};
}
