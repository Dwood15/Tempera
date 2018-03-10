#include "script.h"

#include "../client_signature.h"

void execute_script(const char *script) noexcept {
    static auto *execute_script_address = get_signature("execute_script_sig").address();
#ifdef __GNUC__
    asm (
        "pushad;"
        "push %0;"
        "call %1;"
        "add esp, 4;"
        "popad;"
        :
        : "r" (script), "r" (execute_script_address)
    );
#elif _MSC_VER
	__asm {
	    pushad
        push script
        call execute_script_address
        add esp, 4
        popad
	}
#endif
}
