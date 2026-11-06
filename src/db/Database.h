#pragma once
#include <string>
#include <sqlite3.h>

// Подготовленный запрос. Сам освобождается в деструкторе (RAII),
// поэтому вызывать sqlite3_finalize вручную не нужно.
class Statement {
public:
    Statement(sqlite3* db, const std::string& sql);
    ~Statement();

    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    // подстановка параметров "?" (нумерация с 1)
    Statement& bind(int index, const std::string& value);
    Statement& bind(int index, int value);

    // следующая строка результата: true - строка есть, false - строки кончились
    bool step();

    // выполнить запрос без чтения результата (INSERT, UPDATE, DELETE)
    void run();

    // чтение столбцов текущей строки (нумерация с 0)
    int getInt(int column) const;
    std::string getText(int column) const;

private:
    sqlite3* db_;
    sqlite3_stmt* st_ = nullptr;
};

// Обёртка над соединением с SQLite.
class Database {
public:
    explicit Database(const std::string& path);
    ~Database();

    // копировать соединение с БД нельзя
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // запрос без параметров и без результата
    void exec(const std::string& sql);

    // подготовленный запрос с параметрами
    Statement prepare(const std::string& sql);

    // id последней вставленной строки
    long long lastInsertId() const;

    // сколько строк изменил последний запрос
    int changes() const;

    // создаёт таблицы, если их ещё нет
    void initSchema();

private:
    sqlite3* db_ = nullptr;
};
