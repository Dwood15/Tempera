#include "chat.h"

#include "../client_signature.h"
#include "../command/console.h"
#include "../halo_data/server.h"
#include "../halo_data/table.h"
#include "../messaging/messaging.h"

void chat_out(const char *message, uint32_t channel) noexcept {
	if (server_type() == SERVER_NONE) return;
	auto chat_out = reinterpret_cast<uintptr_t>(get_signature("chat_out_sig").address());
	size_t x = strlen(message);
#define BLEN 256
	if (x > BLEN) {
		return;
	}
	auto *unicode = new short[BLEN + 1]();
	auto *unicode2 = new short[BLEN + 1]();

	for (unsigned int i = 0; i < x; i++) {
		unicode[i] = (short) message[i];
		unicode2[i] = (short) message[i];
	}

#ifdef __GNUC__
	asm (
		"pushad;"
		"mov eax, %0;"
		"push ebx;"
		"mov edx, %1;"
		"mov esi, %2;"
		"push esp;"
		"call %3;"
		"add esp, 8;"
		"popad;"
		:
		: "r" (channel), "r" (unicode), "r" (unicode2), "r" (chat_out)
	);
#elif _MSC_VER
	__asm {
	pushad
	mov eax, channel
	push ebx
	mov edx, unicode
	mov esi, unicode2
	push esp
	call chat_out
	add esp, 8
	popad
	}
#endif

	delete[] unicode;
	delete[] unicode2;
}

bool player_can_use_vehicle_chat() noexcept {
	BaseHaloObject *object_data = reinterpret_cast<BaseHaloObject *>(HaloObject(HaloPlayer().object_id()).object_data());
	if (!object_data) return false;
	return HaloObject(object_data->parent_object_id).object_data() != nullptr;
}

ChimeraCommandError chat_command(size_t argc, const char **argv) noexcept {
	if (argc == 1) {
		console_is_out(false);
		return CHIMERA_COMMAND_ERROR_SUCCESS;
	}
	auto method = std::string(argv[0]);
	int channel;
	if (method == "all") channel = 0;
	else if (method == "team") channel = 1;
	else if (method == "vehicle") channel = player_can_use_vehicle_chat() ? 2 : 1;
	else {
		console_out_error("Expected all, team, or vehicle for argument #1.");
		return CHIMERA_COMMAND_ERROR_FAILURE;
	}
	auto message = std::string(argv[1]);
	for (size_t i = 2; i < argc; i++) {
		message = message + " " + argv[i];
	}
	chat_out(message.data(), channel);
	console_is_out(false);
	return CHIMERA_COMMAND_ERROR_SUCCESS;
}
