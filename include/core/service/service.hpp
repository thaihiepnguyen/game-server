#pragma once
#include <memory>
#include <core/database/db_connection.hpp>
#include "core/repository/repository.hpp"

class Provider; // Forward declaration to avoid circular dependency
class IService {
private:
    std::shared_ptr<Repository> _repository;

public:
    virtual ~IService() = default;

    /**
     * Injects a provider into the service.
     * Don't store the provider as a shared_ptr , as it may lead to circular dependencies.
     * Just use it to access other services instead.
     */
    virtual void inject(std::shared_ptr<Provider> provider) {};

    virtual void setRepository(std::shared_ptr<Repository> repository) final {
        _repository = repository;
    }

    virtual std::shared_ptr<Repository> repository() const {
        if (!_repository) {
            throw std::runtime_error("Repository is not set for the service");
        }
        return _repository;
    }
};