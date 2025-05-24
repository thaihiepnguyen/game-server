#pragma once
#include <memory>
#include "core/service/service.hpp"
#include "core/repository/repository_register.hpp"
#include "repository/user_repository.hpp"

class UserService : public IService {
private:
    std::shared_ptr<UserRepository> _userRepository;

public:
    void inject(std::shared_ptr<RepositoryRegister> repo) override {
        _userRepository = repo->getRepository<UserRepository>();
    }

    void createUser(const std::string& username, const std::string& password, const std::string& salt);
};