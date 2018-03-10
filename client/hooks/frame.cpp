#include "frame.h"
#include <vector>
#include <stdint.h>

#include "../client_signature.h"

static void initialize_frame() noexcept;
static bool frame_initialized = false;

static std::vector<Event<event_no_args>> preevents;

void add_preframe_event(event_no_args event_function, EventPriority priority) noexcept {
    for(size_t i=0;i<preevents.size();i++) {
        if(preevents[i].function == event_function) return;
    }
    if(!frame_initialized) initialize_frame();
    preevents.emplace_back(event_function, priority);
}
void remove_preframe_event(event_no_args event_function) noexcept {
    for(size_t i=0;i<preevents.size();i++) {
        if(preevents[i].function == event_function) {
            preevents.erase(preevents.begin() + i);
            return;
        }
    }
}

static void on_preframe() {
    call_in_order(preevents);
}

static std::vector<Event<event_no_args>> events;

void add_frame_event(event_no_args event_function, EventPriority priority) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) return;
    }
    if(!frame_initialized) initialize_frame();
    events.emplace_back(event_function, priority);
}
void remove_frame_event(event_no_args event_function) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) {
            events.erase(events.begin() + i);
            return;
        }
    }
}

static void on_frame() {
    call_in_order(events);
}

static void initialize_frame() noexcept {
    frame_initialized = true;
    static BasicCodecave render_code;
    write_jmp_call(get_signature("on_frame_sig").address(), reinterpret_cast<void *>(on_preframe), reinterpret_cast<void *>(on_frame), render_code);
}
