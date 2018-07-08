
#pragma once
#include <precompile.h>
#include "base.h"

namespace Yelo
{
	namespace Enums
	{
		enum error_message_priority : signed long
		{
			_error_message_priority_out_of_memory, // used when allocations fail in the engine
			_error_message_priority_warning,
			_error_message_priority_assert,
			_error_message_priority_critical,

			k_number_of_error_message_priorities
		};
	};

	namespace blam
	{
		typedef void (__cdecl* proc_error)(Enums::error_message_priority priority, const char * format, ...);

		extern proc_error error;

		bool __cdecl errors_handle();
	};
};

#ifndef YELO_NO_ERROR_MACRO
#define YELO_ERROR_IMPL(priority, format, ...) Yelo::blam::error(Yelo::Enums::priority, format, __VA_ARGS__)

#define YELO_WARN_IF(condition, format, ...) if(condition) { Yelo::blam::error(Yelo::Enums::_error_message_priority_warning, format, __VA_ARGS__); }
#define YELO_ERROR_IF(condition, format, ...) if(condition) { Yelo::blam::error(Yelo::Enums::_error_message_priority_assert, format, __VA_ARGS__); }
#else
#define YELO_ERROR_IMPL(priority, format, ...) __noop
#endif

#define YELO_ERROR_OUT_OF_MEMORY(format, ...)	YELO_ERROR_IMPL(_error_message_priority_out_of_memory, format, __VA_ARGS__)
#define YELO_WARN(format, ...)					YELO_ERROR_IMPL(_error_message_priority_warning, format, __VA_ARGS__)
#define YELO_ERROR_FAILURE(format, ...)			YELO_ERROR_IMPL(_error_message_priority_assert, format, __VA_ARGS__)
#define YELO_ERROR_CRITICAL(format, ...)		YELO_ERROR_IMPL(_error_message_priority_critical, format, __VA_ARGS__)
