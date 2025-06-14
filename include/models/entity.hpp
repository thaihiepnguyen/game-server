
#pragma once
#include <string>

class Entity {
public:
    long long id;
    std::string createdAt;
    std::string updatedAt;

    Entity() : id(0) {}
};