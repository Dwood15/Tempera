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
	bool WriteMemory(void *address, const void *src, size_t size);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copies the bytes of a buffer to a foriegn memory space. </summary>
	///
	/// <param name="address">	Start of the foreign memory space. </param>
	/// <param name="src">   	Buffer to copy from. </param>
	///
	/// <returns>	not FALSE if it succeeds, FALSE if it fails. </returns>
	template <typename T, size_t size>
	inline bool WriteMemory(void *address, const T (&src)[size]);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Overwrites a range of foreign memory with a specific byte. </summary>
	///
	/// <param name="address">	Start address of the foreign memory to overwrite. </param>
	/// <param name="value">  	The byte value to use when overwriting. </param>
	/// <param name="count">  	Number of times to write the byte. </param>
	void WriteMemory(void *address, int value, size_t count);


	// makes the 32 bits at [address] equal [value]
	bool WriteMemory(void *address, void *value);
#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast
	// At [hook_address] writes 'call [hook]' plus user specified end opcode (total: 6 bytes)
	void CreateHookRelativeCall(void *hook, void *hook_address, byte end);

	// At [jmp_address]+1, write the relative address of [to_address] (total: 5 bytes)
	// Writes '0xE9' if [write_opcode]
	// returns the original jmp address (absolute)
	uintptr_t WriteRelativeJmp(void *to_address, void *jmp_address, bool write_opcode = false);

	// At [call_address]+1, write the relative address of [to_address] (total: 5 bytes)
	// Writes '0xE8' if [write_opcode]
	// returns the original call address (absolute)
	uintptr_t WriteRelativeCall(void *to_address, void *call_address, bool write_opcode = false);

}

namespace calls {
	template <typename T>
	static void patchValue(uintptr_t to_patch, T replace_with) {
		*(T *) to_patch = replace_with;
	}

	// template<typename T, uint num>
	// void patchList(T replace_with, std::array<uintptr_t, num> list[]) {
	// 	for(auto ptr : list) {
	// 		patchValue<T>(ptr, replace_with);
	// 	}
	// }

	static unsigned int calc_addr_offset(const uintptr_t dest, int real_address) {
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
	void __inline WriteSimpleHook(uintptr_t target, T toCall) {
		uintptr_t addr = calc_addr_offset(target, (int)toCall);
		patchValue<uintptr_t>(target, addr);
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
