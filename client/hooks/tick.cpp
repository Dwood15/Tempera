#include "../client_signature.h"
#include "../command/command.h"
#include "../messaging/messaging.h"
#include "../halo_data/tag_data.h"
#include "tick.h"

static LARGE_INTEGER current_tick_time;

static void initialize_tick() noexcept;
static bool tick_initialized = false;

static std::vector<Event<event_no_args>> preevents;

bool first_tick = false;

void add_pretick_event(event_no_args event_function, EventPriority priority) noexcept {
    for(size_t i=0;i<preevents.size();i++) {
        if(preevents[i].function == event_function) return;
    }

    if(!tick_initialized) initialize_tick();
    preevents.emplace_back(event_function, priority);
}
void remove_pretick_event(event_no_args event_function) noexcept {
    for(size_t i=0;i<preevents.size();i++) {
        if(preevents[i].function == event_function) {
            preevents.erase(preevents.begin() + i);
            return;
        }
    }
}

static void on_pretick() noexcept {
    first_tick = true;
    call_in_order(preevents);
}

static std::vector<Event<event_no_args>> events;

void add_tick_event(event_no_args event_function, EventPriority priority) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) return;
    }
    if(!tick_initialized) initialize_tick();
    events.emplace_back(event_function, priority);
}

void remove_tick_event(event_no_args event_function) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) {
            events.erase(events.begin() + i);
            return;
        }
    }
}

static void on_tick() noexcept {
    QueryPerformanceCounter(&current_tick_time);
    call_in_order(events);
}

static void initialize_tick() noexcept {
    tick_initialized = true;
    static BasicCodecave on_tick_bytecode;
    write_jmp_call(get_signature("on_tick_sig").address(), reinterpret_cast<void *>(on_pretick), reinterpret_cast<void *>(on_tick), on_tick_bytecode);
}

int32_t tick_count() noexcept {
    static auto *tick_counter = reinterpret_cast<int32_t *>(**reinterpret_cast<char ***>(get_signature("tick_counter_sig").address() + 1) + 0xC);
    return *tick_counter;
}

const float &tick_rate() noexcept {
    static auto *trs = *reinterpret_cast<float **>(get_signature("tick_rate_sig").address() + 2);
    return *trs;
}

void set_tick_rate(float new_rate) noexcept {
    static auto &trs = *const_cast<float *>(&tick_rate());
    DWORD prota, protb;
    VirtualProtect(&trs, sizeof(trs), PAGE_READWRITE, &prota);
    trs = new_rate;
    VirtualProtect(&trs, sizeof(trs), prota, &protb);
    return;
}

float effective_tick_rate() noexcept {
    static auto *address = get_signature("game_speed_sig").address();
    return *reinterpret_cast<float *>(**reinterpret_cast<char ***>(address + 1) + 0x18) * tick_rate();
}

double tick_time() noexcept {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter_time_elapsed(current_tick_time, counter);
}

double tick_progress() noexcept {
    auto current_time = tick_time() * effective_tick_rate();
    if(current_time < 1.0) return current_time;
    else return 1.0;
}

ChimeraCommandError tps_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        auto new_rate = atof(argv[0]);
        if(new_rate < 0.01) {
            console_out_error("Tick rate cannot be less than 0.01.");
            return CHIMERA_COMMAND_ERROR_FAILURE;
        }
        set_tick_rate(new_rate);
    }
    console_out(std::to_string(tick_rate()));
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
