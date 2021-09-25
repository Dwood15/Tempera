/**
 * GPLv3 or Later
 */
#pragma once

#ifndef shitty_macros
#define shitty_macros

#include <cstddef>


//WARNING: Please avoid nesting macros/typedefs, that breaks intellisense 9/10 times.
#pragma region sanity checks

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"

#define TO_STR(x) #x
#define XSTR(s) TO_STR(s)

template <size_t Diff>
struct TAssertEqualityDiff {
	static_assert(Diff == 0, "A != B");
	static constexpr bool _cResult2 = (Diff == 0);
	static constexpr bool _cResult = (Diff == 0);
};

template <size_t A, size_t B>
struct TAssertEquality {
	static constexpr bool AssertDiff() {
		if constexpr(A > B) {
			TAssertEqualityDiff<A - B> inst;
			return A == B;
		}

		if constexpr(B > A) {
			TAssertEqualityDiff<B - A> inst;
			return B == A;
		}
		return true;
	}

	static_assert(A-B == 0, "A != B");
	static constexpr bool _cResult = (A-B == 0);
	static constexpr bool _cResult2 = (B-A == 0);
};

#define STAT_ASSERT(object, size) static_assert(TAssertEquality<sizeof(object), size>::_cResult);

#define IMPLEMENTATION_REQUIRED static_assert(false, "This features requires implementation to compile on this target.");

/*auto numDigits = [](unsigned long num, unsigned long dig = 0) constexpr {
	
	while()
	if (num < 10) {
		return dig + 1;
	}

	return numDigits(num / 10);
};

static_assert(numDigits(12) == 2);
static_assert(numDigits(100) == 3);
static_assert(numDigits(1001) == 4); */
//constexpr char [] style functions don't work when passed into static_assert. 

//#pragma clang diagnostic pop

//#define STAT_ASSERT(object, size) static_assert(size_check<object, size>());

typedef float          real;
typedef int            int32;
typedef long           byte_swap_code_t;
typedef signed char    sbyte;
typedef unsigned char  byte;
typedef unsigned int   *uintptr;
typedef unsigned int   uint32;
typedef unsigned short   uint16;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned long  long_flags;
typedef signed char    byte_enum;
typedef unsigned char  byte_flags;
typedef short          int16;

//sanity checks.
STAT_ASSERT(bool, 0x1);
STAT_ASSERT(char, 0x1);
STAT_ASSERT(short, 0x2);
STAT_ASSERT(__int16, 0x2);
STAT_ASSERT(__int32, 0x4);
STAT_ASSERT(long, 0x4);
STAT_ASSERT(float, 0x4);
STAT_ASSERT(long long, 0x8);
STAT_ASSERT(double, 0x8);
STAT_ASSERT(size_t, sizeof(unsigned long));

//Typedef'd sanity checks
STAT_ASSERT(ushort, 0x2);
#pragma endregion
#define INTELLISENSE_HACK(item) //int a[sizeof(item)] = 0;

#define INVALID -1

#pragma region functional
#define FLAG(bit)               ( 1<<((uint32)bit) )
#define UNK_TYPE(type) pad_##type
#define PAD24(name) unsigned char name[24];

#pragma region pad/unknown/unused macros
// Pad a structure using a byte[] field named pad[num] by [count] bytes
#define PAD(num, count) byte pad##num##[ count ]
// Pad a structure using a byte[] field named pad_[name] by [count] bytes
#define PAD_(name, count) byte pad_##name##[ count ]
// Markup a field as unknown
#define UNKNOWN(num) Unknown##num

/// Add an anonymous 8-bit (1 byte) field to a structure.
#define PAD8 unsigned char : 8;
/// Add an anonymous 16-bit (2 byte) field to a structure.
/// Add an anonymous 24-bit field to a structure.
//#define PAD24 unsigned char : 8; unsigned short : 16;
/// Add an anonymous 32-bit (4 byte) field to a structure.
#define PAD32 unsigned long : 32;
/// Add an anonymous 48-bit field to a structure.
#define PAD48 unsigned short : 16; unsigned long : 32;
/// Add an anonymous 64-bit (8 byte) field to a structure.
#define PAD64 unsigned long long : 64;
/// Add an anonymous 128-bit (16 byte) field to a structure.
#define PAD128 unsigned long long : 64; unsigned long long : 64;

