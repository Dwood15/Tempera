#pragma once

#include "../memory/upgrades/blam_memory_upgrades.hpp"

namespace Yelo {
#define pad_bool  unsigned char : 8;

	// # from 0 to 255
	typedef unsigned char byte;
	// # from -128 to 127
	typedef signed char   sbyte;
#define pad_byte PAD8

	// # from 0 to 65,535
	//typedef //unsigned  //short;
	// # from -32,768 to 32,767
	//typedef // signed //short   ;
#define pad_int16 unsigned short : 16;

	// # from 0 to 4,294,967,295
	typedef unsigned long uint;
#define pad_uint unsigned long : 32
	// # from -2147483648 to 2147483647
	//Gotta watch out for this sin...
	//typedef //signed long long;
#define pad_int32 unsigned long : 32

	// # from 0 to 18,446,744,073,709,551,615
	typedef unsigned __int64 uint64;
	// # from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
	// typedef signed //__int64  // i//nt64;
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
	typedef char tag_string[31 + 1];
#define pad_tag_string unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32; unsigned long : 32
	// 256 character ASCII string, with null terminator
	typedef char long_string[Yelo::Enums::k_long_string_length + 1];
#define pad_long_string PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128

	typedef char string_id_value[Yelo::Enums::k_string_id_length + 1];
	typedef uint string_id;
#define pad_string_id unsigned long : 32

	// 128 character ASCII string, with null terminator
	typedef char string128[Yelo::Enums::k_string_128_length + 1];
	// 256 character ASCII string, with null terminator
	typedef char string256[Yelo::Enums::k_string_256_length + 1];

	// 64 character UNICODE string (128 bytes), with null terminator
	typedef wchar_t wstring64[Yelo::Enums::k_string_64_length + 1];
	// 256 character UNICODE string (512 bytes), with null terminator
	typedef wchar_t wstring256[Yelo::Enums::k_string_256_length + 1];

	struct string_list {
		long count;
		const char **strings;

		const char **begin() const { return strings; }

		const char **begin() { return strings; }

		const char **end() const { return strings + count; }

		const char **end() { return strings + count; }

		const char *operator [](long index) const {
			// assert(index >= 0 && index < count);

			return strings[index];
		}
	};

	static_assert(sizeof(string_list) == 0x8);

	// a tag group id
	typedef unsigned long tag;

	// an enumerated value in a 1 byte range
	typedef signed char byte_enum;
#define pad_byte_enum PAD8

	// an enumerated value in a 2 byte range
#define pad_enum unsigned short : 16

	// an enumerated value in a 4 byte range (not an officially used type in halo 1 tags, at least not up front anyway)
	typedef signed long long_enum;
#define pad_long_enum unsigned long : 32

	// bit flags in a 1 byte range
	typedef unsigned char byte_flags;
#define pad_byte_flags PAD8

	// bit flags in a 2 byte range
	typedef unsigned short short_flags;
#define pad_word_flags unsigned short : 16

	// bit flags in a 4 byte range
	typedef unsigned long long_flags;
#define pad_long_flags unsigned long : 32

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
		// NOTE: I would use std::array here, but I have yet to tested how well it plays with xbox modules (ie, Halo2_Xbox)
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
