#include "core/command/command.hpp"


class SignupCommand : public ICommand {
public:
    std::map<std::string, Protocol::Value> execute(const std::map<std::string, Protocol::Value>& request) override {
        std::map<std::string, Protocol::Value> response;
        if (std::holds_alternative<std::monostate>(request.at("username"))) {
            response["status"] = "error";
            response["message"] = "Username is required.";
            return response;
        }
        std::string username = std::get<std::string>(request.at("username"));
        std::string password = std::get<std::string>(request.at("password"));
        std::map<std::string, Protocol::Value> userData = std::get<std::map<std::string, Protocol::Value>>(request.at("position"));

        response["status"] = "success";
        return response;
    }
};