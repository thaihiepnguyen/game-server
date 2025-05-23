#pragma once
#include <memory>
#include "core/service/service.hpp"
#include "core/repository/repository_register.hpp"
#include "repository/user_repository.hpp"

class UserService : public IService {
private:
    std::shared_ptr<UserRepository> _userRepository;

public:
    void inject(std::shared_ptr<RepositoryRegister> r) override {
        _userRepository = r->getRepository<UserRepository>();
    }
};