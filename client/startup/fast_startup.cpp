#include "fast_startup.h"

#include "../halo_data/map.h"
#include "../messaging/messaging.h"
#include "../client_signature.h"
#include "../hac2.h"
#include "../open_sauce.h"
#include "../path.h"
#include "../settings.h"
#include "../halo_data/tag_data.h"

#include "crc32.h"

struct CacheEntry {
    char name[64] = {};
    uint32_t crc32;
};
std::vector<CacheEntry> cache;
static bool use_cache = false;
static bool modded_stock_maps = false;

static bool same_string_case_insensitive(const char *a, const char *b) {
    if(a == b) return true;
    while(tolower(*a) == tolower(*b)) {
        if(*a == 0) return true;
        a++;
        b++;
    }
    return false;
}

static bool save_cache() noexcept {
    char path[MAX_PATH] = {};
    sprintf(path, "%s\\chimera\\cache.bin", halo_path());
    FILE *f = fopen(path, "wb");
    if(f) {
        fwrite(&cache[0], sizeof(cache[0])*cache.size(), 1, f);
        fclose(f);
    }
    return f != NULL;
}

static bool load_cache() noexcept {
    char path[MAX_PATH] = {};
    sprintf(path, "%s\\chimera\\cache.bin", halo_path());
    FILE *f = fopen(path, "rb");
    if(f) {
        fseek(f, 0, SEEK_END);
        size_t cache_items = ftell(f) / sizeof(CacheEntry);
        fseek(f, 0, SEEK_SET);
        cache.clear();
        for(size_t i=0;i<cache_items;i++) {
            CacheEntry ce;
            fread(&ce, sizeof(ce), 1, f);
            cache.push_back(ce);
        }
        fclose(f);
    }
    return f != NULL;
}

// CRC of map = CRC of BSPs, model data, and tag data
static uint32_t calculate_crc32_of_map_file(FILE *f) noexcept {
    uint32_t crc = 0;
    MapHeader header;
    fread(&header, sizeof(header), 1, f);

    auto *tag_data = new char[header.tag_data_size];
    fseek(f, header.tag_data_offset, SEEK_SET);
    fread(tag_data, header.tag_data_size, 1, f);

    auto *scenario_tag = tag_data + (*reinterpret_cast<uint32_t *>(tag_data) - 0x40440000) + (*reinterpret_cast<uint32_t *>(tag_data + 4) & 0xFFFF) * 0x20;
    auto *scenario_tag_data = tag_data + (*reinterpret_cast<uint32_t *>(scenario_tag + 0x14) - 0x40440000);

    // First, the BSP(s)
    auto &structure_bsp_count = *reinterpret_cast<uint32_t *>(scenario_tag_data + 0x5A4);
    auto *structure_bsps = tag_data + (*reinterpret_cast<uint32_t *>(scenario_tag_data + 0x5A4 + 4) - 0x40440000);
    for(size_t b=0;b<structure_bsp_count;b++) {
        char *bsp = structure_bsps + b * 0x20;
        auto &bsp_offset = *reinterpret_cast<uint32_t *>(bsp);
        auto &bsp_size = *reinterpret_cast<uint32_t *>(bsp + 4);

        char *bsp_data = new char[bsp_size];
        fseek(f, bsp_offset, SEEK_SET);
        fread(bsp_data, bsp_size, 1, f);
        crc = crc32(crc, bsp_data, bsp_size);
        delete[] bsp_data;
    }

    // Next, model data
    auto &model_vertices_offset = *reinterpret_cast<uint32_t *>(tag_data + 0x14);
    auto &vertices_size = *reinterpret_cast<uint32_t *>(tag_data + 0x20);

    auto *model_vertices = new char[vertices_size];
    fseek(f, model_vertices_offset, SEEK_SET);
    fread(model_vertices, vertices_size, 1, f);
    crc = crc32(crc, model_vertices, vertices_size);
    delete[] model_vertices;

    // Lastly, tag data
    crc = crc32(crc, tag_data, header.tag_data_size);
    delete[] tag_data;

    return crc;
}

static uint32_t stock_crc32(const std::string &name) {
    if(name == "beavercreek")
        return 0x07B3876A;
    else if(name == "sidewinder")
        return 0xBD95CF55;
    else if(name == "damnation")
        return 0x0FBA059D;
    else if(name == "ratrace")
        return 0xF7F8E14C;
    else if(name == "prisoner")
        return 0x43B81A8B;
    else if(name == "hangemhigh")
        return 0xA7C8B9C6;
    else if(name == "chillout")
        return 0x93C53C27;
    else if(name == "carousel")
        return 0x9C301A08;
    else if(name == "boardingaction")
        return 0xF4DEEF94;
    else if(name == "bloodgulch")
        return 0x7B309554;
    else if(name == "wizard")
        return 0xCF3359B1;
    else if(name == "putput")
        return 0xAF2F0B84;
    else if(name == "longest")
        return 0xC8F48FF6;
    else if(name == "dangercanyon")
        return 0xC410CD74;
    else if(name == "deathisland")
        return 0x1DF8C97F;
    else if(name == "gephyrophobia")
        return 0xD2872165;
    else if(name == "infinity")
        return 0x0E7F7FE7;
    else if(name == "timberland")
        return 0x54446470;
    else if(name == "icefields")
        return 0x5EC1DEB7;
    else
        return 0xFFFFFFFF;
}

