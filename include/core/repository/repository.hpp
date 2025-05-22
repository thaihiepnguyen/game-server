#pragma once
#include <string>
#include <vector>

class IRepository {
public:
    virtual ~IRepository() = default;

    virtual void add(const std::string& item) = 0;
};