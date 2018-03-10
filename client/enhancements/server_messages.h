#pragma once
#include "../messaging/messaging.h"
#include "../client_signature.h"

#include "../command/command.h"

/// Function for command chimera_block_server_messages
ChimeraCommandError block_server_messages_command(size_t argc, const char **argv) noexcept;
