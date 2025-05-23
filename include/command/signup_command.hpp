#pragma once
#include "core/command/command.hpp"
#include "core/service/provider.hpp"
#include "repository/user_repository.hpp"


class SignupCommand : public ICommand {
public:
    void inject(std::shared_ptr<Provider> provider) override {}

    std::unordered_map<std::string, Protocol::Value> execute(const std::unordered_map<std::string, Protocol::Value>& request) override {
        std::unordered_map<std::string, Protocol::Value> response;
        if (std::holds_alternative<std::monostate>(request.at("username"))) {
            response["status"] = "error";
            response["message"] = "Username is required.";
            return response;
        }
        std::string username = std::get<std::string>(request.at("username"));
        std::string password = std::get<std::string>(request.at("password"));
        std::unordered_map<std::string, Protocol::Value> userData = std::get<std::unordered_map<std::string, Protocol::Value>>(request.at("position"));

        response["status"] = "success";
        return response;
    }
};