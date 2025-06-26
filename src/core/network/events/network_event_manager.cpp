#include "core/network/events/network_event_manager.hpp"

void NetworkEventManager::notify(std::string eventName)
{
    for (const auto &listener : _listeners[eventName])
    {
        listener();
    }
}

void NetworkEventManager::subscribe(std::string eventName, std::function<void()> callback)
{
    _listeners[eventName].push_back(callback);
}