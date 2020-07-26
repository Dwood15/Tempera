#pragma once

#include <macros_generic.h>

namespace Yelo::AI {
	struct s_ai_communication_event : TStructImpl(16) {};
	typedef s_ai_communication_event ai_communication_dialogue_events_t[105];
	typedef s_ai_communication_event ai_communication_reply_events_t[46];

	struct s_ai_conversation_datum : TStructImpl(100) {};
	typedef Memory::DataArray<s_ai_conversation_datum, 8>
		ai_conversation_data_t;

	struct s_ai_communication_packet {
		long  : 32;
		short : 16; // 4
		Enums::ai_communication_type dialogue_type_index; // 6
		short          : 16; // 8
		unsigned short : 16; // A ?

		short          : 16; // C
		unsigned short : 16; // E ?
		unsigned long  : 32; // 10 ?
		short          : 16; // 14
		unsigned short : 16; // 16 ?
		short          : 16; // 18
		short          : 16; // 1A
		bool broken; // 1C false = reformed
		unsigned char  : 8;
		unsigned short : 16;
	}; static_assert(sizeof(s_ai_communication_packet) == 0x20);
};
