#include "core/repository/repository_register.hpp"


void RepositoryRegister::registerRepo(std::shared_ptr<IRepository> repository) {
    if (!repository.get()) {
        throw std::invalid_argument("Repository cannot be null");
    }

    auto& r = *repository.get();
    std::string name = typeid(r).name();
    
    if (repositories.find(name) != repositories.end()) {
        throw std::runtime_error("Repository already registered: " + name);
    }

    repositories[name] = repository;
}

template<typename T>
std::shared_ptr<T> RepositoryRegister::getRepository() {
    std::string name = typeid(T).name();
    auto it = repositories.find(name);
    if (it == repositories.end()) {
        throw std::runtime_error("Repository not found: " + name);
    }
    return std::dynamic_pointer_cast<T>(it->second);
}