// Add a field to a structure that pads as if it were of type [type]
#define PAD_TYPE(type) type pad_##type
// Add a field to a structure that markups what is thought to be a field of type [type]
#define UNKNOWN_TYPE(type) pad_##type

// Add a field to a structure that markups a unused field of type [type]
#define UNUSED_32(type) unsigned long : 32;


//potentially unused static ptr
#define STATIC_PTR(type) [[maybe_unused]] static type

#pragma endregion

#pragma endregion

#ifndef NONE
#define NONE                  -1
#endif

// Name of the section where we'll place executable data
//#define API_CODEDATA_SECTION_NAME   ".yelo"
// Apply this to data which will later contain code which will also be executed
//#define API_CODEDATA            __declspec(allocate(API_CODEDATA_SECTION_NAME))
// The fucking *compiler* ignores 'write' with 'execute. Needs manual fix-up
//#pragma section( API_CODEDATA_SECTION_NAME, read, write, execute )
// ...Actually, this fixes the problem of the compiler ignoring the 'write' attribute
// When compiled into object-code (.obj), the 'write' attribute is lost, so it's not really the linker's fault
//#pragma comment(linker, "/section:" API_CODEDATA_SECTION_NAME ",ERW")
#define FUNC_GET(funcName)       CurrentRuntime->getFunctionBegin(#funcName)
#endif

//TODO: Move to enums


#define __trueinline extern __forceinline

#define pad_bool  unsigned char : 8;

// # from 0 to 255
// # from -128 to 127
#define pad_byte PAD8

// # from 0 to 65,535
//typedef //unsigned  //short;
// # from -32,768 to 32,767
//typedef // signed //short   ;
#define pad_int16 unsigned short : 16;

// # from 0 to 4,294,967,295
#define pad_uint unsigned long : 32
// # from -2147483648 to 2147483647
//Gotta watch out for this sin...
//typedef //signed long long;
#define pad_int32 unsigned long : 32

// # from 0 to 18,446,744,073,709,551,615
typedef unsigned long long uint64;
// # from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
// typedef signed //long long  // i//nt64;
#define pad_int64 unsigned long : 32; unsigned long : 32

// a floating-point # from 1.175494351e-38F to 3.402823466e+38F
typedef float real;
#define pad_real unsigned long : 32

// Constant '\0' terminated ascii string
typedef const char    *cstring;
// '\0\0' terminated unicode string
typedef wchar_t       *wstring;
// Constant '\0\0' terminated unicode string
typedef const wchar_t *wcstring;

// null value for a [string] or [const char *]
#define CSTRING_NULL   "\0"
// null value for a [wstring] or [wstring]
#define WSTRING_NULL   L"\0"

// 32 character ASCII string, with null terminator
typedef char tag_string[32];
// 256 character ASCII string, with null terminator
typedef char long_string[256];

typedef char string_id_value[128];
typedef uint string_id;
// 128 character ASCII string, with null terminator
typedef char string128[128];
// 256 character ASCII string, with null terminator
typedef char string256[256];

// 64 character UNICODE string (128 bytes), with null terminator
typedef wchar_t       wstring64[64];
// 256 character UNICODE string (512 bytes), with null terminator
typedef wchar_t       wstring256[256];

// an enumerated value in a 1 byte range

typedef signed long    long_enum;
typedef unsigned short short_flags;
typedef unsigned long  long_flags;
typedef unsigned long  tag;

typedef unsigned int  uintptr_t;

typedef char          tag_string[32];
typedef const char    *cstring;
typedef char          *tag_reference_name_reference;

/// <summary>	The integral type used to represent game ticks. </summary>
typedef long  game_ticks_t;
/// <summary>	The integral type used to represent (relative) game timers. </summary>
/// <remarks>	Steps are in ticks. Generally used for countdowns </remarks>
typedef short game_time_t;

