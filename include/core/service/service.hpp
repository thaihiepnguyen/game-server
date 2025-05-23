#pragma once
#include <memory>
#include "core/repository/repository_register.hpp"


class IService {
public:
    virtual ~IService() = default;
    virtual void inject(std::shared_ptr<RepositoryRegister> repositoryRegister) {};
};