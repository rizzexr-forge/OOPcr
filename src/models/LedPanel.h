#pragma once
#include "models/Equipment.h"

// Светодиодная панель: размер, число диодов, RGB или нет
class LedPanel : public Equipment {
public:
    std::string getType() const override { return "ledpanel"; }
    std::string getTypeName() const override { return "LED-панель"; }
    std::string getSpecs() const override;
    void setSpecs(const std::string& specs) override;
    std::string describe() const override;
    std::vector<std::pair<std::string, std::string>> specFields() const override;

private:
    int sizeCm_ = 0;   // сторона, см
    int leds_ = 0;     // число диодов
    int rgb_ = 0;      // 1 - RGB, 0 - одноцветная
};
