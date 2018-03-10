#include "keyboard.h"

#include "../client_signature.h"
#include "../command/console.h"
#include "../keystone.h"
#include "chat.h"
#include "server.h"
#include "../messaging/messaging.h"

KeyboardKeys &get_keyboard_keys() noexcept {
    static KeyboardKeys *buffer = nullptr;
    if(!buffer) {
        buffer = *reinterpret_cast<KeyboardKeys **>(get_signature("keyboard_keys_sig").address() + 1);
    }
    return *buffer;
}

void check_keys() noexcept {
    extern bool keystone_enabled;
    if(server_type() != SERVER_NONE && !console_is_out() && keystone_enabled) {
        static char chat_key = 0;
        if(get_keyboard_keys().t) {
            chat_key = 1;
            console_is_out(true, "chimera_chat all ");
        }
        else if(get_keyboard_keys().y) {
            chat_key = 2;
            console_is_out(true, "chimera_chat team ");
        }
        else if(get_keyboard_keys().h) {
            chat_key = 3;
            console_is_out(true, player_can_use_vehicle_chat() ? "chimera_chat vehicle " : "chimera_chat team ");
        }
        else {
            chat_key = 0;
        }
    }
}
