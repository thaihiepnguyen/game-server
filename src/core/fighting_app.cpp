#include "core/fighting_app.hpp"

FightingGameApplication::FightingGameApplication() {
    _repository = std::make_shared<Repository>();
    _provider = std::make_shared<Provider>();
}

void FightingGameApplication::_handleCommand(const std::shared_ptr<TCPConnection>& connection, Protocol::Packet& packet) {
    auto it = _commands.find(packet.command);

    if (it == _commands.end()) {
        throw std::runtime_error("Command not found");
    }
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
        handler->execute(connection, packet);
    }
}

FightingGameApplication* FightingGameApplication::registerAuthMiddleware(ICommand* middleware) {
    _authMiddleware = std::shared_ptr<ICommand>(middleware);

    Logger::logInfo("Authentication Middleware registered: " 
        + String::demangle(typeid(*middleware).name()));
    return this;
}

FightingGameApplication* FightingGameApplication::registerMiddleware(ICommand* middleware) {
    _middlewares.push_back(std::shared_ptr<ICommand>(middleware));

    Logger::logInfo("Middleware registered: " + String::demangle(typeid(*middleware).name()));
    return this;
}

FightingGameApplication* FightingGameApplication::registerCommand(Protocol::Command id, ICommand* command, bool isPublic = false) {
    std::shared_ptr<ICommand> cmdPtr(command);

    if (!isPublic) {
        _authMap[cmdPtr] = _authMiddleware;
    }
    
    _commands[id] = cmdPtr;
    _commands[id]->inject(_provider);


    Logger::logInfo("Command registered: " + String::demangle(typeid(*command).name()) + " -> command " + std::to_string(static_cast<unsigned int>(id)));
    return this;
}

FightingGameApplication* FightingGameApplication::registerService(IService* service) {
    auto sharedService = std::shared_ptr<IService>(service);
    _provider->addService(sharedService);
    sharedService->inject(_provider);
    sharedService->setRepository(_repository);

    Logger::logInfo("Service registered: " + String::demangle(typeid(*service).name()));
    return this;
}

FightingGameApplication* FightingGameApplication::registerDatabaseConnection(IDatabaseConnection* dbConnection) {
    _dbConnection = std::shared_ptr<IDatabaseConnection>(dbConnection);
    _repository->setDatabaseConnection(_dbConnection);
    return this;
}

FightingGameApplication* FightingGameApplication::start(unsigned short port) {
    auto server = std::make_shared<TCPServer>();

    if (server->bind(port)) {
        server->listen();

        server->accept([&](auto connection, std::string msg) {
            Protocol::Packet packet = Protocol::decode(msg);
            try {
                this->_handleCommand(connection, packet);
            } catch (const std::exception& e) {
                std::unordered_map<std::string, Protocol::Value> res;
                res["status"] = Protocol::Value("error");
                res["message"] = Protocol::Value(e.what());
                Protocol::Packet errorPacket(packet.command, res);
                std::string errorJson = Protocol::encode(errorPacket);
                connection->send(errorJson);
            }
        });
    }

    return this;
}

FightingGameApplication::~FightingGameApplication() {
    if (_dbConnection) {
        _dbConnection->disconnect();
    }
}