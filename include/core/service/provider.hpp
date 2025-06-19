#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "core/service/service.hpp"
#include <stdexcept>

class FightingGameApplication; // Forward declaration to allow access to private members
class Provider
{
private:
    std::unordered_map<std::string, std::shared_ptr<IService>> _services;
    void addService(std::shared_ptr<IService> service);

    friend class FightingGameApplication; // Allow FightingGameApplication to access private members
public:
    template <typename T>
    std::shared_ptr<T> getService()
    {
        std::string name = typeid(T).name();
        auto it = _services.find(name);
        if (it == _services.end())
        {
            throw std::runtime_error("Service not found: " + name);
        }
        return std::dynamic_pointer_cast<T>(it->second);
    }
};