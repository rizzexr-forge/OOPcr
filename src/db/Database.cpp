#include "db/Database.h"
#include <stdexcept>

// ---------- Statement ----------

Statement::Statement(sqlite3* db, const std::string& sql) : db_(db) {
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &st_, nullptr) != SQLITE_OK) {
        throw std::runtime_error(std::string("Ошибка подготовки запроса: ") + sqlite3_errmsg(db_));
    }
}

Statement::~Statement() {
    sqlite3_finalize(st_);
}

Statement& Statement::bind(int index, const std::string& value) {
    sqlite3_bind_text(st_, index, value.c_str(), -1, SQLITE_TRANSIENT);
    return *this;
}

Statement& Statement::bind(int index, int value) {
    sqlite3_bind_int(st_, index, value);
    return *this;
}

bool Statement::step() {
    int rc = sqlite3_step(st_);
    if (rc == SQLITE_ROW) return true;
    if (rc == SQLITE_DONE) return false;
    throw std::runtime_error(std::string("Ошибка выполнения запроса: ") + sqlite3_errmsg(db_));
}

void Statement::run() {
    step();
}

int Statement::getInt(int column) const {
    return sqlite3_column_int(st_, column);
}

std::string Statement::getText(int column) const {
    const unsigned char* text = sqlite3_column_text(st_, column);
    return text ? reinterpret_cast<const char*>(text) : "";
}

// ---------- Database ----------

Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        std::string msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        db_ = nullptr;
        throw std::runtime_error("Не удалось открыть БД: " + msg);
    }
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}

void Database::exec(const std::string& sql) {
    char* err = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg = err ? err : "неизвестная ошибка";
        sqlite3_free(err);
        throw std::runtime_error("Ошибка SQL: " + msg);
    }
}

Statement Database::prepare(const std::string& sql) {
    return Statement(db_, sql);
}

long long Database::lastInsertId() const {
    return sqlite3_last_insert_rowid(db_);
}

int Database::changes() const {
    return sqlite3_changes(db_);
}

void Database::initSchema() {
    exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id       INTEGER PRIMARY KEY AUTOINCREMENT,
            login    TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            role     TEXT NOT NULL
        );
    )");

    exec(R"(
        CREATE TABLE IF NOT EXISTS equipment (
            id               INTEGER PRIMARY KEY AUTOINCREMENT,
            type             TEXT NOT NULL,
            name             TEXT NOT NULL,
            inventory_number TEXT NOT NULL UNIQUE,
            status           TEXT NOT NULL DEFAULT 'in_stock',
            location         TEXT NOT NULL DEFAULT '',
            specs            TEXT NOT NULL DEFAULT ''
        );
    )");

    exec(R"(
        CREATE TABLE IF NOT EXISTS rentals (
            id           INTEGER PRIMARY KEY AUTOINCREMENT,
            equipment_id INTEGER NOT NULL,
            client_id    INTEGER NOT NULL,
            worker_id    INTEGER NOT NULL,
            date_from    TEXT NOT NULL,
            date_to      TEXT NOT NULL,
            returned     INTEGER NOT NULL DEFAULT 0
        );
    )");
}
