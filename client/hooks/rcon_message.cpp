#include "rcon_message.h"

#include <string.h>

#include "../client_signature.h"
#include "../messaging/messaging.h"
#include "../../math/data_types.h"

static BasicCodecave rcon_message_code;

static std::vector<Event<rcon_message_event>> events;

void add_rcon_message_event(rcon_message_event event_function, EventPriority priority) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) return;
    }
    events.emplace_back(event_function, priority);
}

void remove_rcon_message_event(rcon_message_event event_function) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) {
            events.erase(events.begin() + i);
            return;
        }
    }
}

static void on_rprint(uint32_t a, uint32_t b, uint32_t c, char **d, uint32_t e) {
    char *text = *(d + 1);
    size_t len = strlen(text);

    bool allow = true;
    call_in_order_allow(events, allow, text);
    if(!allow) return;

    extern bool block_server_messages_active;
    extern LARGE_INTEGER last_time_rcon_was_used;
    if(block_server_messages_active && counter_time_elapsed(last_time_rcon_was_used) > 5) {
        return;
    }

    if(len > 4 && text[len - 4] == '|' && text[len - 3] == 'n' && text[len - 2] == 'c') {
        char color = text[len - 1];
        if(color > 0x60) color -= 0x20;
        float a(1.0), r(0.0), g(0.0), b(0.0);
        switch(color) {
            case '0': {
                break;
            }
            case '1': {
                b = 0.7;
                break;
            }
            case '2': {
                g = 0.7;
                break;
            }
            case '3': {
                g = 0.7;
                b = 0.7;
                break;
            }
            case '4': {
                r = 0.7;
                break;
            }
            case '5': {
                r = 0.7;
                b = 0.7;
                break;
            }
            case '6': {
                r = 0.7;
                g = 0.7;
                break;
            }
            case '7': {
                r = 0.7;
                g = 0.7;
                b = 0.7;
                break;
            }
            case '8': {
                r = 0.5;
                g = 0.5;
                b = 0.5;
                break;
            }
            case '9': {
                r = 0.2;
                g = 0.2;
                b = 1.0;
                break;
            }
            case 'A': {
                r = 0.2;
                g = 1.0;
                b = 0.2;
                break;
            }
            case 'B': {
                r = 0.2;
                g = 1.0;
                b = 1.0;
                break;
            }
            case 'C': {
                r = 1.0;
                g = 0.2;
                b = 0.2;
                break;
            }
            case 'D': {
                r = 1.0;
                g = 0.2;
                b = 1.0;
                break;
            }
            case 'E': {
                r = 1.0;
                g = 1.0;
                b = 0.2;
                break;
            }
            default: break;
        }
        console_out(text, ColorARGB(a,r,g,b));
        allow = false;
    }
    else if(len > 6 && text[len - 6] == '|' && text[len - 5] == 'n' && text[len - 4] == 'c') {
        ColorARGB ncolor;
        long p = strtoul(text + (len - 3), NULL, 16);
        ncolor.red = ((p >> 8) & 0xF) / 15.0;
        ncolor.green = ((p >> 4) & 0xF) / 15.0;
        ncolor.blue = (p & 0xF) / 15.0;
        console_out(text, ncolor);
        allow = false;
    }
    else if(len > 7 && text[len - 7] == '|' && text[len - 6] == 'n' && text[len - 5] == 'c') {
        ColorARGB ncolor;
        long p = strtoul(text + (len - 4), NULL, 16);
        ncolor.alpha = ((p >> 12) & 0xF) / 15.0;
        ncolor.red = ((p >> 8) & 0xF) / 15.0;
        ncolor.green = ((p >> 4) & 0xF) / 15.0;
        ncolor.blue = (p & 0xF) / 15.0;
        console_out(text, ncolor);
        allow = false;
    }
    else if(len > 9 && text[len - 9] == '|' && text[len - 8] == 'n' && text[len - 7] == 'c') {
        ColorARGB ncolor;
        long p = strtoul(text + (len - 6), NULL, 16);
        ncolor.red = ((p >> 16) & 0xFF) / 255.0;
        ncolor.green = ((p >> 8) & 0xFF) / 255.0;
        ncolor.blue = (p & 0xFF) / 255.0;
        console_out(text, ncolor);
        allow = false;
    }
    else if(len > 11 && text[len - 11] == '|' && text[len - 10] == 'n' && text[len - 9] == 'c') {
        ColorARGB ncolor;
        long p = strtoul(text + (len - 8), NULL, 16);
        ncolor.alpha = ((p >> 24) & 0xFF) / 255.0;
        ncolor.red = ((p >> 16) & 0xFF) / 255.0;
        ncolor.green = ((p >> 8) & 0xFF) / 255.0;
        ncolor.blue = (p & 0xFF) / 255.0;
        console_out(text, ncolor);
        allow = false;
    }

    if(allow) {
        console_out(text, ColorARGB(1.0,1.0,1.0,1.0));
    }
}

void initialize_rcon_message() noexcept {
    write_jmp_call(get_signature("on_rcon_message_sig").address(), reinterpret_cast<void *>(on_rprint), nullptr, rcon_message_code);
    memset(rcon_message_code.data + 7, 0x90, 5);
}
