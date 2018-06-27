#pragma once

#include "../precompile.h"

/**
 * @brief MPP == MAX PLAYER PATCH _B == BYTE
 * @param value == address to_patch
 */
#define MPP_B(value, ...) patchValue<byte>(value, MAX_PLAYER_COUNT_LOCAL);
#define MPP_ARB(value, arb, ...) patchValue<byte>(value, arb);

namespace Enums {
	enum CallingConventions {
		m_cdecl, //Everything goes onto the stack - stack is cleaned up by the caller.
		m_stdcall, //Everything goes onto the stack, but stack is cleaned up by callee.
		m_fastcall, //Example: __fastcall void Foo(int iGoInto_ECX, int iGoInto_EDX, int iGetPushed_Last, int iGetPushed_2nd, int iGetPushed_First);
		m_thiscall
	};
}

typedef Enums::CallingConventions Convention;

namespace calls {

	template <typename T>
	void patchValue(uintptr_t to_patch, T replace_with) {
		*(T *) to_patch = replace_with;
	}

	static const unsigned int calc_addr_offset(const uintptr_t dest, int real_address) {
		int real_address_offset = (real_address) - ((int) dest) - 4;
		return static_cast<unsigned int>(real_address_offset);
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
	template <uintptr_t addr, Convention conv = Convention::m_cdecl, typename retType = void, typename ...argTypes>
	inline retType DoCall(argTypes... args) {
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

		static const ufunc_t func_to_call = reinterpret_cast<ufunc_t>( addr );
		return func_to_call(args...);
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

		static const ufunc_t func_to_call = reinterpret_cast<ufunc_t>( addr );
		return func_to_call(args...);
	};

	// template <uintptr_t addr, Convention conv>
	// inline void DoCall() {
	// 	// typedef retType (__stdcall *function_t)(argTypes...);
	// 	using ufunc_t = void(__cdecl *)();
	//
	// 	if constexpr(conv == Convention::m_stdcall) {
	// 		using ufunc_t = void(__stdcall *)();
	//
	// 	} else if constexpr(conv == Convention::m_fastcall) {
	// 		using ufunc_t = void(__fastcall *)();
	//
	// 	} else if constexpr(conv == Convention::m_thiscall) {
	// 		using ufunc_t = void(__thiscall *)();
	//
	// 	} else if constexpr(conv == Convention::m_cdecl) {
	// 		using ufunc_t = void(__cdecl *)();
	//
	// 	} else {
	// 		throw "Invalid return type specified!";
	// 	}
	//
	// 	static const ufunc_t func_to_call = reinterpret_cast<ufunc_t>( addr );
	// 	func_to_call();
	// };
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
