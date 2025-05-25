#include "core/mmorpg_app.hpp"

MMORPGApplication::MMORPGApplication() {
    _repository = std::make_shared<Repository>();
    _provider = std::make_shared<Provider>();
}

std::unordered_map<std::string, Protocol::Value> MMORPGApplication::_handleCommand(Protocol::Command id, const std::unordered_map<std::string, Protocol::Value>& request) {
    auto it = _commands.find(id);
    if (it == _commands.end()) {
        throw std::runtime_error("Command not found");
    }
    std::unordered_map<std::string, Protocol::Value> response = request;

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
        response = handler->execute(response);
    }

    return response;
}

MMORPGApplication* MMORPGApplication::registerAuthMiddleware(ICommand* middleware) {
    _authMiddleware = std::shared_ptr<ICommand>(middleware);

    Logger::logInfo("Authentication Middleware registered: " 
        + String::demangle(typeid(*middleware).name()));
    return this;
}

MMORPGApplication* MMORPGApplication::registerMiddleware(ICommand* middleware) {
    _middlewares.push_back(std::shared_ptr<ICommand>(middleware));

    Logger::logInfo("Middleware registered: " + String::demangle(typeid(*middleware).name()));
    return this;
}

MMORPGApplication* MMORPGApplication::registerCommand(Protocol::Command id, ICommand* command, bool isPublic = false) {
    std::shared_ptr<ICommand> cmdPtr(command);

    if (!isPublic) {
        _authMap[cmdPtr] = _authMiddleware;
    }
    
    _commands[id] = cmdPtr;
    _commands[id]->inject(_provider);


    Logger::logInfo("Command registered: " + String::demangle(typeid(*command).name()) + " -> command " + std::to_string(static_cast<unsigned int>(id)));
    return this;
}

MMORPGApplication* MMORPGApplication::registerService(IService* service) {
    auto sharedService = std::shared_ptr<IService>(service);
    _provider->addService(sharedService);
    sharedService->inject(_provider);
    sharedService->setRepository(_repository);

    Logger::logInfo("Service registered: " + String::demangle(typeid(*service).name()));
    return this;
}

MMORPGApplication* MMORPGApplication::registerDatabaseConnection(IDatabaseConnection* dbConnection) {
    _dbConnection = std::shared_ptr<IDatabaseConnection>(dbConnection);
    _repository->setDatabaseConnection(_dbConnection);
    return this;
}

MMORPGApplication* MMORPGApplication::listen(unsigned short port) {
    _port = port;
    auto server = std::make_shared<TCPServer>();
    server->run(_port, [this](const Protocol::Packet& packet) {
        return this->_handleCommand(packet.command, packet.data);
    });

    return this;
}

MMORPGApplication::~MMORPGApplication() {
    if (_dbConnection) {
        _dbConnection->disconnect();
    }
}