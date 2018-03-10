#pragma once

#include <stdint.h>

union HaloTagID {
    uint16_t index;
    uint32_t id;

    bool is_null() const noexcept;
    bool is_valid() const noexcept;
    HaloTagID(const int &new_id) noexcept;
    HaloTagID(const unsigned int &new_id) noexcept;
    operator int() const noexcept;
    operator unsigned int() const noexcept;
};

struct HaloTagDependency {
    uint32_t tag_class;
    char *tag_path;
    uint32_t unknown;
    HaloTagID tag_id;
};

struct HaloTag {
    uint32_t tag_class;
    uint32_t tag_class_secondary;
    uint32_t tag_class_tertiary;
    HaloTagID id;
    const char *path;
    char *data;
    char dont_care[0x8];

    static HaloTag &from_id(const HaloTagID &tag_id) noexcept;
    static HaloTag &lookup(const char *tag_class, const char *tag_path);
};
