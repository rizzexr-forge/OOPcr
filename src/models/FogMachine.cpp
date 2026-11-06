#include "models/FogMachine.h"

std::string FogMachine::getSpecs() const {
    return "output=" + std::to_string(output_) + ";tank=" + std::to_string(tank_);
}

void FogMachine::setSpecs(const std::string& specs) {
    auto m = parseSpecs(specs);
    output_ = getInt(m, "output");
    tank_ = getInt(m, "tank");
}

std::string FogMachine::describe() const {
    return "Производительность: " + std::to_string(output_) + " м³/мин, бак: " +
           std::to_string(tank_) + " л";
}

std::vector<std::pair<std::string, std::string>> FogMachine::specFields() const {
    return {{"output", "Производительность, м3/мин"}, {"tank", "Объём бака, л"}};
}
