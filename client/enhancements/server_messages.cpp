#include "server_messages.h"

bool block_server_messages_active = false;

static void unblock_next_message() noexcept {
    auto &server_messages_3_sig = get_signature("server_messages_3_sig");
    server_messages_3_sig.undo();
}

static void block_next_message() noexcept {
    auto &server_messages_3_sig = get_signature("server_messages_3_sig");
    write_code_any_value(server_messages_3_sig.address() + 1, reinterpret_cast<int>(unblock_next_message) - reinterpret_cast<int>(server_messages_3_sig.address() + 5));
}

/// Function for command chimera_block_server_messages
ChimeraCommandError block_server_messages_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != block_server_messages_active) {
            auto &server_messages_1_sig = get_signature("server_messages_1_sig");
            auto &server_messages_2_sig = get_signature("server_messages_2_sig");
            auto &server_messages_3_sig = get_signature("server_messages_3_sig");
            if(new_value) {
                const short nope[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
                write_code_s(server_messages_1_sig.address(), nope);

                const unsigned char data[] = {
                    // jle return
                    0x7E, 0x0A,

                    // call block_next_message
                    0xE8, 0xFF, 0xFF, 0xFF, 0xFF,

                    // jmp somewhere
                    0xE9, 0xFF, 0xFF, 0xFF, 0xFF,

                    // return
                    // jmp back
                    0xE9, 0xFF, 0xFF, 0xFF, 0xFF
                };

                static BasicCodecave codecave(data, sizeof(data));

                write_code_any_value(server_messages_2_sig.address(), static_cast<unsigned char>(0xE9));
                write_code_any_value(server_messages_2_sig.address() + 1, reinterpret_cast<int>(codecave.data) - reinterpret_cast<int>(server_messages_2_sig.address() + 5));
                write_code_any_value(server_messages_2_sig.address() + 5, static_cast<unsigned char>(0x90));

                write_code_any_value(codecave.data + 2 + 1, reinterpret_cast<int>(block_next_message) - reinterpret_cast<int>(codecave.data + 2 + 5));
                write_code_any_value(codecave.data + 7 + 1, *reinterpret_cast<int *>(server_messages_2_sig.signature() + 2) + reinterpret_cast<int>(server_messages_2_sig.address() + 6) - reinterpret_cast<int>(codecave.data + 7 + 5));
                write_code_any_value(codecave.data + 12 + 1, reinterpret_cast<int>(server_messages_2_sig.address() + 5) - reinterpret_cast<int>(codecave.data + 12 + 5));
            }
            else {
                server_messages_1_sig.undo();
                server_messages_2_sig.undo();
                server_messages_3_sig.undo();
            }
            block_server_messages_active = new_value;
        }
    }
    console_out(block_server_messages_active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
