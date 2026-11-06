#pragma once
#include "models/Equipment.h"

// Вращающаяся голова: диапазон pan/tilt и число гобо
class MovingHead : public Equipment {
public:
    std::string getType() const override { return "movinghead"; }
    std::string getTypeName() const override { return "Вращающаяся голова"; }
    std::string getSpecs() const override;
    void setSpecs(const std::string& specs) override;
    std::string describe() const override;
    std::vector<std::pair<std::string, std::string>> specFields() const override;

private:
    int pan_ = 0;    // градусы
    int tilt_ = 0;   // градусы
    int gobo_ = 0;   // количество гобо
};
