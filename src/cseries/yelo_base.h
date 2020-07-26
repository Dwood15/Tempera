#pragma once

#include <cmath>
#include <cstring>
#include <string>
#include <time.h>
#include "macros_generic.h"
#include "../memory/datum_index.h"
#include "MacrosCpp.h"
#include <functional>

static const errno_t k_errnone = 0;

namespace Yelo {

	// System Initialize function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void              (__stdcall *proc_initialize)();

	// System Dispose function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void              (__stdcall *proc_dispose)();

	// System Initialize for new map function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void              (__stdcall *proc_initialize_for_new_map)();

	// System Dispose from old map function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void              (__stdcall *proc_dispose_from_old_map)();

	// System Initialize for bsp load function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void              (__stdcall *proc_initialize_for_new_bsp)();

	// System Dispose for bsp unload function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void              (__stdcall *proc_dispose_from_old_bsp)();

	// System Update function pointer
	typedef void              (__stdcall *proc_update)(float);

	// System Update function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void              (__stdcall *proc_update_no_args)();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Describes the number of bits needed in a bitfield for a whole number. </summary>
	///
	/// <typeparam name="max_value">	The inclusive max value of the bit field. </typeparam>
	template<int max>
	constexpr unsigned short bitfield_size() {
		return::std::log2(max) + 1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Describes the number of bits needed in a bitfield for an enum value. </summary>
	///
	/// <typeparam name="number_of_members">	The number of members in the enum represented by this bitfield. </typeparam>
	template <int num_members>
	constexpr unsigned short bitfield_enum_size() {
		return::std::log2(num_members) + 1;
	}

	// TODO: refactor these to PascalCase

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether an ASCII string is NULL or begins with a null terminator </summary>
	static inline bool is_null_or_empty(const char* const str) {
		return str == nullptr || str[0] == '\0';
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether an ASCII string buffer begins with a null terminator </summary>
	template <size_t kLength>
	static inline bool is_null_or_empty(const char (&array)[kLength]) {
		return array[0] == '\0';
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether a wide string is NULL or begins with a null terminator </summary>
	static inline bool is_null_or_empty(const wchar_t * str) {
		return str == nullptr || str[0] == L'\0';
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether a wide string buffer begins with a null terminator </summary>
	template <size_t kLength>
	static inline bool is_null_or_empty(const wchar_t (&array)[kLength]) {
		return array[0] == L'\0';
	}

	// Takes [wide] and converts it to an ascii string, to be held in [string]. If [wide_length] is not -1, the string
	// is assumed to be null terminated
	// Returns [string] if successful
	// If NULL is returned, you can use GetLastError() for error information
	// static char *wstring_to_string(char *string, long string_length, wstring wide, long wide_length = -1) {
	// 	if (!WIN32_FUNC(WideCharToMultiByte)(CP_ACP, 0, wide, wide_length, string, string_length, nullptr, nullptr))
	// 		return nullptr;
	// 	else
	// 		return string;
	// }

	// [string_length] includes the null terminator
	static char *wstring_to_string_lazy(char *string, long string_length, wchar_t * wide) {

		string[--string_length] = '\0';
		for (long x = 0; string_length--; x++) {
			string[x] = CAST(char, wide[x]);
			if (wide[x] == L'\0') break;
		}

		return string;
	}

	// Takes [string] and converts it to an unicode string, to be held in [wide]. If [string_length] is not -1, the string
	// is assumed to be null terminated
	// Returns [wide] if successful
	// If NULL is returned, you can use GetLastError() for error information
	// static wstring string_to_wstring(wstring wide, long wide_length, const char * string, long string_length = -1) {
	// 	if (!WIN32_FUNC(MultiByteToWideChar)(CP_ACP, 0, string, string_length, wide, wide_length))
	// 		return nullptr;
	// 	else
	// 		return wide;
	// }

	// [string_length] includes the null terminator
	static wstring string_to_wstring_lazy(wchar_t * string, long string_length, const char * ascii) {

		string[--string_length] = L'\0';
		for (long x = 0; string_length--; x++) {
			string[x] = CAST(wchar_t, ascii[x]);
			if (ascii[x] == '\0') break;
		}

		return string;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Determines whether the end of the string instance matches the specified suffix string. </summary>
	///
	/// <param name="str">   	The string in question. </param>
	/// <param name="suffix">	The string to compare to the substring at the end of this instance. </param>
	///
	/// <returns>	true if suffix matches the end of the instance; otherwise, false. </returns>
	static bool EndsWith(const::std::string &str, const::std::string &suffix) {
		// based on http://stackoverflow.com/a/20447331/444977

		if (suffix.length() > str.length())
			return false;

		auto pos   = str.length() - suffix.length();
		auto pos_a = &str[pos];
		auto pos_b = &suffix[0];
		while (*pos_a != '\0')
			if (*pos_a++ != *pos_b++)
				return false;

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Allows us to interpret a machine word (in this case, 32-bits) as various sorts of types. Either as a pointer,
	/// 	a value-array, or just vanilla values.
	/// 	It's a shitty struct name, but fuck, this has been around almost as-is since...2006 or earlier.
	/// </summary>
	///
	/// <typeparam name="T">	. </typeparam>
	template <typename T>
	struct TTypeHolder {
		union { // t_type_interface_union
			T* pointer;

			union {// t_type_interface_pointers
				bool* boolean;
				char* character;
				byte* byte;
				sbyte* sbyte;
				ushort* uint16;
				short* int16;
				uint* uint32;
				int* int32;
				real* real;

				datum_index* datum;

				cstring ascii;
				wstring unicode;
			}ptr;

			union {// t_type_interface_arrays
				bool boolean[1];
				char character[1];
				byte byte[1];
				sbyte sbyte[1];
				ushort uint16[1];
				short int16[1];
				uint uint32[1];
				int int32[1];
				real real[1];

				datum_index datum[1];

				cstring ascii[1];
				wstring unicode[1];
			}array;

			bool boolean;
			char character;
			byte byte;
			sbyte sbyte;
			ushort uint16;
			short int16;
			uint uint32;
			int int32;
			real real;

			datum_index datum;
		};

		// hope you know what you're doing if you use this!
		bool IsNull() const { return pointer == nullptr; }

		// default equality logic is to compare the pointer (32) bits.
		// could be problematic for value types <32bits like bool, where the upper bits aren't zeroed.
		// at which point you should be doing value.boolean == ... anyway.
		inline bool operator ==(const TTypeHolder& rhs) const { return this->pointer == rhs.pointer; }
		inline bool operator !=(const TTypeHolder& rhs) const { return this->pointer != rhs.pointer; }
	};

	typedef TTypeHolder<void> TypeHolder;
	static_assert(sizeof(TypeHolder) == 0x4);

	static const TypeHolder k_null_as_type_holder = {{reinterpret_cast<void *>(0)}};  ///< nullptr represented as a TypeHolder value
	static const TypeHolder k_none_as_type_holder = {{reinterpret_cast<void *>(-1)}}; ///< NONE represented as TypeHolder val

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	If the COM interface reference isn't NULL, releases it and NULL it. </summary>
	///
	/// <tparam name="TInterface">	COM interface type. </tparam>
	/// <param name="obj">	[in,out] If non-null, the COM object. </param>
	template <typename TInterface>
	inline
	void safe_release(TInterface *&obj) // TODO: refactor to SafeRelease
	{
		if (obj != nullptr) {
			obj->Release();
			obj = nullptr;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Class for automatically releasing a COM object when it goes out of scope. </summary>
	///
	/// <tparam name="TInterface">	COM interface type. </tparam>
	template <typename TInterface>
	class c_auto_release final {
		TInterface *m_target_object;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="target_object">	[in] If non-null, the target object. </param>
		c_auto_release(TInterface *target_object)
			: m_target_object(target_object) {}

		/// <summary>	Default constructor. </summary>
		c_auto_release()
			: m_target_object(nullptr) {}

		/// <summary>	Destructor. </summary>
		~c_auto_release() {
			if (m_target_object) {
				m_target_object->Release();
			}
			m_target_object = nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator. Sets the raw value to the provided value. </summary>
		///
		/// <param name="value">	The value to set. </param>
		///
		/// <returns>	The current value. </returns>
		TInterface *&operator =(const TInterface *&value) {
			m_target_object = value;

			return m_target_object;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Member reference operator. </summary>
		///
		/// <returns>	The referenced member. </returns>
		TInterface **operator &() {
			return &m_target_object;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Member dereference operator. </summary>
		///
		/// <returns>	The dereferenced object. </returns>
		TInterface *&operator ->() {
			return m_target_object;
		}

		/// <summary>	Support for casting the object to the templated type. </summary>
		operator TInterface *&() {
			return m_target_object;
		}
	};

	/// <summary>	Class for executing a function at the end of a scope. </summary>
	class c_scope_end_execute {
		/// <summary>	Defines an alias representing the end execute function. </summary>
		typedef::std::function<void()> end_execute_func_t;

		const end_execute_func_t m_function;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="function">	The function to run. </param>
		c_scope_end_execute(const end_execute_func_t &function)
			: m_function(function) {}

		/// <summary>	Destructor. </summary>
		~c_scope_end_execute() {
			m_function();
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	If the object pointer reference isn't NULL, deletes it and NULL it. </summary>
	///
	/// <param name="obj">	[in,out] The object to delete. NULL on return </param>
	template <typename T>
	inline
	void SafeDelete(T *&obj) {
		if (obj != nullptr) {
			delete obj;
			obj = nullptr;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	If the object pointer reference isn't NULL, deletes it and NULL it. </summary>
	///
	/// <param name="obj">	[in,out] The object to delete. NULL on return </param>
	template <typename T>
	inline
	void SafeDeleteArray(T *&obj) {
		if (obj != nullptr) {
			delete[] obj;
			obj = nullptr;
		}
	}

	// Returns INVALID_HANDLE_VALUE as nullptr, else returns [h]
	// Should be used in conjunction with::std::unique_ptr and winapi_handle_closer
	// inline HANDLE ToSafeHandle(HANDLE h) {
	// 	return h == INVALID_HANDLE_VALUE ? nullptr : h;
	// }

	/// <summary>	Primarily a deleter for::std::unique_ptr for use with WinAPI handles. </summary>
	// struct winapi_handle_closer {
	// 	void operator ()(HANDLE h) const {
	// 		// YELO_ASSERT_DISPLAY(h != INVALID_HANDLE_VALUE, "tried to close an INVALID handle");
	// 		if (h != nullptr)
	// 			WIN32_FUNC(CloseHandle)(h);
	// 	}
	// };

	/// <summary>	Primarily a deleter for::std::unique_ptr for objects allocated by LocalAlloc. </summary>
	// struct winapi_local_deleter {
	// 	void operator ()(HLOCAL h) const {
	// 		// YELO_ASSERT_DISPLAY(h != INVALID_HANDLE_VALUE, "tried to close an INVALID handle");
	// 		if (h != nullptr)
	// 			LocalFree(h);
	// 	}
	// };

	/// <summary>	Primarily a deleter for::std::unique_ptr for use CRT's FILE. </summary>
	struct crt_file_closer {
		void operator ()(FILE *h) const {
			if (h != nullptr) {
				fclose(h);
				// YELO_ASSERT_DISPLAY(result == 0, "failed to fclose");
			}
		}
	};

	static const char * BooleanToString(bool value) {
		return value ? "true" : "false";
	}

	typedef int (__cdecl *proc_stdlib_compare)(void *, const void *, const void *);

	template <typename T, size_t k_array_size, typename TContext = void *>
	inline void Qsort(T (&_Base)[k_array_size], int (__cdecl *_PtFuncCompare)(TContext, const T *, const T *), TContext _Context = nullptr) {
		static_assert(sizeof(TContext) <= sizeof(void *), "Context type is incompatible with C runtime API");

		::qsort_s(_Base, k_array_size, sizeof(T), reinterpret_cast<proc_stdlib_compare>(_PtFuncCompare),
					 reinterpret_cast<void *>(_Context));
	}

	template <typename T, typename TContext = void *, typename TCompareParam = const T *>
	inline void Qsort(T *_Base, rsize_t _NumOfElements, int (__cdecl *_PtFuncCompare)(TContext, TCompareParam, TCompareParam), TContext _Context = nullptr) {
		static_assert(sizeof(TContext) <= sizeof(void *), "Context type is incompatible with C runtime API");

		::qsort_s(_Base, _NumOfElements, sizeof(T), reinterpret_cast<proc_stdlib_compare>(_PtFuncCompare), reinterpret_cast<void *>(_Context));
	}

	// template <typename TKey, typename T, size_t k_array_size, typename TContext = void *>
	// inline
	// T *Bsearch(const TKey *_Key, T (&_Base)[k_array_size],
	// 			  int (__cdecl *_PtFuncCompare)(TContext, const TKey *, const T *), TContext _Context = nullptr) {
	// 	static_assert(sizeof(TContext) <= sizeof(void *), "Context type is incompatible with C runtime API");
	//
	// 	return reinterpret_cast<T *>(::bsearch_s(_Key, _Base, k_array_size, sizeof(T), reinterpret_cast<proc_stdlib_compare>(_PtFuncCompare), reinterpret_cast<void *>(_Context)));
	// }
	//
	// template <typename TKey, typename T, typename TContext = void *, typename TCompareParam = const T *>
	// inline T *Bsearch(const TKey *_Key, T *_Base, rsize_t _NumOfElements, int (__cdecl *_PtFuncCompare)(TContext, const TKey *, TCompareParam), TContext _Context = nullptr) {
	// 	static_assert(sizeof(TContext) <= sizeof(void *), "Context type is incompatible with C runtime API");
	//
	// 	return reinterpret_cast<T *>(::bsearch_s(_Key, _Base, _NumOfElements, sizeof(T), reinterpret_cast<proc_stdlib_compare>(_PtFuncCompare), reinterpret_cast<void *>(_Context)));
	// }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether an array is bitwise zero. </summary>
	///
	/// <param name="array">	Array to test. </param>
	/// <param name="index"> 	Element index in array to start testing at. </param>
	/// 
	///  <returns>	true if the array elements are equal to bitwise zero, false if one or more aren't. </returns>
	template <typename T, size_t kLength>
	inline
	bool ArrayIsZero(T (&array)[kLength], size_t index = 0) {
		T           zero = T();
		for (size_t x    = index; x < kLength; x++) {
			if (memcmp(&array[x], &zero, sizeof(zero)) != 0) {
				return false;
			}
		}

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Clear an array to be bitwise zero. </summary>
	///
	/// <param name="array">	Array to zero. </param>
	/// <param name="index"> 	Element index in array to start zeroing at. </param>
	template <typename T, size_t kLength>
	inline
	void ArrayZero(T (&array)[kLength], size_t index = 0) {
		memset(&array[index], 0, sizeof(T) * (kLength - index));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copy a range of elements from one array to another. </summary>
	///
	/// <param name="dst">			Destination array.</param>
	/// <param name="dst_index">	Element index in dst to start the copy at. </param>
	/// <param name="src">			Source array. </param>
	/// <param name="src_index">	Element index in src to start the copy from. </param>
	/// <param name="count">		The number of elements to copy from src to dst. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	template <typename T, size_t kDstLength, size_t kSrcLength>
	inline
	bool ArrayCopy(T (&dst)[kDstLength], size_t dst_index, const T (&src)[kSrcLength], size_t src_index, size_t count = kSrcLength) {
		return memcpy_s(&dst[dst_index], sizeof(T) * (kDstLength - dst_index),
							 &src[src_index], sizeof(T) * (count - src_index)) == k_errnone;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copy a range of elements from one array to another. </summary>
	///
	/// <param name="dst"> 		Destination array. </param>
	/// <param name="src"> 		Source array </param>
	/// <param name="count">	The number of elements to copy from src to dst. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	template <typename T, size_t kDstLength, size_t kSrcLength>
	inline
	bool ArrayCopy(T (&dst)[kDstLength], const T (&src)[kSrcLength], size_t count = kSrcLength) {
		return ArrayCopy(dst, 0, src, 0, count);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copy a range of elements from one array to another. </summary>
	///
	/// <param name="dst">			Destination array.</param>
	/// <param name="dst_index">	Element index in dst to start the copy at. </param>
	/// <param name="src">			Pointer to the first source element.  </param>
	/// <param name="src_index">	Element index in src to start the copy from. </param>
	/// <param name="count">		The number of elements to copy from src to dst. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	template <typename T, size_t kDstLength>
	inline
	bool ArrayCopyPtr(T (&dst)[kDstLength], size_t dst_index, const T *src, size_t src_index, size_t count) {
		return memcpy_s(&dst[dst_index], sizeof(T) * (kDstLength - dst_index), &src[src_index], sizeof(T) * count) == k_errnone;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copy a range of elements from one array to another. </summary>
	///
	/// <param name="dst">			Destination array.</param>
	/// <param name="src">			Pointer to the first source element.  </param>
	/// <param name="count">		The number of elements to copy from src to dst. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	template <typename T, size_t kDstLength>
	inline
	bool ArrayCopyPtr(T (&dst)[kDstLength], const T *src, size_t count) {
		return ArrayCopyPtr(dst, 0, src, 0, count);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the current time and format it into [time_str]. </summary>
	///
	/// <param name="time_str">	The resulting time string. </param>
	static void GetTimeStampString(tag_string &time_str) {
		const size_t k_time_str_sizeof = sizeof(tag_string);

		memset(time_str, 0, k_time_str_sizeof);

		tm     newtime;
		time_t aclock;
		time(&aclock); // Get time in seconds
		localtime_s(&newtime, &aclock); // Convert time to struct tm form
		asctime_s(time_str, k_time_str_sizeof, &newtime);
		time_str[25] = '\0'; // remove the \n character that asctime adds (http://msdn.microsoft.com/en-us/library/b6htak9c.aspx)
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the current time and format it into [time_str] using a file name friendly format. </summary>
	///
	/// <remarks>	The formatting output by this function is YYYY_MM_DD_hh_mm_ss. </remarks>
	///
	/// <param name="time_str">	The resulting time string. </param>
	static void GetTimeStampStringForFile(tag_string &time_str) {
		const size_t k_time_str_sizeof = sizeof(tag_string);

		memset(time_str, 0, k_time_str_sizeof);

		tm     newtime;
		time_t aclock;
		time(&aclock); // Get time in seconds
		localtime_s(&newtime, &aclock); // Convert time to struct tm form

		sprintf_s(time_str, k_time_str_sizeof, "%04d_%02d_%02d_%02d_%02d_%02d",
					 newtime.tm_year + 1900,
					 newtime.tm_mon + 1,
					 newtime.tm_mday,
					 newtime.tm_hour,
					 newtime.tm_min,
					 newtime.tm_sec);
	}

	// Displays a message to the user using the WinAPI
	// Use this when are probably about to get really messy...
	static void PrepareToDropError(const char * text) {
		if (text == nullptr) text = "(null)";
		throw "Gonna die";
		// ::MessageBoxA(nullptr, text, "Prepare to Drop!");
	}

	namespace blam {
		static char g_display_assert_buffer[512];
	};
};
