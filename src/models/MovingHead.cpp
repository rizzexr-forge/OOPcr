#include "models/MovingHead.h"

std::string MovingHead::getSpecs() const {
    return "pan=" + std::to_string(pan_) + ";tilt=" + std::to_string(tilt_) +
           ";gobo=" + std::to_string(gobo_);
}

void MovingHead::setSpecs(const std::string& specs) {
    auto m = parseSpecs(specs);
    pan_ = getInt(m, "pan");
    tilt_ = getInt(m, "tilt");
    gobo_ = getInt(m, "gobo");
}

std::string MovingHead::describe() const {
    return "Pan: " + std::to_string(pan_) + "°, Tilt: " + std::to_string(tilt_) +
           "°, гобо: " + std::to_string(gobo_);
}

std::vector<std::pair<std::string, std::string>> MovingHead::specFields() const {
    return {{"pan", "Pan, градусы"}, {"tilt", "Tilt, градусы"}, {"gobo", "Число гобо"}};
}
