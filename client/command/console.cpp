#include "console.h"
#include "../messaging/messaging.h"
#include "../client_signature.h"
#include "../halo_data/chat.h"
#include "../hooks/frame.h"
#include "../halo_data/table.h"

char *console_text = NULL;

static void block_error() noexcept {
    auto *push_req = get_signature("console_block_error_sig").address();
    DWORD old_protect = 0;
    DWORD old_protect_b = 0;
    if(VirtualProtect(push_req, 5, PAGE_READWRITE, &old_protect) == false) return;
    for(int i=0;i<5;i++) *(push_req + 6 + i) = 0x90;
    VirtualProtect(push_req, 5, old_protect, &old_protect_b);
}

static void unblock_error() noexcept {
    get_signature("console_block_error_sig").undo();
}

LARGE_INTEGER last_time_rcon_was_used;

static void read_command() noexcept {
    block_error();
    if(strlen(console_text) > 127) {
        unblock_error();
        return;
    }
    switch(execute_chimera_command(console_text, false, true)) {
        case CHIMERA_COMMAND_ERROR_NOT_ENOUGH_ARGUMENTS: {
            auto &f = find_chimera_command(console_text);
            char text[256] = {};
            sprintf(text,"Error: Function %s takes at least %u argument%s.", f.name(), f.min_args(), f.min_args() == 1 ? "" : "s");
            console_out_error(text);
            break;
        }
        case CHIMERA_COMMAND_ERROR_TOO_MANY_ARGUMENTS: {
            auto &f = find_chimera_command(console_text);
            char text[256] = {};
            sprintf(text,"Error: Function %s takes no more than %u argument%s.", f.name(), f.max_args(), f.max_args() == 1 ? "" : "s");
            console_out_error(text);
            break;
        }
        case CHIMERA_COMMAND_ERROR_UNSUPPORTED: {
            auto &f = find_chimera_command(console_text);
            char text[256] = {};
            sprintf(text,"Error: Function %s is unsupported on your Halo client.", f.name());
            console_out_error(text);
            break;
        }
        case CHIMERA_COMMAND_ERROR_COMMAND_NOT_FOUND: {
            extern bool on_command_lua(const char *command);
            auto command = split_arguments(console_text, true);
            bool rcon = command[0] == "rcon";
            if(rcon) {
                QueryPerformanceCounter(&last_time_rcon_was_used);
            }
            if(!rcon && !on_command_lua(console_text)) {
                console_text[0] = 0;
            }
            else {
                unblock_error();
            }
            break;
        }
        default: {}
    }
}

void initialize_console() noexcept {
    auto &console_call_s = get_signature("console_call_sig");
    auto *console_ptr = console_call_s.address();
    console_text = I8PTR(*reinterpret_cast<uint32_t *>(console_ptr - 4));
    static BasicCodecave console_codecave;
    write_jmp_call(console_call_s.address(), reinterpret_cast<void *>(read_command), nullptr, console_codecave);
}

bool console_is_out(int change, const char *with_text) noexcept {
    if(change != -1) {
        reinterpret_cast<void (*)(int out)>(get_signature("toggle_console_sig").address())(change ? 1 : 2);
        if(change == 1 && with_text) {
            auto len = strlen(with_text);
            memcpy(console_text, with_text, len + 1);
            *reinterpret_cast<short *>(console_text + 0x106) = len;
        }
    }
    static auto *out = *reinterpret_cast<char **>(get_signature("console_is_out_sig").address() + 2);
    return *out;
}

struct ConsoleEntry {
    uint32_t idents[3];
    char zero2;
    char text[0x103];
    ColorARGB color;
    int32_t frames_out_fade; // goes up to 151
};

static void on_console() {
    static GenericTable *&table = **reinterpret_cast<GenericTable ***>(get_signature("console_text_table_sig").address() + 2);
    static LARGE_INTEGER last_frame = {};
    if(last_frame.QuadPart == 0) QueryPerformanceCounter(&last_frame);
    LARGE_INTEGER now_frame;
    QueryPerformanceCounter(&now_frame);
    auto time_since = counter_time_elapsed(last_frame, now_frame);
    last_frame = now_frame;
    ConsoleEntry *entries = reinterpret_cast<ConsoleEntry *>(table->first);
    for(size_t i=0;i<table->size;i++) {
        if(console_is_out()) {
            entries[i].color.alpha += time_since * 10;
            if(entries[i].color.alpha > 1.0) entries[i].color.alpha = 1.0;
        }
        else {
            if(entries[i].frames_out_fade < 0 || entries[i].color.alpha < 0.075) {
                entries[i].color.alpha -= time_since * 10;
                entries[i].frames_out_fade = -100;
                if(entries[i].color.alpha < 0.0) entries[i].color.alpha = 0.0;
            }
            else {
                entries[i].color.alpha -= time_since / 6.0;
                if(entries[i].color.alpha < 0.80) {
                    entries[i].color.alpha -= time_since / 3.0;
                }
                if(entries[i].color.alpha < 0.0) {
                    entries[i].color.alpha = 0.0;
                }
            }
        }
    }
}

/// Fix the console text
void setup_console_text_fix() noexcept {
    auto &fade = get_signature("console_fade_sig");
    const unsigned char nop[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    write_code_c(fade.address(), nop);
    add_frame_event(on_console);
}

bool already_set = false;

ChimeraCommandError enable_console_command(size_t argc, const char **argv) noexcept {
    static bool active = true;
    if(argc == 1) {
        auto new_value = bool_value(argv[0]);
        if(!already_set && new_value != active) {
            if(console_is_out()) {
                console_is_out(false);
            }
            **reinterpret_cast<char **>(get_signature("enable_console_sig").address() + 1) = new_value;
        }
        active = new_value;
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
