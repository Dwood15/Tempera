#include "keystone.h"

#include "client_signature.h"
#include "messaging/messaging.h"
#include "settings.h"

bool custom_keystone_in_use() noexcept {
    static int found = -1;
    if(found == -1) {
        auto *module = GetModuleHandle("keystone.dll");
        if(!module) return false;
        auto **bird = reinterpret_cast<const char **>(GetProcAddress(module, "bird"));
        found = (bird != 0 && strcmp(*bird, "(^)>") == 0);
    }
    return found;
}

static void handle_keystone_text(const short *text) {
    char b[128] = {};
    for(int i=0;i<sizeof(b)-1;i++) {
        switch(text[i]) {
            case 0: break;
            default: b[i] = text[i];
        }
    }
    console_out(b);
}

bool keystone_enabled = false;

void setup_keystone_override() noexcept {
    if(!custom_keystone_in_use()) return;
    keystone_enabled = true;
    auto &on_keystone_message_sig = get_signature("on_keystone_message_sig");
    write_code_any_value(on_keystone_message_sig.address(), static_cast<unsigned char>(0xE9));
    write_code_any_value(on_keystone_message_sig.address() + 1, reinterpret_cast<int>(handle_keystone_text) - reinterpret_cast<int>(on_keystone_message_sig.address() + 5));
}
