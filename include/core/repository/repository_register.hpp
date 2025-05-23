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
    void registerRepo(std::shared_ptr<IRepository> repository);

    template<typename T>
    std::shared_ptr<T> getRepository();
};