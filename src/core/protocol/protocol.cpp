#include "core/protocol/protocol.hpp"


namespace Protocol {
    std::string encode(const Packet& packet) {
        nlohmann::json j;
        j["command"] = static_cast<unsigned short>(packet.command);

        std::function<nlohmann::json(const Value&)> to_json;
        to_json = [&](const Value& val) -> nlohmann::json {
            if (std::holds_alternative<std::monostate>(val)) return nullptr;
            if (std::holds_alternative<int64_t>(val)) return std::get<int64_t>(val);
            if (std::holds_alternative<double>(val)) return std::get<double>(val);
            if (std::holds_alternative<std::string>(val)) return std::get<std::string>(val);
            if (std::holds_alternative<bool>(val)) return std::get<bool>(val);
            if (std::holds_alternative<Protocol::Object>(val)) {
                nlohmann::json obj = nlohmann::json::object();
                for (const auto& [key, nestedVal] : std::get<Protocol::Object>(val)) {
                    obj[key] = to_json(nestedVal);
                }
                return obj;
            }
            if (std::holds_alternative<Protocol::Array>(val)) {
                nlohmann::json arr = nlohmann::json::array();
                for (const auto& item : std::get<Protocol::Array>(val)) {
                    arr.push_back(to_json(item));
                }
                return arr;
            }
            return nullptr;
        };

        nlohmann::json j_data = nlohmann::json::object();
        for (const auto& [key, val] : packet.data) {
            j_data[key] = to_json(val);
        }

        j["data"] = j_data;
        return j.dump();
    }

    Packet decode(const std::string& jsonString) {
        nlohmann::json j = nlohmann::json::parse(jsonString);
        Command command = static_cast<Command>(j["command"].get<unsigned short>());
        Object data;

        std::function<Value(const nlohmann::json&)> from_json;
        from_json = [&](const nlohmann::json& j) -> Value {
            if (j.is_null()) return std::monostate{};
            if (j.is_number_integer()) return j.get<int64_t>();
            if (j.is_number_float()) return j.get<double>();
            if (j.is_string()) return j.get<std::string>();
            if (j.is_boolean()) return j.get<bool>();
            if (j.is_object()) {
                Object obj;
                for (auto it = j.begin(); it != j.end(); ++it) {
                    obj[it.key()] = from_json(it.value());
                }
                return obj;
            }
            if (j.is_array()) {
                Array arr;
                for (const auto& item : j) {
                    arr.push_back(from_json(item));
                }
                return arr;
            }
            return std::monostate{};
        };

        for (auto it = j["data"].begin(); it != j["data"].end(); ++it) {
            data[it.key()] = from_json(it.value());
        }

        return Packet(command, data);
    }
}
