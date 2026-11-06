#pragma once
#include <iostream>
#include <string>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace console {

// цвета
inline constexpr const char* RED    = "\033[31m";
inline constexpr const char* GREEN  = "\033[32m";
inline constexpr const char* YELLOW = "\033[33m";
inline constexpr const char* CYAN   = "\033[36m";
inline constexpr const char* RESET  = "\033[0m";

// бросается, если ввод закончился (Ctrl+Z / Ctrl+D); ловится в main
struct InputClosed {};

// вызвать один раз в начале main
inline void init() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) {
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

inline void clear() {
    std::cout << "\033[2J\033[H";
}

// очищает экран и печатает заголовок раздела
inline void title(const std::string& text) {
    clear();
    std::cout << CYAN << "=== " << text << " ===" << RESET << "\n\n";
}

// читает строку целиком (с пробелами)
inline std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    if (!std::getline(std::cin, s)) {
        throw InputClosed{};
    }
    return s;
}

// читает строку, пока не введут что-то непустое
inline std::string readNonEmpty(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        if (!s.empty()) return s;
        std::cout << RED << "Поле не может быть пустым!" << RESET << "\n";
    }
}

// читает число, пока не введут нормально
inline int readInt(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            size_t pos = 0;
            int v = std::stoi(s, &pos);
            if (pos == s.size()) return v;
        } catch (...) {}
        std::cout << RED << "Введите число!" << RESET << "\n";
    }
}

inline void pause() {
    readLine("\nНажмите Enter...");
}

} // namespace console
