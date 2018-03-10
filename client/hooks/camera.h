#pragma once

#include "event.h"

void add_precamera_event(event_no_args event_function, EventPriority priority = EVENT_PRIORITY_DEFAULT) noexcept;
void remove_precamera_event(event_no_args event_function) noexcept;
void add_camera_event(event_no_args event_function, EventPriority priority = EVENT_PRIORITY_DEFAULT) noexcept;
void remove_camera_event(event_no_args event_function) noexcept;
