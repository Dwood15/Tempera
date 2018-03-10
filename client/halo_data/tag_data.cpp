#include <exception>
#include <string.h>
#include "tag_data.h"
#include "tiarace/hce_tag_class_int.h"

bool HaloTagID::is_null() const noexcept {
    return this->id == 0xFFFFFFFF;
}

bool HaloTagID::is_valid() const noexcept {
    return !this->is_null() && this->index < *reinterpret_cast<uint32_t *>(0x4044000C);
}

HaloTagID::HaloTagID(const int &new_id) noexcept {
    this->id = new_id;
}

HaloTagID::HaloTagID(const unsigned int &new_id) noexcept {
    this->id = new_id;
}

HaloTagID::operator int() const noexcept {
    return this->id;
}

HaloTagID::operator unsigned int() const noexcept {
    return this->id;
}

HaloTag &HaloTag::from_id(const HaloTagID &tag_id) noexcept {
    return (*reinterpret_cast<HaloTag **>(0x40440000))[tag_id.index];
}

HaloTag &HaloTag::lookup(const char *tag_class, const char *tag_path) {
    uint32_t tag_class_int;
    if(strlen(tag_class) > 4) {
        tag_class_int = HaloCE::tag_class_int_from_string(tag_class);
    }
    else {
        char tag_class_buf[5] = {};
        bool spaces = false;
        for(int i=0;i<4;i++) {
            if(spaces || tag_class[i] == 0x0) {
                tag_class_buf[3 - i] = 0x20;
                spaces = true;
            }
            else {
                tag_class_buf[3 - i] = tag_class[i];
            }
        }
        tag_class_int = *reinterpret_cast<uint32_t *>(tag_class_buf);
    }

    for(size_t i=0;i<*reinterpret_cast<uint32_t *>(0x4044000C);i++) {
        auto &tag = HaloTag::from_id(i);
        if(tag.tag_class == tag_class_int && strcmp(tag.path, tag_path) == 0) {
            return tag;
        }
    }
    throw std::exception();
}
