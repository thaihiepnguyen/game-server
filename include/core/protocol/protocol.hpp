#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iostream>
#include <variant>


namespace Protocol {
    struct Packet;
    struct Value;

    Packet decode(const std::string& jsonString);
    std::string encode(const Packet& packet);
    
    using Object = std::unordered_map<std::string, Value>;
    using Array = std::vector<Value>;

    struct Value : std::variant<
        std::monostate,
        int64_t,
        double,
        std::string,
        bool,
        Object,
        Array
    > {
        using variant::variant;
    };

    enum class Command : unsigned short {
        // Client to Server
        SIGN_UP = 1,
        SIGN_IN = 2,
        TEST = 3,


        // Server to Client
    };

    struct Packet {
        Command command;
        std::unordered_map<std::string, Value> data;

        Packet(Command cmd, const std::unordered_map<std::string, Value>& d) : command(cmd), data(d) {}

        std::string toString() const {
            return Protocol::encode(*this);
        }
    };
}