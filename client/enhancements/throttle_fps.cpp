#include "throttle_fps.h"
#include "../client_signature.h"
#include "../hooks/frame.h"
#include "../messaging/messaging.h"

static LARGE_INTEGER prev_time;
static double throttle_fps = 0.0;

static void after_frame() noexcept {
    auto seconds_per_frame = 1.0 / throttle_fps;
    do {
        LARGE_INTEGER now_time;
        QueryPerformanceCounter(&now_time);
        auto r = counter_time_elapsed(prev_time, now_time);
        if(r >= seconds_per_frame) {
            prev_time = now_time;
            break;
        }
        DWORD zzz = 1000.0 * (seconds_per_frame - r);
        if(zzz > 0) zzz--;
        Sleep(zzz);
    }
    while(true);
}

ChimeraCommandError throttle_fps_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        auto new_value = atof(argv[0]);
        if(new_value != throttle_fps) {
            if(new_value <= 5.0) {
                if(new_value != 0.0) {
                    console_out_error("Invalid framerate.");
                    return CHIMERA_COMMAND_ERROR_FAILURE;
                }
                new_value = 0.0;
            }

            if(new_value == 0.0) {
                remove_frame_event(after_frame);
            }
            else {
                add_frame_event(after_frame, EVENT_PRIORITY_FINAL);
                QueryPerformanceCounter(&prev_time);
            }
            throttle_fps = new_value;
        }
    }
    if(throttle_fps == 0.0) {
        console_out("off");
    }
    else {
        console_out(std::to_string(throttle_fps));
    }
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
