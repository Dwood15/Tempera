#pragma once

#include "tag_data.h"

uint32_t spawn_object(const HaloTagID &tag_id, float x, float y, float z, uint32_t parent=0xFFFFFFFF) noexcept;
bool delete_object(uint32_t object_id) noexcept;
