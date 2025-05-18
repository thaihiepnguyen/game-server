#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <cstdint>
#include <memory>
#include <iostream>

class RowResult {
public:
    using Value = std::variant<int64_t, double, std::string, bool, std::nullptr_t>;
    using Row = std::vector<Value>;

    RowResult() = default;

    void addRow(const Row& row);

    const std::vector<Row>& rows() const;
    size_t size() const;
    const Row& operator[](size_t idx) const;

private:
    std::vector<Row> _rows;
};