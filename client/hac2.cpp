#include "hac2.h"
#include "windows.h"

bool hac2_present() noexcept {
    static bool hac2_present = GetModuleHandle("hac.dll") != nullptr;
    return hac2_present;
}
