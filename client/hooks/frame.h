#pragma once

#include "event.h"

void add_preframe_event(event_no_args event_function, EventPriority priority = EVENT_PRIORITY_DEFAULT) noexcept;
void remove_preframe_event(event_no_args event_function) noexcept;
void add_frame_event(event_no_args event_function, EventPriority priority = EVENT_PRIORITY_DEFAULT) noexcept;
void remove_frame_event(event_no_args event_function) noexcept;
