#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "core/repository/repository.hpp"

class RepositoryRegister {
private:
    std::unordered_map<std::string, std::shared_ptr<IRepository>> repositories;
public:
    void registerRepository(std::shared_ptr<IRepository> repository) {
        if (!repository.get()) {
            throw std::invalid_argument("Repository cannot be null");
        }

        auto& r = *repository.get();
        std::string name = typeid(r).name();
        
        std::cout << "Registering repository: " << name << "\n";
        if (repositories.find(name) != repositories.end()) {
            throw std::runtime_error("Repository already registered: " + name);
        }

        repositories[name] = repository;
    }

    template<typename T>
    std::shared_ptr<T> getRepository() {
        std::string name = typeid(T).name();
        auto it = repositories.find(name);
        if (it == repositories.end()) {
            throw std::runtime_error("Repository not found: " + name);
        }
        return std::dynamic_pointer_cast<T>(it->second);
    }
};