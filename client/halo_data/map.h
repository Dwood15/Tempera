#pragma once

#include <stdint.h>

enum MapGameType : uint16_t {
    MAP_SINGLE_PLAYER = 0,
    MAP_MULTIPLAYER,
    MAP_USER_INTERFACE
};

struct MapHeader {
    /// Must be equal to 0x68656164
    uint32_t head = 0x68656164;
    /// 7 if retail; 0x261 if Custom Edition; 5 if Xbox
    uint32_t engine_type;
    /// Halo ignores map files that are >384 MiB
    uint32_t file_size;
    char padding1[4];
    /// File offset to tag data (which is loaded at 0x40440000)
    uint32_t tag_data_offset;
    /// File size of tag data.
    uint32_t tag_data_size;
    char padding2[8];
    /// File name of map excluding extension; typically matches scenario tag name, but not required
    char name[32];
    /// Unused on PC version of Halo
    char build[32];
    MapGameType game_type;
    char padding3[2];
    /// Calculated with CRC32 of BSPs, models, and tag data
    uint32_t crc32_unused;
    char padding4[0x794];
    /// Must be equal to 0x666F6F74
    uint32_t foot = 0x666F6F74;
};

MapHeader &get_map_header() noexcept;


struct MapIndex {
    const char *file_name;
    uint32_t loaded;
    uint32_t unknown;
    uint32_t crc32;
};

MapIndex *map_indices() noexcept;
uint32_t maps_count() noexcept;
void setup_pc_map_compatibility() noexcept;
