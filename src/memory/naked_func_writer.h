#pragma once

// Uncomment, or define this before the header, if the debug build isn't incremental
//#define NAKED_FUNC_WRITER_NO_INCREMETNAL_BUILD_WORKAROUND

#include <macros_generic.h>
#include <cstdlib>
#include "memory_interface_base.hpp"

#define NAKED_FUNC_WRITER_EMIT_EOF_CHAR 0xCC
#define NAKED_FUNC_WRITER_EMIT_EOF_WORD 0xCCCCCCCC

namespace Yelo {
	namespace Enums {
		enum {
			k_naked_func_writer_max_asm_length = 256
		};
	};

	namespace Memory {
		/*
		Usage:

		Define the asm to a naked func:
			NAKED_FUNC_WRITER_ASM_BEGIN(lea_eax_some_array)
			lea eax, some_array
			nop													NAKED_FUNC_WRITER_ASM_END();

		Somewhere in your code, write the naked func to an address:
			Memory::c_naked_func_writer<lea_eax_some_array>::
				Write( K_SOME_ADDRESS1 );

		Use the nop_count overload when you have to fill the bytes of pre-existing opcodes or such
			Memory::c_naked_func_writer<lea_eax_some_array>::
				Write( K_SOME_ADDRESS2, 2 );
		*/
		template <void (__cdecl *k_naked_func)()>
		class c_naked_func_writer {
			template <void (__cdecl * )(), void (__cdecl * )()>
			friend
			class c_naked_func_writer_with_undo;

			// NOTE: MSDN says "/INCREMENTAL is implied when /DEBUG is specified"
			// So we have to jump on the function trampoline to get to the actual bytes address
			static byte *GetNakedFuncBytes() {
				return static_cast<byte *>(k_naked_func);
			}

			// Determine the assembly code length of [k_naked_func]
			static::std::size_t DetermineAsmLength() {
				static size_t g_length = 0;

				if (g_length == 0) {
					const byte *bytes = GetNakedFuncBytes();
					for (size_t x = 0; x < Enums::k_naked_func_writer_max_asm_length; x++) {
						if (*static_cast<const uint *>(bytes + x) == NAKED_FUNC_WRITER_EMIT_EOF_WORD) {
							g_length = x;
							break;
						}
					}
				}

				return g_length;
			}

		public:
			// Write the asm code of [k_naked_func] to [address]
			static size_t Write(void *address) {
				size_t asm_length = DetermineAsmLength();

				byte *bytes = GetNakedFuncBytes();
				byte *addr  = static_cast<byte *>(address);

				WriteMemory(addr, (int) bytes, asm_length);

				return asm_length;
			}

			// Write the asm code of [k_naked_func] to [address]
			// Then write a no-op code [nop_count] times
			// Only use this if k_naked_func is written multiple times and needs filler bytes in one or more places
			// Otherwise add the 'nop' instructions to the k_naked_func's asm directly
			static size_t Write(void *address, size_t nop_count) {
				size_t asm_length = DetermineAsmLength();

				byte *bytes = GetNakedFuncBytes();
				byte *addr  = static_cast<byte *>(address);

				WriteMemory(addr, (int) bytes, asm_length);
				WriteMemory(addr + asm_length, Enums::_x86_opcode_nop, nop_count);

				return asm_length + nop_count;
			}
		};

		// [k_naked_undo_func] - original asm bytes
		// [k_naked_func] - new asm bytes
		template <void (__cdecl *k_naked_undo_func)(), void (__cdecl *k_naked_func)()>
		class c_naked_func_writer_with_undo {
			typedef c_naked_func_writer<k_naked_undo_func> old_func_t;
			typedef c_naked_func_writer<k_naked_func>      new_func_t;

			static void AssertAsmLengths(const char *asm_mismatch_msg) {
				size_t old_func_length = old_func_t::DetermineAsmLength();
				size_t new_func_length = new_func_t::DetermineAsmLength();
				//YELO_ASSERT_DISPLAY( old_func_length==new_func_length, "%s", asm_mismatch_msg );
			}

			static void Verify(const void *code_addr, const char *asm_mismatch_msg) {
				//DebugRunOnce( AssertAsmLengths(asm_mismatch_msg) );

				size_t old_func_length = old_func_t::DetermineAsmLength();
				byte *old_func_bytes = old_func_t::GetNakedFuncBytes();
				//YELO_ASSERT_DISPLAY( memcmp(code_addr, old_func_bytes, old_func_length)==0, "%s", asm_mismatch_msg );
			}

		public:
			// Write the asm code of [k_naked_func] to [address]
			static size_t Write(void *address) {

				return new_func_t::Write(address);
			}

			// Write the asm code of [k_naked_undo_func] to [address]
			static size_t Undo(void *address) {
				return old_func_t::Write(address);
			}
		};
	};
};
