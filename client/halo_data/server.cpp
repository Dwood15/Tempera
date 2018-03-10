#include "server.h"

#include "../client_signature.h"

ServerType server_type() {
    static auto *server_type = *reinterpret_cast<ServerType **>(get_signature("server_type_sig").address() + 3);
    return *server_type;
}

Gametype gametype() {
    static auto *gametype = *reinterpret_cast<Gametype **>(get_signature("current_gametype_sig").address() + 2);
    return *gametype;
}
