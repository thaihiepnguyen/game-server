#include "protocol/protocol.hpp"


namespace Protocol {
    std::string encode(const Packet& packet) {
        nlohmann::json json;
        json["command"] = static_cast<unsigned short>(packet.command);
        for (const auto& [key, value] : packet.data) {
            if (value.has_value()) {
                if (std::holds_alternative<int64_t>(value.value())) {
                    json["data"][key] = std::get<int64_t>(value.value());
                } else if (std::holds_alternative<double>(value.value())) {
                    json["data"][key] = std::get<double>(value.value());
                } else if (std::holds_alternative<std::string>(value.value())) {
                    json["data"][key] = std::get<std::string>(value.value());
                } else if (std::holds_alternative<bool>(value.value())) {
                    json["data"][key] = std::get<bool>(value.value());
                }
            } else {
                json["data"][key] = nullptr;
            }
        }
        return json.dump();
    }

    Packet decode(const std::string& jsonString) {
        auto json = nlohmann::json::parse(jsonString);
        Command cmd = static_cast<Command>(json["command"].get<unsigned short>());
        std::map<std::string, Value> data;

        for (auto& [key, value] : json["data"].items()) {
            if (value.is_null()) {
                data[key] = std::nullopt;
            } else if (value.is_boolean()) {
                data[key] = value.get<bool>();
            } else if (value.is_number_integer()) {
                data[key] = value.get<int64_t>();
            } else if (value.is_number_float()) {
                data[key] = value.get<double>();
            } else if (value.is_string()) {
                data[key] = value.get<std::string>();
            }
        }

        return Packet(cmd, data);
    }
}
