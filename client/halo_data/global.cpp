#include "global.h"

#include <stdint.h>
#include <string.h>
#include "tag_data.h"
#include "../client_signature.h"

ScriptingGlobal read_global(const char *global_name) noexcept {
    static auto *haddr = get_signature("hs_globals_sig").address();
    ScriptingGlobal sg;
    if(haddr) {
        auto *hs_globals = *reinterpret_cast<char **>(haddr + 7);
        auto &first_global = *reinterpret_cast<uint32_t *>(haddr + 1);
        auto *hs_globals_table = *reinterpret_cast<char **>(hs_globals);
        auto *hs_table = *reinterpret_cast<char **>(hs_globals_table + 0x34);

        auto *scenario_tag = HaloTag::from_id(*reinterpret_cast<HaloTagID *>(0x40440004)).data;
        auto &globals_count = *reinterpret_cast<uint32_t *>(scenario_tag + 0x4A8);
        auto *globals_address = *reinterpret_cast<char **>(scenario_tag + 0x4A8 + 4);
        for(uint32_t i=0;i<globals_count;i++) {
            auto *global = globals_address + i * 92;
            if(strcmp(global,global_name) == 0) {
                auto &global_type = *reinterpret_cast<uint16_t *>(global + 0x20);
                auto *location = hs_table + (i + first_global) * 8;
                switch(global_type) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 9:
                    case 10:
                        sg.type = SCRIPTING_GLOBAL_UNIMPLEMENTED;
                        break;
                    case 5:
                        sg.type = SCRIPTING_GLOBAL_BOOLEAN;
                        break;
                    case 6:
                        sg.type = SCRIPTING_GLOBAL_REAL;
                        break;
                    case 7:
                        sg.type = SCRIPTING_GLOBAL_SHORT;
                        break;
                    case 8:
                        sg.type = SCRIPTING_GLOBAL_LONG;
                        break;
                    default:
                        sg.type = SCRIPTING_GLOBAL_LONG;
                        break;
                }
                sg.value.long_int = *reinterpret_cast<long *>(location + 4);
            }
        }
    }
    return sg;
}

bool set_global(const char *global_name, ScriptingGlobalValue value) noexcept {
    static auto *haddr = get_signature("hs_globals_sig").address();
    if(haddr) {
        auto *hs_globals = *reinterpret_cast<char **>(haddr + 7);
        auto &first_global = *reinterpret_cast<uint32_t *>(haddr + 1);
        auto *hs_globals_table = *reinterpret_cast<char **>(hs_globals);
        auto *hs_table = *reinterpret_cast<char **>(hs_globals_table + 0x34);

        auto *scenario_tag = HaloTag::from_id(*reinterpret_cast<HaloTagID *>(0x40440004)).data;
        auto &globals_count = *reinterpret_cast<uint32_t *>(scenario_tag + 0x4A8);
        auto *globals_address = *reinterpret_cast<char **>(scenario_tag + 0x4A8 + 4);
        for(uint32_t i=0;i<globals_count;i++) {
            auto *global = globals_address + i * 92;
            if(strcmp(global,global_name) == 0) {
                auto &global_type = *reinterpret_cast<uint16_t *>(global + 0x20);
                auto *location = hs_table + (i + first_global) * 8;
                switch(global_type) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 9:
                    case 10:
                        return false;
                        break;
                    case 5:
                        *reinterpret_cast<char *>(location + 4) = value.boolean ? 1 : 0;
                        break;
                    case 6:
                        *reinterpret_cast<float *>(location + 4) = value.real;
                        break;
                    case 7:
                        *reinterpret_cast<short *>(location + 4) = value.short_int;
                        break;
                    default:
                        *reinterpret_cast<long *>(location + 4) = value.long_int;
                        break;
                }
                return true;
            }
        }
    }
    return false;
}
