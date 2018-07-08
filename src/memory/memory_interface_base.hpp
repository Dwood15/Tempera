#include <precompile.h>
#include "../cseries/base.h"

#pragma once
//////////////////////////////////////////////////////////////////////////
// Engine pointer markup system
//
//-Reference based interface
//	ENGINE_DPTR
//		Define a platform variant reference to a pointer in the engine's memory
//	ENGINE_PTR
//		Define a platform variant reference to a value in the engine's memory
//	Macro parameter notes:
//		* If [PTR_NULL], labels that when compiling for that platform we don't 
//			make use of the 'reference' we defined
//		* If [PTR_UNKNOWN], labels that when compiling for that platform we don't 
//			know the address of the 'reference' we're trying to create. In this 
//			situation, compilation of the source will be halted and no output 
//			executable will be created.
//
//-Constant based interface
//	FUNC_PTR
//		Declare a platform variant constant representing a engine function's address
//	DATA_PTR
//		Declare a platform variant constant representing a engine variable's address
//	Macro parameter notes:
//		* See the 'reference' based interface notes.
//		* FUNC_PTR uses: [FUNC_PTR_NULL], [FUNC_PTR_UNKNOWN]
//		* DATA_PTR uses: [DATA_PTR_NULL], [DATA_PTR_UNKNOWN]
//	PLATFORM_VALUE
//		Inline a platform variant constant (could be a number, string, code, etc)
//////////////////////////////////////////////////////////////////////////

// Use this value for platforms where this pointer does not matter
#define PTR_NULL         0x1337BEEF
// Use this value for platforms where this pointer hasn't been located yet
#define PTR_UNKNOWN         NULL
// Use this value for platforms where this function does not matter
// Use this value for platforms where this function hasn't been located yet
#define FUNC_PTR_UNKNOWN   NULL
// Use this value for platforms where this data does not matter
#define DATA_PTR_NULL      0xDEADBEEF
// Use this value for platforms where this data hasn't been located yet
#define DATA_PTR_UNKNOWN   NULL

// Gets the enum value of [name]
#define GET_FUNC_PTR(name) PTR_##name
#define GET_DATA_PTR(name) PTR_##name
// Type casts [name] to a void*, since FUNC_PTRs are really enums
#define GET_FUNC_VPTR(name) (CAST_PTR(void*, PTR_##name ))
#define GET_DATA_VPTR(name) (CAST_PTR(void*, PTR_##name ))

// Double pointer inline get
#define GET_DPTR2(name)      pp##name)
// Pointer inline get
#define GET_PTR2(name)      p##name
// Double pointer inline dereference-get
#define GET_DPTR(name)      ( * pp##name )
// Pointer inline dereference-get
#define GET_PTR(name)      ( * p##name) )

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Macro glue for getting a pointer to game engine data, with various levels of pointer asserts. </summary>
/// <remarks>	va_args allow the callee to specify a de-reference operator. </remarks>
/// <param name="get_ptr_macro">	Macro for getting the PTR object. </param>
/// <param name="get_macro">		Macro for getting PTR_IMPL's value. </param>
/// <param name="name">				The name of the PTR object. </param>
#define PTR_IMPL_GET_GUTS(get_ptr_macro, get_macro, name, ...)                     \
   /* validate pointer is available for this platform */                        \
{   auto name = get_ptr_macro(name);                                       \
   assert(CAST_PTR(void*,PTR_UNKNOWN) != name && CAST_PTR(void*,PTR_NULL) != name); }   \
   /* validate pointer is initialized */                                    \
   auto name = get_macro(name);                                          \
/*	YELO_ASSERT(name);																*/   \
   return __VA_ARGS__ name;

// Double pointer implement get by-value
#define DPTR_IMP_GET(name)      { PTR_IMPL_GET_GUTS(GET_DPTR2, GET_DPTR , name) }
// Double pointer implement get by-reference
#define DPTR_IMP_GET2(name)      { PTR_IMPL_GET_GUTS(GET_DPTR2, GET_DPTR2, name) }
// Pointer implement get by-value
// Pointer implement get by-reference

// Double pointer implement get by-reference, with debug assert
#define DPTR_IMP_GET_BYREF(name){ PTR_IMPL_GET_GUTS(GET_DPTR2, GET_DPTR, name, *) }

// MSVC hack for a bug MS REFUSES to fix, since mother fucking VS2008
// http://stackoverflow.com/a/10002424/444977
// I don't know why we have to use this for FUNC_PTR/DATA_PTR but not ENGINE_PTR.
// Bug must only occur when there's only one non-va-arg
#define PLATFORM_VALUE_HACK_(args_list) PLATFORM_VALUE args_list

