#pragma once
#include <nlohmann/json.hpp>

class Position {
public:
    float x;
    float y;

    Position() : x(0), y(0) {}
    Position(float x, float y) : x(x), y(y) {}

    friend void to_json(nlohmann::json& j, const Position& p) {
        j = nlohmann::json{{"x", p.x}, {"y", p.y}};
    }

    friend void from_json(const nlohmann::json& j, Position& p) {
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
    }
};