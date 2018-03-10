#include "server_ip.h"

#include "../messaging/messaging.h"
#include "../client_signature.h"
#include "../hooks/frame.h"

extern char *console_text;
static char new_text[1024];

static void read_buffer() noexcept {
    strcpy(new_text, console_text);
    const char *connect = "connect";
    for(size_t c=0;c<4;c++) {
        char *beginning = new_text + c;
        if(strncmp(beginning, connect, strlen(connect)) == 0) {
            for(size_t i=strlen(connect); i<sizeof(new_text) - 1 - c;i++) {
                if(beginning[i] == 0) break;
                if(beginning[i] != ' ') beginning[i] = '*';
            }
            return;
        }
    }
}

ChimeraCommandError block_server_ip_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            auto &join_server_ip_text_sig = get_signature("join_server_ip_text_sig");
            auto &f1_ip_text_render_call_sig = get_signature("f1_ip_text_render_call_sig");
            auto &create_server_ip_text_sig = get_signature("create_server_ip_text_sig");
            auto &console_buffer_text_show_sig = get_signature("console_buffer_text_show_sig");
            if(new_value) {
                const unsigned char mod[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
                write_code_c(join_server_ip_text_sig.address() + 5, mod);
                write_code_c(f1_ip_text_render_call_sig.address(), mod);

                const short mod_create_server_ip[] = { 0xB9, 0x00, 0x00, 0x00, 0x00, 0x90, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x66, 0xB9, 0x00, 0x00, 0x90, 0x90, 0x90 };
                write_code_s(create_server_ip_text_sig.address(), mod_create_server_ip);

                write_code_any_value(console_buffer_text_show_sig.address(), static_cast<unsigned char>(0xB8));
                write_code_any_value(console_buffer_text_show_sig.address() + 1, new_text);
                write_code_any_value(console_buffer_text_show_sig.address() + 5, static_cast<unsigned char>(0x90));
                add_preframe_event(read_buffer);

                memset(new_text, 0, sizeof(new_text));
            }
            else {
                join_server_ip_text_sig.undo();
                f1_ip_text_render_call_sig.undo();
                create_server_ip_text_sig.undo();
                console_buffer_text_show_sig.undo();
                remove_preframe_event(read_buffer);
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
