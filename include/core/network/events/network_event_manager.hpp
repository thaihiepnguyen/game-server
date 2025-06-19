#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

class NetworkEventManager
{
private:
    std::unordered_map<std::string, std::vector<std::function<void()>>> _listeners;

    void notify(std::string eventName)
    {
        for (const auto &listener : _listeners[eventName])
        {
            listener();
        }
    }

    friend class TCPConnection; // Allow TCPConnection to access private members
public:
    void subscribe(std::string eventName, std::function<void()> callback)
    {
        _listeners[eventName].push_back(callback);
    }
};