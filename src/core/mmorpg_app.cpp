#include "core/mmorpg_app.hpp"



std::unordered_map<std::string, Protocol::Value> MMORPGApplication::_handleCommand(Protocol::Command id, const std::unordered_map<std::string, Protocol::Value>& request) {
    auto it = _commands.find(id);
    if (it == _commands.end()) {
        throw std::runtime_error("Command not found");
    }
    std::unordered_map<std::string, Protocol::Value> response;

    std::vector<std::shared_ptr<ICommand>> handlers;
    auto command = it->second;
    
    // Check if the command requires authentication
    auto itAuth = _authMap.find(command);
    if (itAuth != _authMap.end()) {
        handlers.push_back(itAuth->second);
    }

    // Global middlewares
    for (const auto& middleware : _middlewares) {
        handlers.push_back(middleware);
    }

    handlers.push_back(command);
    for (const auto& handler : handlers) {
        response = handler->execute(request);
    }

    return response;
}