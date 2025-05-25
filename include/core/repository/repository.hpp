#pragma once
#include <string>
#include <vector>
#include "core/database/db_connection.hpp"

class Repository {
private:
    std::shared_ptr<IDatabaseConnection> _dbConnection;

public:
    Repository() : _dbConnection(nullptr) {}
    void setDatabaseConnection(std::shared_ptr<IDatabaseConnection> dbConnection);

    std::shared_ptr<IDatabaseConnection> db() const;

    template<typename T>
    T findById(long long id) {
        if (id <= 0) {
            throw std::invalid_argument("ID must be greater than 0");
        }

        if (!db()->isConnected()) {
            throw std::runtime_error("Database connection is not established");
        }

        std::string query = "SELECT * FROM " + T::tableName() + " WHERE id = " + std::to_string(id);
        DBResult result = db()->execute(query);
        if (result.size() == 0) {
            throw std::runtime_error("Entity not found with ID: " + std::to_string(id));
        }
        const auto& record = result[0];
        return T::fromRecord(record);
    }

    template<typename T>
    std::vector<T> findAll() {
        if (!db()->isConnected()) {
            throw std::runtime_error("Database connection is not established");
        }
        std::string query = "SELECT * FROM " + T::tableName();
        DBResult result = db()->execute(query);

        std::vector<T> entities;
        for (const auto& row : result.rows()) {
            entities.push_back(T::fromRecord(row));
        }
        return entities;
    }

    template<typename T>
    long long create(const T& entity) {
        if (!db()->isConnected()) {
            throw std::runtime_error("Database connection is not established");
        }

        std::unordered_map<std::string, std::string> record = T::toRecord(entity);
        record.erase("id"); // Ensure 'id' is not included in insert
        if (record.empty()) {
            throw std::runtime_error("Entity does not have any fields to insert");
        }

        std::string query = "INSERT INTO " + T::tableName() + " (";

        for (const auto& [key, value] : record) {
            query += key + ", ";
        }
        query.pop_back();
        query.pop_back();
        query += ") VALUES (";

        for (const auto& [key, value] : record) {
            query += "'" + value + "'" + ", ";
        }
        query.pop_back();
        query.pop_back();
        query += ")";
        db()->execute(query);

        std::string lastInsertIdQuery = "SELECT LAST_INSERT_ID() AS id";
        DBResult lastInsertIdResult = db()->execute(lastInsertIdQuery);
        if (lastInsertIdResult.size() == 0) {
            throw std::runtime_error("Failed to retrieve last insert ID");
        }

        return std::get<std::int64_t>(lastInsertIdResult[0].at("id"));
    }

    template<typename T>
    void update(const T& entity) {
        if (!db()->isConnected()) {
            throw std::runtime_error("Database connection is not established");
        }

        auto record = T::toRecord(entity);
        if (record.find("id") == record.end()) {
            throw std::runtime_error("Entity does not have an 'id' field");
        }

        std::string query = "UPDATE " + T::tableName() + " SET ";

        for (const auto& [key, value] : record) {
            if (key != "id") {
                query += key + " = '" + value + "', ";
            }
        }
        query.pop_back(); // Remove last comma
        query.pop_back(); // Remove last space
        query += " WHERE id = " + record["id"];

        std::cout << "Executing update query: " << query << std::endl;

        db()->execute(query);
    }

    template<typename T>
    void remove(long long id) {
        if (id <= 0) {
            throw std::invalid_argument("ID must be greater than 0");
        }

        if (!db()->isConnected()) {
            throw std::runtime_error("Database connection is not established");
        }

        std::string query = "DELETE FROM " + T::tableName() + " WHERE id = " + std::to_string(id);
        db()->execute(query);
    }
};