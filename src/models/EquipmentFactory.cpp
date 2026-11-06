#include "models/EquipmentFactory.h"
#include "models/Spotlight.h"
#include "models/MovingHead.h"
#include "models/LedPanel.h"
#include "models/FogMachine.h"

std::unique_ptr<Equipment> EquipmentFactory::create(const std::string& type) {
    if (type == "spotlight")  return std::make_unique<Spotlight>();
    if (type == "movinghead") return std::make_unique<MovingHead>();
    if (type == "ledpanel")   return std::make_unique<LedPanel>();
    if (type == "fogmachine") return std::make_unique<FogMachine>();
    return nullptr;
}

const std::vector<std::string>& EquipmentFactory::types() {
    static const std::vector<std::string> t = {"spotlight", "movinghead", "ledpanel", "fogmachine"};
    return t;
}
