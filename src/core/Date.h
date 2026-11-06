#pragma once
#include <string>

// Даты хранятся строками "ГГГГ-ММ-ДД", их можно сравнивать как обычные строки.
namespace date {

// сегодняшняя дата
std::string today();

// строка - настоящая дата в формате ГГГГ-ММ-ДД
bool isValid(const std::string& s);

} // namespace date
