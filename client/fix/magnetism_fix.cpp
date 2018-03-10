#include <math.h>
#include "magnetism_fix.h"
#include "../client_signature.h"
#include "../halo_data/table.h"
#include "../messaging/messaging.h"

bool gamepad_plugged_in() noexcept {
    static char *joybutt = *reinterpret_cast<char **>(get_signature("joybutt_sig").address() + 2);
    for(int i=0;i<8;i++) {
        if(*reinterpret_cast<uint32_t *>(joybutt + 0x240 * i) != 0xFFFFFFFF) {
            return true;
        }
    }
    return false;
}

static bool gamepad_being_used = false;

static void enable_magnetism_fix() noexcept {
    const short magnetism_mod[] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
    write_code_s(get_signature("magnetism_sig").address(),magnetism_mod);
}

static void disable_magnetism_fix() noexcept {
    get_signature("magnetism_sig").undo();
}

static float deadzone = 0.15;

static void on_gamepad_movement_horizontal() noexcept {
    auto &movement_info = get_movement_info();
    float &m = movement_info.aim_horizontal;
    float &a = movement_info.custom_look_horizontal;
    float &b = movement_info.custom_look_vertical;
    float &c = movement_info.aim_vertical;
    if(fabs(a) >= deadzone || fabs(b) > deadzone || fabs(c) > deadzone) {
        m = a;
        if(gamepad_being_used != 1) enable_magnetism_fix();
        gamepad_being_used = 1;
    }
    else m = 0.0;
}

static void on_gamepad_movement_vertical() noexcept {
    auto &movement_info = get_movement_info();
    float &m = movement_info.aim_vertical;
    float &a = movement_info.custom_look_horizontal;
    float &b = movement_info.custom_look_vertical;
    float &c = movement_info.aim_horizontal;
    if(fabs(a) >= deadzone || fabs(b) > deadzone || fabs(c) > deadzone) {
        m = b;
        if(gamepad_being_used != 1) enable_magnetism_fix();
        gamepad_being_used = 1;
    }
    else m = 0.0;
}

static void on_mouse_movement_horizontal() noexcept {
    gamepad_being_used = 0;
    disable_magnetism_fix();
}

static bool enabled_before = false;

