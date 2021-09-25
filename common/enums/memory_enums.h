#pragma once

#include <macros_generic.h>

namespace Yelo {
	namespace Enums {
		enum x86_opcode : byte {
			// push imm16\32
				_x86_opcode_push_imm = 0x68,

			// jz short imm8
				_x86_opcode_jz_short  = 0x74,
			// jnz short imm8
				_x86_opcode_jnz_short = 0x75,
			// jge short imm8
				_x86_opcode_jge_short = 0x7D,

			_x86_opcode_nop = 0x90,

			// mov [e]ax, [offset]
				_x86_opcode_mov_eax_y_offset = 0xA1,

			// mov [offset], [e]ax
				_x86_opcode_mov_offset_y_eax = 0xA3,

			// return near imm16\32
				_x86_opcode_retn      = 0xC2,
			// regular return near
				_x86_opcode_ret       = 0xC3,
			// call near imm16\32
				_x86_opcode_call_near = 0xE8,
			// jmp near imm16\32
				_x86_opcode_jmp_near  = 0xE9,

			// jmp short imm8
				_x86_opcode_jmp_short = 0xEB,
		};
		enum x86_opcode_twobyte : unsigned __int16 {
			// call ds:[address]
				_x86_opcode_call_abs = 0x15FF, /*FF 15*/
			// call ds:[address]
				_x86_opcode_jmp_abs  = 0x25FF, /*FF 25*/
		};

		STAT_ASSERT(Enums::x86_opcode_twobyte, 2);
	};

	namespace Memory {
#pragma pack(push, 1)
		struct Opcode {
			struct s_call {
				byte         Op;
				unsigned int Address;
			};
			STAT_ASSERT(s_call, 0x5);

			struct s_call_ret : s_call {
				byte   Ret;
				ushort Count;
			};
			STAT_ASSERT(s_call_ret, (sizeof(s_call) + 0x3));

			// absolute call opcode
			struct s_call_abs {
				ushort Op;
				ushort Address;
			};
			STAT_ASSERT(s_call_abs, 0x4);

			// absolute call opcode with return
			struct s_call_abs_ret : s_call_abs {
				byte   Ret;
				ushort Count;
			};
			STAT_ASSERT(s_call_abs_ret, (sizeof(s_call_abs) + 0x3));
		};
#pragma pack(pop)
	};
};
