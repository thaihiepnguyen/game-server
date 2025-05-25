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