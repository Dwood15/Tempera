#include "table.h"
#include "../client_signature.h"

MovementInfo &get_movement_info() noexcept {
    static auto *movement_info_address = reinterpret_cast<MovementInfo *>(*reinterpret_cast<char **>(get_signature("movement_info_sig").address() + 2) - 0x20);
    return *movement_info_address;
}

GenericTable &get_object_table() noexcept {
    static auto *object_table = **reinterpret_cast<GenericTable ***>(get_signature("object_table_sig").address() + 2);
    return *object_table;
}

GenericTable &get_player_table() noexcept {
    static auto *player_table = **reinterpret_cast<GenericTable ***>(get_signature("player_table_sig").address() + 1);
    return *player_table;
}

GenericTable &get_particle_table() noexcept {
    static auto *particle_table = **reinterpret_cast<GenericTable ***>(get_signature("particle_table_sig").address() + 2);
    return *particle_table;
}

GenericTable &get_antenna_table() noexcept {
    static auto *antenna_table = **reinterpret_cast<GenericTable ***>(get_signature("antenna_table_sig").address() + 2);
    return *antenna_table;
}

GenericTable &get_flag_table() noexcept {
    static auto *flag_table = **reinterpret_cast<GenericTable ***>(get_signature("flag_table_sig").address() + 2);
    return *flag_table;
}

GenericTable &get_light_table() noexcept {
    static auto *light_table = **reinterpret_cast<GenericTable ***>(get_signature("light_table_sig").address() + 2);
    return *light_table;
}

GenericTable &get_effect_table() noexcept {
    static auto *effect_table = **reinterpret_cast<GenericTable ***>(get_signature("effect_table_sig").address() + 1);
    return *effect_table;
}

GenericTable &get_decal_table() noexcept {
    static auto *effect_table = **reinterpret_cast<GenericTable ***>(get_signature("decal_table_sig").address() + 1);
    return *effect_table;
}

uint32_t client_player_index() noexcept {
    static auto *player_id = (**reinterpret_cast<uint32_t ***>(get_signature("player_id_sig").address() + 2) + 1);
    return *player_id;
}

bool unichar_equal(const short *a, const short *b, size_t size) noexcept {
    if(a == b) return true;
    if(a == nullptr || b == nullptr) return false;
    size_t i = 0;
    while(i++ < size && *(a++) == *(b++)) {
        if(*a == 0) return true;
    }
    return false;
}

bool unichar_equal(const char *a, const short *b, size_t size) noexcept {
    if(reinterpret_cast<const short *>(a) == b) return true;
    if(a == nullptr || b == nullptr) return false;
    size_t i = 0;
    while(i++ < size && *(a++) == *(b++)) {
        if(*a == 0) return true;
    }
    return false;
}

uint32_t HaloPlayer::index() noexcept {
    return this->player_index;
}

uint32_t HaloPlayer::object_id() noexcept {
    if(this->player_data() == nullptr) return NULL_ID;
    return *reinterpret_cast<uint32_t *>(this->player_data() + 0x34);
}

char *HaloPlayer::player_data() noexcept {
    GenericTable &pt = get_player_table();
    if(this->player_index >= pt.size || this->player_index >= pt.max_count) return nullptr;
    auto *data = reinterpret_cast<char *>(pt.first) + this->player_index * pt.index_size;
    if(*reinterpret_cast<uint16_t *>(data) == 0xFFFF) return nullptr;
    return data;
}

HaloPlayer::HaloPlayer(uint32_t player_index) noexcept {
    this->player_index = player_index & 0xFFFF;
}

HaloPlayer::HaloPlayer(const short *name) noexcept {
    GenericTable &pt = get_player_table();
    auto *p = reinterpret_cast<char *>(pt.first);
    for(size_t i=0;i<pt.size;i++) {
        auto *pl = p + i * pt.index_size;
        if(*reinterpret_cast<uint16_t *>(pl) == 0xFFFF) continue;
        if(unichar_equal(name,(const short *)(pl + 4), HALO_NAME_LENGTH)) {
            this->player_index = i;
            return;
        }
    }
    this->player_index = 0xFFFF;
}

HaloPlayer::HaloPlayer(const char *name) noexcept {
    GenericTable &pt = get_player_table();
    auto *p = reinterpret_cast<char *>(pt.first);
    for(size_t i=0;i<pt.size;i++) {
        auto *pl = p + i * pt.index_size;
        if(*reinterpret_cast<uint16_t *>(pl) == 0xFFFF) continue;
        if(unichar_equal(name,(const short *)(pl + 4), HALO_NAME_LENGTH)) {
            this->player_index = i;
            return;
        }
    }
    this->player_index = 0xFFFF;
}

uint32_t HaloObject::index() noexcept {
    return this->object_index;
}

char *HaloObject::object_data() noexcept {
    auto &ot = get_object_table();
    if((this->object_index & 0xFFFF) > ot.max_count) return nullptr;
    return *reinterpret_cast<char **>(reinterpret_cast<char *>(ot.first) + (this->object_index & 0xFFFF) * ot.index_size + 0x8);
}

HaloObject::HaloObject(uint32_t object_index) noexcept {
    this->object_index = object_index;
}

CameraType get_camera_type() noexcept {
    static auto *cta = get_signature("camera_type_sig").address();
    if(cta) return static_cast<CameraType>(*reinterpret_cast<short *>(*reinterpret_cast<char **>(cta + 0x2) + 0x56));
    else return CAMERA_FIRST_PERSON;
}
