#pragma once

#include "event.h"

typedef bool (*rcon_message_event)(const char *);

void add_rcon_message_event(rcon_message_event event_function, EventPriority priority = EVENT_PRIORITY_DEFAULT) noexcept;
void remove_rcon_message_event(rcon_message_event event_function) noexcept;
void initialize_rcon_message() noexcept;
