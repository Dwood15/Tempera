#include "open_sauce.h"
#include "windows.h"

static bool check_dinput8() noexcept {
    auto *module = GetModuleHandle("dinput8.dll");
    if(!module) return false;
    for(int i=0;i<0x2000;i++) {
        if(memcmp(reinterpret_cast<char *>(module) + i, ".yelo", 5) == 0) return true;
    }
    return false;
}

bool open_sauce_present() noexcept {
    static bool is_present = check_dinput8();
    return is_present;
}
