#pragma once

#include "../hooks/event.h"
#include "../halo_data/hud.h"

#define OFFSETTER_INDEX_NULL 0
typedef size_t OffsetterIndex;

/// Create an offsetter and return its index.
OffsetterIndex create_offsetter(short x, short y, bool text) noexcept;

/// Delete an offsetter. No operation is done if 0 is passed. An exception will occur if an invalid index is passed.
void destroy_offsetter(OffsetterIndex index);