#define ENGINE_PTR(type, name, ...) static auto* const p##name = reinterpret_cast<type*>(__VA_ARGS__);  static_assert(__VA_ARGS__ != NULL );

#define DATA_PTR(name, ...)                           \
   enum DATA_PTR_##name                           \
   { PTR_##name = PLATFORM_VALUE_HACK_((__VA_ARGS__)) };   \
   static_assert( GET_DATA_PTR(name) != NULL );

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
		static_assert(sizeof(Enums::x86_opcode_twobyte) == 2);
	};

	namespace Memory {
		struct Opcode {
			struct s_call {
				byte   Op;
				unsigned int Address;
			};

			struct s_call_ret : s_call {
				byte   Ret;
				unsigned __int16 Count;
			};

			// absolute call opcode
			struct s_call_abs {
				unsigned __int16 Op;
				unsigned __int32 Address;
			};

			// absolute call opcode with return
			struct s_call_abs_ret : s_call_abs {
				byte   Ret;
				unsigned __int16 Count;
			};
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copies the bytes of a buffer to a foriegn memory space. </summary>
		/// 
		/// <remarks>	Changes the protection of the foreign memory while copying, then reverts it back. </remarks>
		///
		/// <param name="address">	Start of the foreign memory space. </param>
		/// <param name="src">	  	Buffer to copy from. </param>
		/// <param name="size">   	The size of the buffer. </param>
		///
		/// <returns>	not FALSE if it succeeds, FALSE if it fails. </returns>
		BOOL WriteMemory(void *address, const void *src, size_t size) {
			return address == memcpy(address, src, size);
		}



		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copies the bytes of a buffer to a foriegn memory space. </summary>
		///
		/// <param name="address">	Start of the foreign memory space. </param>
		/// <param name="src">   	Buffer to copy from. </param>
		///
		/// <returns>	not FALSE if it succeeds, FALSE if it fails. </returns>
		template <typename T, size_t size>
		inline BOOL WriteMemory(void *address, const T (&src)[size]) {
			return WriteMemory(address, src, sizeof(T) * size);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Overwrites a range of foreign memory with a specific byte. </summary>
		///
		/// <param name="address">	Start address of the foreign memory to overwrite. </param>
		/// <param name="value">  	The byte value to use when overwriting. </param>
		/// <param name="count">  	Number of times to write the byte. </param>
		void WriteMemory(void *address, int value, size_t count) {
			memset(address, value, count);
		}


#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast

		// makes the 32 bits at [address] equal [value]
		BOOL WriteMemory(void *address, void *value) {
			*reinterpret_cast<unsigned __int32 *>(address) = reinterpret_cast<uintptr_t>(value);
			return true;
		}

		// At [hook_address] writes 'call [hook]' plus user specified end opcode (total: 6 bytes)
		void CreateHookRelativeCall(void *hook, void *hook_address, byte end) {
			struct relative_call_bytes : Opcode::s_call {
				byte End;
			}; static_assert(sizeof(relative_call_bytes) == 6);
			// call near ....
			// retn\nop
			relative_call_bytes asm_bytes;

			asm_bytes.Op = Enums::_x86_opcode_call_near;

			// relative call to [hook]
			asm_bytes.Address = (reinterpret_cast<intptr_t>(hook)) -
									  ((reinterpret_cast<intptr_t>(hook_address)) +
										sizeof(Opcode::s_call));

			asm_bytes.End = end;

			WriteMemory(hook_address, &asm_bytes, sizeof(asm_bytes));
		}

		// At [jmp_address]+1, write the relative address of [to_address] (total: 5 bytes)
		// Writes '0xE9' if [write_opcode]
		// returns the original jmp address (absolute)
		uintptr_t WriteRelativeJmp(void *to_address, void *jmp_address, bool write_opcode = false) {
			byte real_opcode = Enums::_x86_opcode_jmp_near; // jmp [function]
			if (write_opcode)
				WriteMemory(jmp_address, &real_opcode, sizeof(real_opcode));

			uintptr_t original = (reinterpret_cast<intptr_t>(jmp_address)) + *reinterpret_cast<intptr_t *>( reinterpret_cast<unsigned __int32>(jmp_address) + 1) + sizeof(Opcode::s_call);

			uintptr_t relative = (reinterpret_cast<intptr_t>(to_address)) - ((reinterpret_cast<intptr_t>(jmp_address)) + sizeof(Opcode::s_call));
			WriteMemory((reinterpret_cast<void *>((reinterpret_cast<uintptr_t>(jmp_address)) + 1)), (reinterpret_cast<void *>(relative)));

			return original;
		}

		// At [call_address]+1, write the relative address of [to_address] (total: 5 bytes)
		// Writes '0xE8' if [write_opcode]
		// returns the original call address (absolute)
		uintptr_t WriteRelativeCall(void *to_address, void *call_address, bool write_opcode = false) {
			byte real_opcode = Enums::_x86_opcode_call_near; // call [function]
			if (write_opcode)
				WriteMemory(call_address, &real_opcode, sizeof(real_opcode));

			uintptr_t original = reinterpret_cast<intptr_t>(call_address) + *reinterpret_cast<intptr_t *>(reinterpret_cast<unsigned long>(call_address) + 1) + sizeof(Opcode::s_call);

			uintptr_t relative = reinterpret_cast<intptr_t>(to_address) - (reinterpret_cast<intptr_t>(call_address) + sizeof(Opcode::s_call));
			WriteMemory(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(call_address) + 1), reinterpret_cast<void *>(relative));

			return original;
		}

		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void Write(Opcode::s_call &call, void *address) {
			memcpy(address, &call, sizeof(call));
		}

		// [call_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a call
		// [target] address to make the call goto
		void WriteCall(void *call_buffer, void *address, const void *target) {
			Opcode::s_call *call         = (reinterpret_cast<Opcode::s_call *>(call_buffer));
			Opcode::s_call *call_address = (reinterpret_cast<Opcode::s_call *>(address));

			call->Op         = call_address->Op;               // copy the old
			call_address->Op = Enums::_x86_opcode_call_near;// set the new
			call->Address    = call_address->Address;         // copy the old
			call_address->Address =                     // set the new
				// cast the pointer to a number to perform math on
				reinterpret_cast<intptr_t>(target) - ( reinterpret_cast<intptr_t>(address) + sizeof(Opcode::s_call) );
		}

		// [call_ret_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a call and ret
		// [target] address to make the call goto
		void WriteCallRet(void *call_ret_buffer, void *address, const void *target) {
			(reinterpret_cast<Opcode::s_call_ret *>(call_ret_buffer))->Ret =
				(reinterpret_cast<Opcode::s_call_ret *>(address))->Ret;                  // copy the old
			WriteCall(call_ret_buffer, address, target);
			(reinterpret_cast<Opcode::s_call_ret *>(address))->Ret = Enums::_x86_opcode_ret;   // set the new
		}

		// [call_ret_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a call and ret
		// [target] address to make the call goto
		// [count] number of 32-bit args in the function we're modding. If there are any 
		// 64-bit arguments, count them twice!
		void WriteCallRet(void *call_ret_buffer, void *address, const void *target, const unsigned __int16 count) {
			(reinterpret_cast<Opcode::s_call_ret *>(call_ret_buffer))->Ret =
				(reinterpret_cast<Opcode::s_call_ret *>(address))->Ret;                  // copy the old
			WriteCall(call_ret_buffer, address, target);
			(reinterpret_cast<Opcode::s_call_ret *>(address))->Ret = Enums::_x86_opcode_retn;   // set the new
			(reinterpret_cast<Opcode::s_call_ret *>(call_ret_buffer))->Count =
				(reinterpret_cast<Opcode::s_call_ret *>(address))->Count;                  // copy the old
			(reinterpret_cast<Opcode::s_call_ret *>(address))->Count = (count * 4);         // set the new
		}

		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void WriteRet(Opcode::s_call_ret &call, void *address) {
			memcpy(address, &call, sizeof(call) - sizeof(unsigned long)); // don't include the retn's count
		}

		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void WriteRetn(Opcode::s_call_ret &call, void *address) {
			memcpy(address, &call, sizeof(call));
		}

		// [jmp_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a jmp
		// [target] address to make the jmp goto
		void WriteJmp(void *jmp_buffer, void *address, const void *target) {
			auto jmp         = reinterpret_cast<Opcode::s_call *>(jmp_buffer);
			auto jmp_address = reinterpret_cast<Opcode::s_call *>(address);

			jmp->Op         = jmp_address->Op;                  // copy the old
			jmp_address->Op = Enums::_x86_opcode_jmp_near;   // set the new
			jmp->Address    = jmp_address->Address;         // copy the old
			jmp_address->Address =                     // set the new
				reinterpret_cast<intptr_t>(target) -            // cast the pointer to a number to perform math on
				(
					reinterpret_cast<intptr_t>(address) + sizeof(Opcode::s_call)
				);
		}

		// [jmp_buffer] is a buffer to receive the old jmp address
		// [address] address to overwrite a jmp
		// [target] address to make the jmp goto
		// REMARKS:
		// Jmp type can be anything as long as the address used is 32bits
		void OverwriteJmp(void *jmp_buffer, void *address, const void *target) {
			auto jmp_address = reinterpret_cast<Opcode::s_call *>(address);

			reinterpret_cast<Opcode::s_call *>(jmp_buffer)->Address = jmp_address->Address; // copy the old
			jmp_address->Address =            // set the new
				reinterpret_cast<intptr_t>(target) -   // cast the pointer to a number to perform math on
				(
					reinterpret_cast<intptr_t>(address) + sizeof(Opcode::s_call)
				);
		}

#pragma warning( pop )

		struct s_memory_exec_change_data {
			void *const ExecAddress;   /// <summary>	'to_address', address to be executed (jmp or call) </summary>
			void *const MemoryAddress;   /// <summary>	'call_address', memory address to apply the execution modification. </summary>

			const bool          WriteOpcode;
			bool                IsInitialized;
									  unsigned short : 16;
			std::array<byte, 5> UndoData;/// <summary>	internal buffer to store. </summary>
									  unsigned char : 8; unsigned short : 16;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	At [MemoryAddress], modify the assembly code to jump to the code stored at address [ExecAddress].
			/// </summary>
			///
			/// <remarks>	Total: 5 bytes. See: [WriteRelativeJmp]. </remarks>
			void ApplyAsRelativeJmp() {
				this->Initialize();
				WriteRelativeJmp(ExecAddress, MemoryAddress, WriteOpcode);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	At [MemoryAddress], modify the assembly code to execute the code stored at address [ExecAddress].
			/// </summary>
			///
			/// <remarks>	Total: 5 bytes. See: [WriteRelativeCall]. </remarks>
			void ApplyAsRelativeCall() {
				this->Initialize();
				WriteRelativeCall(ExecAddress, MemoryAddress, WriteOpcode);
			}

			/// <summary>	Restore the memory back to it's original data. </summary>
			void Undo() {
				if (this->IsInitialized) {
					this->IsInitialized = !(memcpy_s(MemoryAddress, UndoData.size(), UndoData.data(), UndoData.size()) == k_errnone);
				}
			}

		private:
			void Initialize() {
				if (!this->IsInitialized) {
					this->IsInitialized = memcpy_s(UndoData.data(), UndoData.size(), MemoryAddress, UndoData.size()) == k_errnone;
				}
			}
		};

		static_assert(sizeof(s_memory_exec_change_data) == 0x14);
#define DEFINE_MEMORY_EXEC_CHANGE(name, to_address, call_address, ...)   \
   static Yelo::Memory::s_memory_exec_change_data name = {            \
         CAST_PTR(void*, (to_address) ),                        \
         CAST_PTR(void*, (call_address) ),                     \
         __VA_ARGS__                                       \
      };

		// Overwrite the memory at [address] with [array]
		// Copies the original memory at [address] into [array] before returning
		// This is useful when writing an array of opaque bytes to game memory
		template <typename T, size_t TSizeOfArray>
		void OverwriteMemoryArray(void *address, T(&array)[TSizeOfArray]) {
			T old_memory[TSizeOfArray];
			// Copy the old memory from the address
			memcpy(old_memory, address, TSizeOfArray);
			// Write the new memory to the address
			memcpy(address, array, TSizeOfArray);
			// Fill [array] with the old memory
			memcpy(array, old_memory, TSizeOfArray);
		}

		// Overwrite the memory at [address] with [array]
		// Does NOT copy the original memory at [address] into [array] before returning
		// This is useful when writing an array of opaque bytes to game memory, esp. during unwinding operations
		template <typename T, size_t TSizeOfArray>
		void OverwriteMemorySansCopyArray(void *address, const T(&array)[TSizeOfArray]) {
			// Write the new memory to the address
			memcpy(address, array, TSizeOfArray);
		}

		// Overwrite the memory at [address] with [data]
		// Copies the original memory at [address] into [data] before returning
		// This is useful when writing a concrete object to game memory
		template <typename T>
		void OverwriteMemory(void *address, T &data) {
			T old_memory;
			// Copy the old memory from the address
			memcpy(&old_memory, address, sizeof(T));
			// Write the new memory to the address
			memcpy(address, &data, sizeof(T));
			// Fill [data] with the old memory
			memcpy(&data, &old_memory, sizeof(T));
		}

		// Overwrite the memory at [address] with [data]
		// Does NOT copy the original memory at [address] into [data] before returning
		// This is useful when writing a concrete object to game memory, esp. during unwinding operations
		template <typename T>
		void OverwriteMemorySansCopy(void *address, const T &data) {
			// Write the new memory to the address
			memcpy(address, &data, sizeof(T));
		}

		template <void *k_memory_address, size_t k_memory_address_size>
		struct s_memory_change_data {
		protected:
			static bool g_initialized;
			static bool g_memory_changed;
			// the bytes found at [k_memory_address], before any changes were ever applied
			static byte g_undo_buffer[k_memory_address_size];

			static void InitializeUndoBuffer() {
				if (!g_initialized) {
					g_initialized = true;
					memcpy(g_undo_buffer, k_memory_address, k_memory_address_size);
				}
			}

		public:
			// At [k_memory_address], write enough asm nop codes to fill [k_memory_address_size]
			static void MemoryApplyNopCodes() {
				InitializeUndoBuffer();

				memset(k_memory_address, Enums::_x86_opcode_nop, k_memory_address_size);
				g_memory_changed = true;
			}

			// Revert [k_memory_address] back to the memory bytes it had when this was initialized
			static void MemoryApplyUndo() {
				InitializeUndoBuffer();

				if (g_memory_changed) {
					OverwriteMemorySansCopyArray(k_memory_address, g_undo_buffer);
					g_memory_changed = false;
				}
			}
		};

		template <void *k_memory_address, size_t k_memory_address_size>
		bool s_memory_change_data<k_memory_address, k_memory_address_size>::g_initialized;
		template <void *k_memory_address, size_t k_memory_address_size>
		bool s_memory_change_data<k_memory_address, k_memory_address_size>::g_memory_changed;
		template <void *k_memory_address, size_t k_memory_address_size>
		byte s_memory_change_data<k_memory_address, k_memory_address_size>::g_undo_buffer[k_memory_address_size];

		template <void *k_memory_address, size_t k_memory_address_size>
		struct s_memory_change_data_object : s_memory_change_data<k_memory_address, k_memory_address_size> {
			byte m_new_memory[k_memory_address_size];

			// At [k_memory_address], write the contents of [m_new_memory]
			void ApplyChanges() {
				InitializeUndoBuffer();

				OverwriteMemorySansCopyArray(k_memory_address, m_new_memory);
				g_memory_changed = true;
			}

			// Revert [k_memory_address] back to the memory bytes it had when this was initialized
			void ApplyUndo() {
				MemoryApplyUndo();
			}
		};

		uintptr_t AlignValue(uintptr_t value, unsigned alignment_bit) {
			const uintptr_t alignment_mask = MASK(alignment_bit);

			if (value & alignment_mask)
				value = (value | alignment_mask) + 1;

			return value;
		}

		template <typename T>
		inline
		T *RebasePointer(uintptr_t pointer, uintptr_t base_address, uintptr_t virtual_base_address) {
			return reinterpret_cast<T *>((pointer - base_address) + virtual_base_address);
		}

		template <typename T>
		inline
		T *RebasePointer(T *pointer, uintptr_t base_address, uintptr_t virtual_base_address) {
			return RebasePointer < T > (reinterpret_cast<uintptr_t>(pointer), base_address, virtual_base_address);
		}

		template <typename T>
		inline
		T *AlignPointer(T *pointer, unsigned alignment_bit) {
			uintptr_t aligned_ptr = reinterpret_cast<uintptr_t>(pointer);
			aligned_ptr = AlignValue(aligned_ptr, alignment_bit);

			return reinterpret_cast<T *>(aligned_ptr);
		}

		unsigned long CRC(unsigned long&crc_reference, const void *buffer, long size) {
			auto p = (reinterpret_cast<const byte *>(buffer));

			while (size--) {
				unsigned long a = (crc_reference >> 8) & 0x00FFFFFFL;
				unsigned long b = g_crc32_table[((long) crc_reference ^ *p++) & 0xFF];
				crc_reference = a ^ b;
			}

			return crc_reference;
		}
	};

	namespace blam {
		inline void crc_new(long &crc_reference) {
			crc_reference = std::numeric_limits<unsigned long>::max();
		}

		inline unsigned long crc_checksum_buffer(unsigned long &crc_reference, const void *buffer, long size) {
			return Memory::CRC(crc_reference, buffer, size);
		}
	};
};
