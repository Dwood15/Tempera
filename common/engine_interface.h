#pragma once
#include <macros_generic.h>
#include <enums/memory_enums.h>
#include <array>
#include <enums/player_enums.h>

/**
 * @brief MPP == MAX PLAYER PATCH _B == BYTE
 * @param value == address to_patch
 */
#define MPP_B(value, ...) patchValue<byte>(value, MAX_PLAYER_COUNT_LOCAL);
#define MPP_ARB(value, arb, ...) patchValue<byte>(value, arb);

namespace Yelo::Enums {
	enum CallingConventions {
		m_cdecl, //Everything goes onto the stack - stack is cleaned up by the caller.
		m_stdcall, //Everything goes onto the stack, but stack is cleaned up by callee.
		m_fastcall, //Example: __fastcall void Foo(int iGoInto_ECX, int iGoInto_EDX, int iGetPushed_Last, int iGetPushed_2nd, int iGetPushed_First);
		m_thiscall,
		m_custom
	};
}

typedef Yelo::Enums::CallingConventions Convention;

namespace memory {
	using namespace Yelo;
	using namespace Yelo::Memory;
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copies the bytes of a buffer to a foreign memory space. </summary>
	///
	/// <remarks>	Changes the protection of the foreign memory while copying, then reverts it back. </remarks>
	///
	/// <param name="address">	Start of the foreign memory space. </param>
	/// <param name="src">	  	Buffer to copy from. </param>
	/// <param name="size">   	The size of the buffer. </param>
	///
	/// <returns>	not FALSE if it succeeds, FALSE if it fails. </returns>
	bool WriteMemory(void *address, const void *src, size_t size) {
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
	inline bool WriteMemory(void *address, const T (&src)[size]) {
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


	// makes the 32 bits at [address] equal [value]
	bool WriteMemory(void *address, void *value) {
		*reinterpret_cast<unsigned __int32 *>(address) = reinterpret_cast<uintptr_t>(value);
		return true;
	}
#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast
	// At [hook_address] writes 'call [hook]' plus user specified end opcode (total: 6 bytes)
	void CreateHookRelativeCall(void *hook, void *hook_address, byte end) {
		struct relative_call_bytes : Yelo::Memory::Opcode::s_call {
			byte End;
		};
		STAT_ASSERT(relative_call_bytes, 6)
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

}



namespace calls {

	template <typename T>
	void patchValue(uintptr_t to_patch, T replace_with) {
		*(T *) to_patch = replace_with;
	}

	// template<typename T, uint num>
	// void patchList(T replace_with, std::array<uintptr_t, num> list[]) {
	// 	for(auto ptr : list) {
	// 		patchValue<T>(ptr, replace_with);
	// 	}
	// }

	static const unsigned int calc_addr_offset(const uintptr_t dest, int real_address) {
		int real_address_offset = (real_address) - ((int) dest) - 4;
		return static_cast<unsigned int>(real_address_offset);
	}

	void __inline nopBytes(uintptr_t location, unsigned short numNops) {
		for (unsigned short i = 0; i < numNops; i++) {
			patchValue<byte>(location + i, 0x90);
		}
	}

	void __inline adjustNPatch32(uintptr_t *loc, uint32 size) {
		patchValue<uint32>(loc[0], size);
		patchValue<uint32>(loc[1], size);
	}
	template<typename T>
	inline void WriteSimpleHook(uintptr_t loc, T newLoc) {
		uintptr_t addr = calc_addr_offset(loc, (int)newLoc);
		patchValue<uintptr_t>(loc, addr);
	};

	/**
	 * Do a competent function call against the engine
	 * @tparam conv
	 * @tparam retType
	 * @tparam argTypes
	 * @param addr
	 * @param args
	 * @return
	 */
	// template <uintptr_t addr, Convention conv = Convention::m_cdecl, typename retType = void, typename ...argTypes>
	// inline retType DoCall(argTypes... args) {
	// 	// typedef retType (__stdcall *function_t)(argTypes...);
	// 	using ufunc_t = retType(__cdecl *)(argTypes...);
	//
	// 	if constexpr(conv == Convention::m_stdcall) {
	// 		using ufunc_t = retType(__stdcall *)(argTypes...);
	//
	// 	} else if constexpr(conv == Convention::m_fastcall) {
	// 		using ufunc_t = retType(__fastcall *)(argTypes...);
	//
	// 	} else if constexpr(conv == Convention::m_thiscall) {
	// 		using ufunc_t = retType(__thiscall *)(argTypes...);
	//
	// 	} else if constexpr(conv == Convention::m_cdecl) {
	// 		using ufunc_t = retType(__cdecl *)(argTypes...);
	//
	// 	} else {
	// 		throw "Invalid return type specified!";
	// 	}
	//
	// 	static const ufunc_t func_to_call = reinterpret_cast<ufunc_t>( addr );
	// 	return func_to_call(args...);
	// };
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wunused-local-typedef"
	/**
	 * Do a competent function call against the engine
	 * @tparam conv
	 * @tparam retType
	 * @tparam argTypes
	 * @param addr
	 * @param args
	 * @return
	 */
	 template <Convention conv = Convention::m_cdecl, typename retType = void, typename ...argTypes>
	inline retType DoCall(uintptr_t addr, argTypes... args) {
		// typedef retType (__stdcall *function_t)(argTypes...);
		using ufunc_t = retType(__cdecl *)(argTypes...);

		if constexpr(conv == Convention::m_stdcall) {
			using ufunc_t = retType(__stdcall *)(argTypes...);

		} else if constexpr(conv == Convention::m_fastcall) {
			using ufunc_t = retType(__fastcall *)(argTypes...);

		} else if constexpr(conv == Convention::m_thiscall) {
			using ufunc_t = retType(__thiscall *)(argTypes...);

		} else if constexpr(conv == Convention::m_cdecl) {
			using ufunc_t = retType(__cdecl *)(argTypes...);

		} else {
			throw "Invalid return type specified!";
		}

		static ufunc_t func_to_call = reinterpret_cast<ufunc_t>( addr );
		return func_to_call(args...);
	};
};

template <typename Func, typename i_t = int, i_t max = MAX_PLAYER_COUNT_LOCAL>
void force_loop_players(Func f) {
	if constexpr (max > 4) {
		for (i_t i = 4; i < max; i++) {
			f(i);
		}
	}

	if constexpr (max > 3) {
		f(3);
	}

	if constexpr (max > 2) {
		f(2);
	}

	if constexpr (max > 1) {
		f(1);
	}

	f(0);
}