#define pad_tag_string unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32

#define pad_long_string PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128
#define pad_string_id unsigned long : 32
#define pad_byte_enum PAD8
// an enumerated value in a 2 byte range
#define pad_enum unsigned short : 16

// an enumerated value in a 4 byte range (not an officially used type in halo 1 tags, at least not up front anyway)
#define pad_long_enum unsigned long : 32

// bit flags in a 1 byte range
#define pad_byte_flags PAD8

// bit flags in a 2 byte range
#define pad_word_flags unsigned short : 16

// bit flags in a 4 byte range
#define pad_long_flags unsigned long : 32

namespace Yelo {
	struct string_list {
		unsigned long count;
		const char    **strings;

		const char **begin() const { return strings; }

		const char **begin() { return strings; }

		const char **end() const { return strings + count; }

		const char **end() { return strings + count; }

		const char *operator [](long index) const {
			// assert(index >= 0 && index < count);

			return strings[index];
		}
	};

	STAT_ASSERT(string_list, 0x8);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Class for defining an interface for blocks of data whose memory layout is not entirely mapped out.
	/// </summary>
	///
	/// <typeparam name="K_SIZE">	Size of the memory block. </typeparam>
	template <const size_t K_SIZE>
	struct TStruct {
		enum { k_size = K_SIZE };

	protected:
		// NOTE: I would use::std::array here, but I have yet to tested how well it plays with xbox modules (ie, Halo2_Xbox)
		byte m_data[K_SIZE];

		template <typename T, const size_t k_offset>
		T GetData() { return *(reinterpret_cast<T *>(&m_data[k_offset])); }

		template <typename T, const size_t k_offset>
		T GetData() const { return *(reinterpret_cast<const T *>(&m_data[k_offset])); }

		template <typename T, const size_t k_offset>
		T *GetDataPtr() { return reinterpret_cast<T *>(&m_data[k_offset]); }

		template <typename T, const size_t k_offset>
		const T *GetDataPtr() const { return reinterpret_cast<const T *>(&m_data[k_offset]); }

		// Usage - "struct s_some_object : TStructImpl(0x40) {};"
#define TStructImpl(size) public TStruct< size >

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-value getter. </summary>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the struct to treat as the get result. </param>
#define TStructGetImpl(type, name, offset) type Get##name() { return GetData<type, offset>(); } type Get##name() const \
      { return GetData<type, offset>(); } __cdecl( ( offset + sizeof( type )) <= k_size );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-address getter. </summary>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the struct to treat as the get result. </param>

		/*   ^ use const here, instead of before the type, in case [type] is defined as something like "long*" */
#define TStructGetPtrImpl(type, name, offset) type const* Get##name() const   { return GetDataPtr<type, offset>(); } __cdecl( ( offset + sizeof( type )) <= k_size );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-value getter for fake TStruct sub-classes. </summary>
		///
		/// <remarks>
		/// 	Requires an 'DATA_OFFSET' constant to be defined, representing the start of the fake sub-class.
		/// </remarks>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the fake sub-class to treat as the get result. </param>
#define TStructSubGetImpl(type, name, offset)      TStructGetImpl(type, name, offset - DATA_OFFSET)
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-address getter for fake TStruct sub-classes. </summary>
		///
		/// <remarks>
		/// 	Requires an 'DATA_OFFSET' constant to be defined, representing the start of the fake sub-class.
		/// </remarks>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the fake sub-class to treat as the get result. </param>
#define TStructSubGetPtrImpl(type, name, offset)   TStructGetPtrImpl(type, name, offset - DATA_OFFSET)
	};
};

/// Cast the pointer [ptr] to a pointer of type [type]
#define CAST_PTR(type, ptr)      (reinterpret_cast<type>(ptr))
#define CAST_PTR_OP(type)      reinterpret_cast<type>

#ifdef _MSC_VER
	#define naked  __declspec(naked)
#elif defined(__GNUC__)
	#define naked __attribute(naked)
#else
	#define naked
#endif

#define NOT_IMPLEMENTED \
	throw \
	"Not yet implemented!";
