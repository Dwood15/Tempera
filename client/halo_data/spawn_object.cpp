#include "spawn_object.h"
#include "server.h"
#include "../client_signature.h"
#include "../halo_data/table.h"

struct s_object_creation_disposition {
    uint32_t tag_id;
    uint32_t unknown0; // can fuck up the shit apparently
    uint32_t player_ident;
    uint32_t parent;
    char unknown1[4];
    char unknown2[4];
    float pos[3];
};

uint32_t spawn_object(unsigned int &tag_id, float x, float y, float z, uint32_t parent) noexcept {
	static auto *querydo = get_signature("create_object_query_sig").address() - 6;
	static auto *doit = get_signature("create_object_sig").address() - 24;
	char query[1024] = {};

#ifdef __GNUC__
	asm (
	"pushad;"
			"push %0;"
			"push %1;"
			"lea eax, dword ptr ds:[%2];"
			"call %3;"
			"add esp, 8;"
			"popad;"
	:
	: "r" (parent), "r" (tag_id.id), "r" (query), "r" (querydo)
	);
#elif _MSC_VER
	__asm {
		pushad
		push parent
		push tag_id
		lea eax, dword ptr ds:[query]
		call querydo
		add esp, 8
		popad
	}
#endif
	auto *sq = reinterpret_cast<s_object_creation_disposition *>(query);
	sq->pos[0] = x;
	sq->pos[1] = y;
	sq->pos[2] = z;
	sq->player_ident = 0xFFFFFFFF;
	uint32_t flag; // 0 = created by local machine; 1 = host; 2 = ???; 3 = client sided object (from giraffe)
	if (server_type() == SERVER_LOCAL) flag = 0;
	else flag = 3;
	uint32_t obj_id_ptr = 0xFFFFFFFF;

#ifdef __GNUC__
	asm(
	"pushad;"
			"push %3;"
			"lea eax, dword ptr ds:[%1];"
			"push eax;"
			"call %2;"
			"add esp, 8;"
			"mov %0, eax;"
			"popad;"
	: "=m" (obj_id_ptr)
	: "r" (query), "r" (doit), "r" (flag)
	);
#elif _MSC_VER
	__asm{
			pushad
			push flag
			lea eax, dword ptr ds:[query]
			push eax
			call doit
			add esp, 8
			mov obj_id_ptr, eax
			popad
	}
#endif

	return obj_id_ptr;
}

bool delete_object(uint32_t object_id) noexcept {
	static auto *delete_object_fn = get_signature("delete_object_sig").address() - 10;
	if (HaloObject(object_id).object_data()) {

#ifdef __GNUC__
		asm (
		"pushad;"
				"mov eax, %0;"
				"call %1;"
				"popad;"
		:
		: "r" (object_id), "r" (delete_object_fn)
		);
#elif _MSC_VER
		__asm {
		pushad
		mov eax, object_id
		call delete_object_fn
		popad
	}
#endif
		return true;
	}
	return false;
}