void fix_magnetism() noexcept {
    gamepad_being_used = gamepad_plugged_in();
    if(!gamepad_being_used) return;
    enabled_before = **reinterpret_cast<char **>(get_signature("player_magnetism_enabled_sig").address() + 1) == 1;
    enable_magnetism_fix();

    const unsigned char fstp_then_call[] {
        // fstp dword ptr - 0x0
        0xD9, 0x1D, 0x00, 0x00, 0x00, 0x00,

        // pushad - 0x6
        0x60,

        // call my shit - 0x7
        0xE8, 0x00, 0x00, 0x00, 0x00,

        // popad - 0xC
        0x61,

        // ret - 0xD
        0xC3
    };

    static BasicCodecave on_gamepad_horizontal_code(fstp_then_call, sizeof(fstp_then_call));
    static BasicCodecave on_gamepad_vertical_code(fstp_then_call, sizeof(fstp_then_call));
    static BasicCodecave on_mouse_horizontal_code(fstp_then_call, sizeof(fstp_then_call));

    **reinterpret_cast<char **>(get_signature("player_magnetism_enabled_sig").address() + 1) = 1;

    DWORD old_protect = 0;
    DWORD old_protect_b = 0;

    auto *on_gamepad_horizontal_addr1 = get_signature("gamepad_horizontal_0_sig").address();
    auto *on_gamepad_vertical_addr1 = get_signature("gamepad_vertical_0_sig").address();
    float *fah = reinterpret_cast<float *>(*reinterpret_cast<uint32_t *>(on_gamepad_horizontal_addr1 + 2)) + 4;
    *reinterpret_cast<float **>(on_gamepad_horizontal_code.data + 2) = fah;

    float *fav = reinterpret_cast<float *>(*reinterpret_cast<uint32_t *>(on_gamepad_vertical_addr1 + 2)) + 4;
    *reinterpret_cast<float **>(on_gamepad_vertical_code.data + 2) = fav;

    auto *on_mouse_horizontal_addr1 = get_signature("mouse_horizontal_0_sig").address();
    *reinterpret_cast<float **>(on_mouse_horizontal_code.data + 2) = reinterpret_cast<float *>(*reinterpret_cast<uint32_t *>(on_mouse_horizontal_addr1 + 2));

    *I32PTR(on_gamepad_horizontal_code.data + 7 + 1) = I32(on_gamepad_movement_horizontal) - I32(on_gamepad_horizontal_code.data + 7 + 5);
    *I32PTR(on_gamepad_vertical_code.data + 7 + 1) = I32(on_gamepad_movement_vertical) - I32(on_gamepad_vertical_code.data + 7 + 5);

    VirtualProtect(on_gamepad_horizontal_addr1, 6, PAGE_READWRITE, &old_protect);
    memset(on_gamepad_horizontal_addr1,0x90,6);
    on_gamepad_horizontal_addr1[0] = 0xE8;
    *I32PTR(on_gamepad_horizontal_addr1 + 1) = I32(on_gamepad_horizontal_code.data) - I32(on_gamepad_horizontal_addr1 + 1 + 4);
    VirtualProtect(on_gamepad_horizontal_addr1, 6, old_protect, &old_protect_b);

    auto *on_gamepad_horizontal_addr2 = get_signature("gamepad_horizontal_1_sig").address();
    VirtualProtect(on_gamepad_horizontal_addr2, 6, PAGE_READWRITE, &old_protect);
    memset(on_gamepad_horizontal_addr2,0x90,6);
    on_gamepad_horizontal_addr2[0] = 0xE8;
    *I32PTR(on_gamepad_horizontal_addr2 + 1) = I32(on_gamepad_horizontal_code.data) - I32(on_gamepad_horizontal_addr2 + 1 + 4);
    VirtualProtect(on_gamepad_horizontal_addr2, 6, old_protect, &old_protect_b);

    VirtualProtect(on_gamepad_vertical_addr1, 6, PAGE_READWRITE, &old_protect);
    memset(on_gamepad_vertical_addr1,0x90,6);
    on_gamepad_vertical_addr1[0] = 0xE8;
    *I32PTR(on_gamepad_vertical_addr1 + 1) = I32(on_gamepad_vertical_code.data) - I32(on_gamepad_vertical_addr1 + 1 + 4);
    VirtualProtect(on_gamepad_vertical_addr1, 6, old_protect, &old_protect_b);

    auto *on_gamepad_vertical_addr2 = get_signature("gamepad_vertical_1_sig").address();
    VirtualProtect(on_gamepad_vertical_addr2, 6, PAGE_READWRITE, &old_protect);
    memset(on_gamepad_vertical_addr2,0x90,6);
    on_gamepad_vertical_addr2[0] = 0xE8;
    *I32PTR(on_gamepad_vertical_addr2 + 1) = I32(on_gamepad_vertical_code.data) - I32(on_gamepad_vertical_addr2 + 1 + 4);
    VirtualProtect(on_gamepad_vertical_addr2, 6, old_protect, &old_protect_b);

    *I32PTR(on_mouse_horizontal_code.data + 7 + 1) = I32(on_mouse_movement_horizontal) - I32(on_mouse_horizontal_code.data + 7 + 5);

    VirtualProtect(on_mouse_horizontal_addr1, 6, PAGE_READWRITE, &old_protect);
    memset(on_mouse_horizontal_addr1,0x90,6);
    on_mouse_horizontal_addr1[0] = 0xE8;
    *I32PTR(on_mouse_horizontal_addr1 + 1) = I32(on_mouse_horizontal_code.data) - I32(on_mouse_horizontal_addr1 + 1 + 4);
    VirtualProtect(on_mouse_horizontal_addr1, 6, old_protect, &old_protect_b);

    auto *on_mouse_horizontal_addr2 = get_signature("mouse_horizontal_1_sig").address();
    VirtualProtect(on_mouse_horizontal_addr2, 6, PAGE_READWRITE, &old_protect);
    memset(on_mouse_horizontal_addr2,0x90,6);
    on_mouse_horizontal_addr2[0] = 0xE8;
    *I32PTR(on_mouse_horizontal_addr2 + 1) = I32(on_mouse_horizontal_code.data) - I32(on_mouse_horizontal_addr2 + 1 + 4);
    VirtualProtect(on_mouse_horizontal_addr2, 6, old_protect, &old_protect_b);
}


ChimeraCommandError aim_assist_command(size_t argc, const char **argv) noexcept {
    static bool active = true;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            if(new_value) {
                fix_magnetism();
            }
            else {
                **reinterpret_cast<char **>(get_signature("player_magnetism_enabled_sig").address() + 1) = enabled_before ? 1 : 0;
                get_signature("gamepad_horizontal_0_sig").undo();
                get_signature("gamepad_vertical_0_sig").undo();
                get_signature("mouse_horizontal_0_sig").undo();
                get_signature("gamepad_horizontal_1_sig").undo();
                get_signature("gamepad_vertical_1_sig").undo();
                get_signature("mouse_horizontal_1_sig").undo();
                get_signature("magnetism_sig").undo();
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
