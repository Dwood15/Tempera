/*
	Project: tempera
   You should have received a copy of the GNU General Public License
   along with tempera. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <assert.h>
#include "headers/macros_generic.h"
#include "headers/real_math.h"

typedef short                     int16;
typedef unsigned int              uintptr_t;
typedef char                      tag_string[32];
typedef const char                *cstring;

namespace Enums {
	constexpr enum CallingConventions {
		_cdecl, //Everything goes onto the stack - stack is cleaned up by the caller.
		_stdcall, //Everything goes onto the stack, but stack is cleaned up by callee.
		_fastcall //Example: __fastcall void Foo(int iGoInto_ECX, int iGoInto_EDX, int iGetPushed_Last, int iGetPushed_2nd, int iGetPushed_First);
	};
}
typedef Enums::CallingConventions Convention;

template <typename retType, typename ...argTypes>
inline retType DoStdCall(uintptr_t addr, argTypes... args) {
	// typedef retType (__stdcall *function_t)(argTypes...);
	using ufunc_t = retType (__stdcall *)(argTypes...);
	static const ufunc_t func_to_call = reinterpret_cast<ufunc_t>(addr);
	return func_to_call(args...);
};

template <typename retType, typename ...argTypes>
inline retType DoFastCall(uintptr_t addr, argTypes... args) {
	// typedef retType (__stdcall *function_t)(argTypes...);
	using ufunc_t = retType (__fastcall *)(argTypes...);
	static const ufunc_t func_to_call = reinterpret_cast<ufunc_t>(addr);
	return func_to_call(args...);
};

template <typename retType, typename ...argTypes>
inline retType DoCdeclCall(uintptr_t addr, argTypes... args) {
	// typedef retType (__stdcall *function_t)(argTypes...);
	using ufunc_t = retType (__cdecl *)(argTypes...);
	static const ufunc_t func_to_call = reinterpret_cast<ufunc_t>(addr);
	return func_to_call(args...);
};

template <typename retType, Convention convention, typename ...argTypes>
inline retType TryCall(uintptr_t addr, argTypes... args) {
	if constexpr (convention == Convention::_stdcall) {

		return DoStdCall<retType, argTypes...>(addr, args...);

	} else if (convention == Convention::_fastcall) {

		return DoFastCall<retType, argTypes...>(addr, args...);

	} else if (convention == Convention::_cdecl) {

		return DoCdeclCall<retType, argTypes...>(addr, args...);
	}
	throw "Invalid template parameters!";
};

/// <summary>	Handle to data allocated by the engine's data-array construct. </summary>
struct datum_index {
	typedef short t_index;
	typedef short t_salt;

	union {
		signed long handle;

		struct {
			t_index index;
			t_salt  salt;
		};
	};

	operator int() { return handle; }

	datum_index operator =(const int &rhs) {
		handle = rhs;
		return *this;
	}

};

STAT_ASSRT(datum_index, 0x4);

struct tag_reference {
	unsigned int group_tag;
	const char   *name;
	int          name_length;
	datum_index  tag_index;
};
STAT_ASSRT(tag_reference, 0x10);
//This is guaranteed to be accurate b/c of stat_assrt macro.
#define TREF_SIZEOF sizeof(tag_reference);

template <typename T>
struct tag_block {
	typedef T *iter;

	//size -> in bytes.
	long size;
	union {
		void *address;
		T    *definitions;
	};

	long constexpr Count() { return size / sizeof(T); }

	[[maybe_unused]]
	iter begin() { return definitions; }

	iter end() { return definitions + Count(); }

	struct tag_block_definition *definition;
};
