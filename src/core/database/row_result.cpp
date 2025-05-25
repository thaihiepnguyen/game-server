#include "core/database/db_result.hpp"

void DBResult::addRow(const Record& row) {
    _rows.push_back(row);
}

const std::vector<DBResult::Record>& DBResult::rows() const { return _rows; }

size_t DBResult::size() const { return _rows.size(); }

const DBResult::Record& DBResult::operator[](size_t idx) const { return _rows[idx]; }