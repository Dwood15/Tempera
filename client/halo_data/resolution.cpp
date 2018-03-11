#include "resolution.h"

#include <stdlib.h>
#include "../messaging/messaging.h"
#include "../client_signature.h"


struct ChangeResolutionStructA {
    uint32_t width;
    uint32_t height;
    uint32_t refresh_rate;
    uint32_t unknown2 = 0;
};

struct ChangeResolutionStructB {
    uint32_t width;
    uint32_t height;
    uint32_t unknown1 = 16;
    uint32_t unknown2 = 1;

    uint32_t unknown3 = 0;
    uint32_t unknown4 = 0;
    uint32_t unknown5 = 3;
    uint32_t unknown6 = 0;

    uint32_t windowed = 1;
    uint32_t no_acid_trip_plz = 1;
    uint32_t unknown7 = 0x4B;
    uint32_t unknown8 = 0;

    uint32_t unknown9 = 0;
    /// 0x80000000 = no vsync; 0 = vsync
    uint32_t no_vsync = 0x80000000;
};

static_assert(sizeof(ChangeResolutionStructB) == 0x38);

Resolution &get_resolution() noexcept {
    static Resolution *resolution = *reinterpret_cast<Resolution **>(get_signature("resolution_sig").address() + 4);
    return *resolution;
}

static void set_resolution(int width, int height, int refresh_rate=1000, int vsync=-1, int windowed=-1) {
    static bool repeating = false;
    auto *change_resolution_query = get_signature("change_resolution_query_sig").address();
    auto *change_resolution = get_signature("change_resolution_sig").address();
    auto *change_window_size = get_signature("change_window_size_sig").address();

    bool repeat = false;

    ChangeResolutionStructA z;
    z.height = height;
    z.width = width;
    z.refresh_rate = refresh_rate;
    ChangeResolutionStructB r;

#ifdef __GNUC__
    asm (
        "pushad;"
        "push %0;"
        "mov eax, %1;"
        "call %2;"
        "add esp, 4;"
        "popad;"
        :
        : "r" (&r), "r" (&z), "m" (change_resolution_query)
    );
    if(vsync >= 0) r.no_vsync = vsync ? 0 : 0x80000000;
    repeat = r.windowed == 0 || windowed == 0;
    if(windowed >= 0) r.windowed = windowed;
    asm (
        "pushad;"
        "push %0;"
        "call %1;"
        "add esp, 4;"
        "popad;"
        :
        : "r" (&r), "m" (change_resolution)
    );
    asm (
        "pushad;"
        "mov eax, %0;"
        "mov ecx, %1;"
        "mov esi, 1;"
        "call %2;"
        "popad;"
        :
        : "m" (height), "m" (width), "m" (change_window_size)
    );
#endif

    auto *block_resolution_change = get_signature("block_auto_resolution_change_sig").address();
    const short nope[] = {0x90, 0x90, 0x90, 0x90, 0x90};
    write_code(block_resolution_change, nope, 5);

    if(repeat && !repeating) {
        repeating = true;
        set_resolution(width, height, refresh_rate, vsync, 1);
        set_resolution(width, height, refresh_rate, vsync, 0);
        repeating = false;
    }

}

/// Function for command chimera_set_resolution
ChimeraCommandError block_vsync_command(size_t argc, const char **argv) noexcept {
    static bool active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value) {
            auto &resolution = get_resolution();
            set_resolution(resolution.width, resolution.height, 1000, 0);
        }
        active = new_value;
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}

/// Function for command chimera_set_resolution
ChimeraCommandError set_resolution_command(size_t argc, const char **argv) noexcept {
    int width = strtol(argv[0], nullptr, 10);
    int height = strtol(argv[1], nullptr, 10);
    if(width < 32 && height < 32 && width > 0 && height > 0) {
        auto &resolution = get_resolution();
        auto width_f = strtof(argv[0], nullptr);
        auto height_f = strtof(argv[1], nullptr);
        height = (int)(resolution.width / width_f * height_f);
        width = resolution.width;
        if(width < 640) {
            height = (int)(640.0f / width_f * height_f);
            width = 640;
        }
        if(height < 480) {
            height = 480;
            width = (int)(480.0 * width_f / height_f);
        }
    }
    if(width < 640 || height < 480) {
        console_out_error("A resolution of at least 640 pixels wide by 480 pixels high is required.");
        return CHIMERA_COMMAND_ERROR_FAILURE;
    }
    int refresh_rate = 1000;
    if(argc >= 3) refresh_rate = strtol(argv[2], nullptr, 10);
    int vsync = -1;
    if(argc >= 4) vsync = bool_value(argv[3]);
    int windowed = -1;
    if(argc >= 5) windowed = bool_value(argv[4]);
    set_resolution(width, height, refresh_rate, vsync, windowed);
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
