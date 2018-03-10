#include "map.h"

#include <Windows.h>
#include "../client_signature.h"
#include "tag_data.h"
#include "../hooks/tick.h"
#include "../messaging/messaging.h"

static uint32_t boffset;
static uint32_t soffset;

MapHeader &get_map_header() noexcept {
    static auto *map_header = *reinterpret_cast<MapHeader **>(get_signature("map_header_sig").address() + 2);
    return *map_header;
}

MapIndex *map_indices() noexcept {
    static auto **all_map_indices = *reinterpret_cast<MapIndex ***>(get_signature("map_index_sig").address() + 2);
    return *all_map_indices;
}

uint32_t maps_count() noexcept {
    static auto *map_count = *reinterpret_cast<uint32_t **>(get_signature("map_index_sig").address() + 2) + 1;
    return *map_count;
}

static void on_load() noexcept {
    if(get_map_header().engine_type == 7) {
        HaloTag *tags = *reinterpret_cast<HaloTag **>(0x40440000);
        for(size_t i=0;i<*reinterpret_cast<uint32_t *>(0x4044000C);i++) {
            if(tags[i].tag_class == 0x6269746D) {
                auto *bitmaps = *reinterpret_cast<char **>(tags[i].data + 0x60 + 4);
                for(uint32_t b=0;b<*reinterpret_cast<uint32_t *>(tags[i].data + 0x60);b++) {
                    auto *bitmap = bitmaps + b * 0x30;
                    if((bitmap[0xF] & 1) == 0) continue;
                    *reinterpret_cast<uint32_t *>(bitmap + 0x18) += boffset;
                }
            }
            else if(tags[i].tag_class == 0x736E6421) {
                auto *pitch_ranges = *reinterpret_cast<char **>(tags[i].data + 0x98 + 4);
                for(uint32_t r=0;r<*reinterpret_cast<uint32_t *>(tags[i].data + 0x98);r++) {
                    auto *range = pitch_ranges + r * 0x48;
                    auto *permutations = *reinterpret_cast<char **>(range + 0x3C + 4);
                    for(uint32_t p=0;p<*reinterpret_cast<uint32_t *>(range + 0x3C);p++) {
                        auto *permutation = permutations + p * 0x7C;
                        if((permutation[0x44] & 1) == 0) continue;
                        *reinterpret_cast<uint32_t *>(permutation + 0x48) += soffset;
                    }
                }
            }
        }
    }
}

const char *bitmaps_path = "chimera\\c_bitmaps";
const char *sounds_path = "chimera\\c_sounds";

uint32_t open_or_create(const char *chimera_path, const char *pc_path, const char *ce_path, bool create_if_not_exists) {
    FILE *b = fopen(chimera_path, "r");
    if(b) {
        fseek(b, -4, SEEK_END);
        uint32_t offset;
        fread(&offset, sizeof(boffset), 1, b);
        fclose(b);
        return offset;
    }
    else if(create_if_not_exists) {
        FILE *sce = fopen(ce_path, "rb");
        if(sce) {
            FILE *spc = fopen(pc_path, "rb");
            if(spc) {
                FILE *sch = fopen(chimera_path, "wb");
                if(sch) {
                    fseek(sce, 0, SEEK_END);
                    fseek(spc, 0, SEEK_END);
                    size_t sce_size = ftell(sce);
                    size_t spc_size = ftell(spc);
                    fseek(sce, 0, SEEK_SET);
                    fseek(spc, 0, SEEK_SET);
                    size_t buff_size = sce_size + spc_size + sizeof(uint32_t);
                    char *buffer = new char[buff_size];
                    fread(buffer, sce_size, 1, sce);
                    fread(buffer + sce_size, spc_size, 1, spc);
                    *reinterpret_cast<uint32_t *>(buffer + sce_size + spc_size) = sce_size;
                    fwrite(buffer, buff_size, 1, sch);
                    delete[] buffer;
                    fclose(sce);
                    fclose(spc);
                    fclose(sch);
                    DeleteFile(pc_path);
                    return sce_size;
                }
                fclose(spc);
            }
            fclose(sce);
        }
    }
    return 0;
}

void setup_pc_map_compatibility() noexcept {
    boffset = open_or_create("maps\\chimera\\c_bitmaps.map", "maps\\chimera\\bitmaps.map", "maps\\bitmaps.map", true);
    soffset = open_or_create("maps\\chimera\\c_sounds.map", "maps\\chimera\\sounds.map", "maps\\sounds.map", true);

    if(boffset && soffset) {
        write_code_any_value(get_signature("compare_ce_sig").address() + 7, static_cast<unsigned short>(0x9090));
        write_code_any_value(get_signature("compare_ce_load_sig").address() + 7, static_cast<unsigned char>(0xEB));
        unsigned char nop[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
        write_code_c(get_signature("pc_map_compatibility_sig").address(), nop);

        unsigned char mod[] = {
            0x89, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF,
            0x60,
            0xE8, 0x00, 0x00, 0x00, 0x00,
            0x61,
            0xE9, 0x00, 0x00, 0x00, 0x00
        };
        static BasicCodecave on_map_load(mod, sizeof(mod));
        auto &on_read_sig = get_signature("on_read_sig");
        auto *on_read_addr = on_read_sig.address();
        write_code_any_value(on_read_addr, static_cast<unsigned char>(0xE9));
        write_code_any_value(on_read_addr + 1, reinterpret_cast<int>(on_map_load.data) - reinterpret_cast<int>(on_read_addr + 5));
        write_code_any_value(on_read_addr + 5, static_cast<unsigned char>(0x90));
        write_code_any_value(on_map_load.data + 2, *reinterpret_cast<int **>(on_read_sig.signature() + 2));
        write_code_any_value(on_map_load.data + 7 + 1, reinterpret_cast<int>(on_load) - reinterpret_cast<int>(on_map_load.data + 7 + 5));
        write_code_any_value(on_map_load.data + 13 + 1, reinterpret_cast<int>(on_read_addr + 5) - reinterpret_cast<int>(on_map_load.data + 13 + 5));

        write_code_any_value(get_signature("bitmaps_string_sig").address() + 1, bitmaps_path);
        write_code_any_value(get_signature("sounds_string_sig").address() + 1, sounds_path);
    }
}
