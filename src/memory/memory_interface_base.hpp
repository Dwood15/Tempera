
#include <limits>
#include <enums/memory_enums.h>
#include <array>
#include <engine_interface.h>
#include "macros_generic.h"

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

namespace Yelo::Memory {
#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast

#pragma warning( pop )

	struct s_memory_exec_change_data {
		void *const ExecAddress;   /// <summary>	'to_address', address to be executed (jmp or call) </summary>
		void *const MemoryAddress;   /// <summary>	'call_address', memory address to apply the execution modification. </summary>

		const bool WriteOpcode;
		bool       IsInitialized;
		unsigned short : 16;
		::std::array<byte, 5> UndoData;/// <summary>	internal buffer to store. </summary>
		unsigned char  : 8;
		unsigned short : 16;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	At [MemoryAddress], modify the assembly code to jump to the code stored at address [ExecAddress].
		/// </summary>
		///
		/// <remarks>	Total: 5 bytes. See: [WriteRelativeJmp]. </remarks>
		void ApplyAsRelativeJmp() {
			this->Initialize();
			memory::WriteRelativeJmp(ExecAddress, MemoryAddress, WriteOpcode);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	At [MemoryAddress], modify the assembly code to execute the code stored at address [ExecAddress].
		/// </summary>
		///
		/// <remarks>	Total: 5 bytes. See: [WriteRelativeCall]. </remarks>
		void ApplyAsRelativeCall() {
			this->Initialize();
			memory::WriteRelativeCall(ExecAddress, MemoryAddress, WriteOpcode);
		}

		/// <summary>	Restore the memory back to it's original data. </summary>
		void Undo() {
			if (this->IsInitialized) {
				this->IsInitialized = memcpy_s(MemoryAddress, UndoData.size(), UndoData.data(), UndoData.size()) != 0;
			}
		}

	private:
		void Initialize() {
			if (!this->IsInitialized) {
				this->IsInitialized = memcpy_s(UndoData.data(), UndoData.size(), MemoryAddress, UndoData.size()) == 0;
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
		static bool g_initialized;
		static bool g_memory_changed;
		// the bytes found at [k_memory_address], before any changes were ever applied
		static byte g_undo_buffer[k_memory_address_size];

		void InitializeUndoBuffer() {
			if (!g_initialized) {
				g_initialized = true;
				memcpy(g_undo_buffer, k_memory_address, k_memory_address_size);
			}
		}

		// At [k_memory_address], write enough asm nop codes to fill [k_memory_address_size]
		void MemoryApplyNopCodes() {
			InitializeUndoBuffer();

			memset(k_memory_address, Enums::_x86_opcode_nop, k_memory_address_size);
			g_memory_changed = true;
		}

		// Revert [k_memory_address] back to the memory bytes it had when this was initialized
		void MemoryApplyUndo() {
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
			s_memory_change_data::InitializeUndoBuffer();

			OverwriteMemorySansCopyArray(k_memory_address, m_new_memory);
			this->g_memory_changed = true;
		}

		// Revert [k_memory_address] back to the memory bytes it had when this was initialized
		void ApplyUndo() {
			s_memory_change_data::MemoryApplyUndo();
		}
	};

	static uintptr_t AlignValue(uintptr_t value, unsigned alignment_bit) {
		const uintptr_t alignment_mask = ((unsigned) (1 << (alignment_bit)) - (unsigned) 1);

		if (value & alignment_mask)
			value = (value | alignment_mask) + 1;

		return value;
	}

	template <typename T>
	inline T *RebasePointer(uintptr_t pointer, uintptr_t base_address, uintptr_t virtual_base_address) {
		return reinterpret_cast<T *>((pointer - base_address) + virtual_base_address);
	}

	template <typename T>
	inline T *RebasePointer(T *pointer, uintptr_t base_address, uintptr_t virtual_base_address) {
		return RebasePointer < T > (reinterpret_cast<uintptr_t>(pointer), base_address, virtual_base_address);
	}

	template <typename T>
	inline T *AlignPointer(T *pointer, unsigned alignment_bit) {
		uintptr_t aligned_ptr = reinterpret_cast<uintptr_t>(pointer);
		aligned_ptr = AlignValue(aligned_ptr, alignment_bit);

		return reinterpret_cast<T *>(aligned_ptr);
	}
};

namespace Yelo::blam {
	inline void crc_new(long &crc_reference) {
#ifdef max
#undef max
#endif
		crc_reference = ::std::numeric_limits<unsigned long>::max();
	}
};
