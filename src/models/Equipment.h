#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

enum class Status { InStock, Rented };

// перевод статуса в строку для БД и обратно
inline std::string statusToDb(Status s) {
    return s == Status::InStock ? "in_stock" : "rented";
}
inline Status statusFromDb(const std::string& s) {
    return s == "rented" ? Status::Rented : Status::InStock;
}

// Абстрактный базовый класс всех приборов.
class Equipment {
public:
    virtual ~Equipment() = default;   // обязателен для базового класса

    // геттеры
    int id() const { return id_; }
    const std::string& name() const { return name_; }
    const std::string& inventoryNumber() const { return inventoryNumber_; }
    Status status() const { return status_; }
    const std::string& location() const { return location_; }

    // сеттеры
    void setId(int v) { id_ = v; }
    void setName(const std::string& v) { name_ = v; }
    void setInventoryNumber(const std::string& v) { inventoryNumber_ = v; }
    void setStatus(Status v) { status_ = v; }
    void setLocation(const std::string& v) { location_ = v; }

    // у каждого наследника своё
    virtual std::string getType() const = 0;      // для БД: "spotlight"
    virtual std::string getTypeName() const = 0;  // для экрана: "Прожектор"
    virtual std::string getSpecs() const = 0;     // "power=300;angle=25"
    virtual void setSpecs(const std::string& specs) = 0;
    virtual std::string describe() const = 0;     // "Мощность: 300 Вт, ..."

    // список запрашиваемых при добавлении характеристик: {ключ, подпись}
    virtual std::vector<std::pair<std::string, std::string>> specFields() const = 0;

    // общий вывод; внутри вызывает describe() наследника (полиморфизм)
    virtual void print() const;

protected:
    // "a=1;b=2" -> {a:"1", b:"2"}
    static std::map<std::string, std::string> parseSpecs(const std::string& specs);
    static int getInt(const std::map<std::string, std::string>& m, const std::string& key);
    static std::string getStr(const std::map<std::string, std::string>& m, const std::string& key);

private:
    int id_ = 0;
    std::string name_;
    std::string inventoryNumber_;
    Status status_ = Status::InStock;
    std::string location_;
};
