#include "core/service/provider.hpp"
#include <stdexcept>

void Provider::addService(std::shared_ptr<IService> service)
{
    if (!service)
    {
        throw std::invalid_argument("Service cannot be null");
    }

    auto &s = *service.get();
    std::string name = typeid(s).name();

    if (_services.find(name) != _services.end())
    {
        throw std::runtime_error("Service already registered: " + name);
    }

    _services[name] = service;
}