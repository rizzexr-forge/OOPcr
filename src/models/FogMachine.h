#pragma once
#include "models/Equipment.h"

// Дым-машина: производительность и объём бака
class FogMachine : public Equipment {
public:
    std::string getType() const override { return "fogmachine"; }
    std::string getTypeName() const override { return "Дым-машина"; }
    std::string getSpecs() const override;
    void setSpecs(const std::string& specs) override;
    std::string describe() const override;
    std::vector<std::pair<std::string, std::string>> specFields() const override;

private:
    int output_ = 0;   // м3/мин
    int tank_ = 0;     // литры
};
