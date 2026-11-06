#pragma once
#include "models/Equipment.h"

// Прожектор: мощность и угол луча
class Spotlight : public Equipment {
public:
    std::string getType() const override { return "spotlight"; }
    std::string getTypeName() const override { return "Прожектор"; }
    std::string getSpecs() const override;
    void setSpecs(const std::string& specs) override;
    std::string describe() const override;
    std::vector<std::pair<std::string, std::string>> specFields() const override;

private:
    int power_ = 0;   // Вт
    int angle_ = 0;   // градусы
};
