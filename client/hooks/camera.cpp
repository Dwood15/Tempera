#include "camera.h"

#include "../client_signature.h"

static void initialize_camera() noexcept;
static bool camera_initialized = false;

static std::vector<Event<event_no_args>> preevents;

void add_precamera_event(event_no_args event_function, EventPriority priority) noexcept {
    for(size_t i=0;i<preevents.size();i++) {
        if(preevents[i].function == event_function) return;
    }
    if(!camera_initialized) initialize_camera();
    preevents.emplace_back(event_function, priority);
}
void remove_precamera_event(event_no_args event_function) noexcept {
    for(size_t i=0;i<preevents.size();i++) {
        if(preevents[i].function == event_function) {
            preevents.erase(preevents.begin() + i);
            return;
        }
    }
}

static void on_precamera() {
    call_in_order(preevents);
}

static std::vector<Event<event_no_args>> events;

void add_camera_event(event_no_args event_function, EventPriority priority) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) return;
    }
    if(!camera_initialized) initialize_camera();
    events.emplace_back(event_function, priority);
}
void remove_camera_event(event_no_args event_function) noexcept {
    for(size_t i=0;i<events.size();i++) {
        if(events[i].function == event_function) {
            events.erase(events.begin() + i);
            return;
        }
    }
}

static void on_camera() {
    call_in_order(events);
}


static void initialize_camera() noexcept {
    camera_initialized = true;
    static BasicCodecave camera_override_code;
    write_jmp_call(get_signature("on_camera_sig").address(), reinterpret_cast<void *>(on_precamera), reinterpret_cast<void *>(on_camera), camera_override_code);
}
