#include "core/command/command.hpp"
#include "core/repository/repository_register.hpp"

class AuthCommand : public ICommand {
public:
    void inject(std::shared_ptr<RepositoryRegister> repositoryRegister) override {
        // Inject dependencies if needed
    }


    std::unordered_map<std::string, Protocol::Value> execute(const std::unordered_map<std::string, Protocol::Value>& request) override {
        std::cout << "Executing AuthCommand...\n";
        return request;
    }
};