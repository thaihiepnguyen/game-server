#include "database/row_result.hpp"

void RowResult::addRow(const Row& row) {
    _rows.push_back(row);
}

const std::vector<RowResult::Row>& RowResult::rows() const { return _rows; }

size_t RowResult::size() const { return _rows.size(); }

const RowResult::Row& RowResult::operator[](size_t idx) const { return _rows[idx]; }