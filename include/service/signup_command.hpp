#include "core/command/command.hpp"
#include "core/repository/repository_register.hpp"
#include "core/repository/user_repository.hpp"


class SignupCommand : public ICommand {
private:
    std::shared_ptr<UserRepository> _userRepository;

public:
    void inject(std::shared_ptr<RepositoryRegister> repositoryRegister) override {
        _userRepository = repositoryRegister->getRepository<UserRepository>();
    }

    std::unordered_map<std::string, Protocol::Value> execute(const std::unordered_map<std::string, Protocol::Value>& request) override {
        _userRepository->add("test");


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