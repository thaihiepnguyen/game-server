#pragma once

#include <vector>
#include <string>
#include <variant>
#include <cstdint>
#include <memory>
#include <iostream>
#include <unordered_map>

class DBResult {
public:
    using Value = std::variant<        
        std::monostate,
        int64_t,
        double,
        std::string,
        bool
    >;
    using Record = std::unordered_map<std::string, Value>;

    DBResult() = default;

    void addRow(const Record& row);

    const std::vector<Record>& rows() const;
    size_t size() const;
    const Record& operator[](size_t idx) const;

private:
    std::vector<Record> _rows;
};