#pragma once
#include <memory>

class Provider;                // Forward declaration to avoid circular dependency
class FightingGameApplication; // Forward declaration to allow access to private members
class IService
{
private:
    /**
     * Injects a provider into the service.
     * Don't store the provider as a shared_ptr , as it may lead to circular dependencies.
     * Just use it to access other services instead.
     */
    virtual void inject(std::shared_ptr<Provider> provider) {};

    friend class FightingGameApplication; // Allow FightingGameApplication to access private members
public:
    virtual ~IService() = default;
};