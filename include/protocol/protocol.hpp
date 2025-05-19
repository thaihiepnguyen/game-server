#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iostream>


namespace Protocol {
    struct Packet;
    Packet decode(const std::string& jsonString);
    std::string encode(const Packet& packet);
    
    using Value = std::optional<std::variant<int64_t, double, std::string, bool, std::nullptr_t>>;

    enum class Command : unsigned short {
        // Client to Server
        LOGIN = 100,
        LOGOUT = 103,
        SEND_MESSAGE,
        GET_MESSAGES,
        GET_USERS,

        // Server to Client
        LOGIN_SUCCESS,
        LOGIN_FAILURE,
        MESSAGE_RECEIVED,
        USERS_LIST,
    };

    struct Packet {
        Command command;
        std::map<std::string, Value> data;

        Packet(Command cmd, const std::map<std::string, Value>& d) : command(cmd), data(d) {}

        std::string toString() const {
            return Protocol::encode(*this);
        }
    };
}