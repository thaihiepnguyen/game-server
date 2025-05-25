#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "core/service/service.hpp"

class Provider {
private:
    std::unordered_map<std::string, std::shared_ptr<IService>> _services;

public:
    void addService(std::shared_ptr<IService> service);

    template<typename T>
    std::shared_ptr<T> getService() {
        std::string name = typeid(T).name();
        auto it = _services.find(name);
        if (it == _services.end()) {
            throw std::runtime_error("Service not found: " + name);
        }
        return std::dynamic_pointer_cast<T>(it->second);
    }
};