static void do_crc_things() noexcept {
    static char *loading_map = *reinterpret_cast<char **>(get_signature("loading_map_sig").address() + 1);
    auto *indices = map_indices();
    for(size_t i=0;i<maps_count();i++) {
        if(same_string_case_insensitive(indices[i].file_name, loading_map)) {
            if(indices[i].crc32 == 0xFFFFFFFF && modded_stock_maps) {
                indices[i].crc32 = stock_crc32(indices[i].file_name);
            }

            if(indices[i].crc32 == 0xFFFFFFFF && use_cache) {
                for(size_t c=0;c<cache.size();c++) {
                    if(same_string_case_insensitive(indices[i].file_name, cache[c].name)) {
                        indices[i].crc32 = cache[c].crc32;
                    }
                }
            }

            if(indices[i].crc32 == 0xFFFFFFFF) {
                char map_path[MAX_PATH] = {};
                sprintf(map_path, "maps\\%s.map", indices[i].file_name);
                FILE *f = fopen(map_path, "rb");
                if(!f && open_sauce_present()) {
                    sprintf(map_path, "maps\\%s.yelo", indices[i].file_name);
                    f = fopen(map_path, "rb");
                }
                if(!f && hac2_present()) {
                    sprintf(map_path, "%s\\hac\\maps\\%s.map", halo_path(), indices[i].file_name);
                    f = fopen(map_path, "rb");
                }
                if(f) {
                    indices[i].crc32 = ~calculate_crc32_of_map_file(f);
                    fclose(f);
                    if(use_cache) {
                        CacheEntry ce;
                        strcpy(ce.name, indices[i].file_name);
                        ce.crc32 = indices[i].crc32;
                        cache.push_back(ce);
                        if(!save_cache()) {
                            console_out_error("Error: Unable to save to cache.");
                        }
                    }
                }
            }

            if(indices[i].crc32 != 0xFFFFFFFF) break;
        }
    }
}

void setup_fast_startup() {
    auto &fast_startup_sig = get_signature("crc32_call_sig");
    auto &get_crc_sig = get_signature("get_crc_sig");
    static unsigned char nop5[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
    write_code_c(fast_startup_sig.address(), nop5);

    unsigned char code[] = {
        // shl eax, 0x04
        0xC1, 0xE0, 0x04,

        // pushad
        0x60,

        // call do_crc_things
        0xE8, 0xFF, 0xFF, 0xFF, 0xFF,

        // popad
        0x61,

        // mov ecx, [eax+edx+0C]
        0x8B, 0x4C, 0x10, 0x0C,

        // jmp back
        0xE9, 0xFF, 0xFF, 0xFF, 0xFF
    };

    static BasicCodecave on_get_crc(code, sizeof(code));
    static unsigned char nop7[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    write_code_c(get_crc_sig.address(), nop7);
    write_code_any_value(get_crc_sig.address(), static_cast<unsigned char>(0xE9));
    write_code_any_value(get_crc_sig.address() + 1, reinterpret_cast<int>(on_get_crc.data) - reinterpret_cast<int>(get_crc_sig.address() + 5));
    write_code_any_value(on_get_crc.data + 4 + 1, reinterpret_cast<int>(do_crc_things) - reinterpret_cast<int>(on_get_crc.data + 4 + 5));
    write_code_any_value(on_get_crc.data + 0xE + 1, reinterpret_cast<int>(get_crc_sig.address() + 5) - reinterpret_cast<int>(on_get_crc.data + 0xE + 5));
}

/// Function for command chimera_cache
ChimeraCommandError cache_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        use_cache = bool_value(argv[0]);
        load_cache();
    }
    console_out(use_cache ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}

/// Function for command chimera_cache_clear
ChimeraCommandError cache_clear_command(size_t argc, const char **argv) noexcept {
    console_out("Erasing cache...");
    cache.clear();
    auto *indices = map_indices();
    for(size_t i=0;i<maps_count();i++) {
        indices[i].crc32 = 0xFFFFFFFF;
    }
    if(!save_cache()) {
        console_out_error("Error: Unable to save to cache.");
    }
    else {
        console_out("Done!");
    }
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}

ChimeraCommandError modded_stock_maps_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        modded_stock_maps = bool_value(argv[0]);
    }
    console_out(modded_stock_maps ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
