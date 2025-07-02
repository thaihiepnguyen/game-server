#include "command/jump_command.hpp"

void JumpCommand::execute(const std::shared_ptr<TCPConnection> &connection, const char *data, std::size_t length)
{
    _inputService->jump(connection);
}