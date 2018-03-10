#include "map_load.h"
#include <vector>
#include <stdint.h>

#include "tick.h"
#include "../client_signature.h"

static std::vector<Event<event_no_args>> events;

static void initialize_map_load() noexcept;
static bool map_load_initialized = false;

void add_map_load_event(event_no_args event_function, EventPriority priority) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) return;
    }
    if(!map_load_initialized) initialize_map_load();
    events.emplace_back(event_function, priority);
}

void remove_map_load_event(event_no_args event_function) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) {
            events.erase(events.begin() + i);
            return;
        }
    }
}

void on_map_load() noexcept {
    call_in_order(events);
}

static void initialize_map_load() noexcept {
    map_load_initialized = true;
    static BasicCodecave on_map_load_bytecode;
    write_jmp_call(get_signature("on_map_load_sig").address(), nullptr, reinterpret_cast<void *>(on_map_load), on_map_load_bytecode);
}
