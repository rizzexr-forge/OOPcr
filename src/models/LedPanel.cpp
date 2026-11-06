#include "models/LedPanel.h"

std::string LedPanel::getSpecs() const {
    return "size=" + std::to_string(sizeCm_) + ";leds=" + std::to_string(leds_) +
           ";rgb=" + std::to_string(rgb_);
}

void LedPanel::setSpecs(const std::string& specs) {
    auto m = parseSpecs(specs);
    sizeCm_ = getInt(m, "size");
    leds_ = getInt(m, "leds");
    rgb_ = getInt(m, "rgb") ? 1 : 0;
}

std::string LedPanel::describe() const {
    return "Размер: " + std::to_string(sizeCm_) + " см, диодов: " + std::to_string(leds_) +
           ", RGB: " + (rgb_ ? "да" : "нет");
}

std::vector<std::pair<std::string, std::string>> LedPanel::specFields() const {
    return {{"size", "Размер стороны, см"}, {"leds", "Число диодов"}, {"rgb", "RGB (1 - да, 0 - нет)"}};
}
