#pragma once

#include <cstdlib>
#include "macros_generic.h"

namespace Yelo::Scripting {
		struct s_hs_compile_globals {
			bool initialized;
			unsigned char : 8; unsigned short : 16;
			rsize_t compiled_source_size;
			char* compiled_source;

			struct {
				size_t offset;
				char* base_address;
				rsize_t size;
			}string_constant;

			bool has_compiled_source; // a source file has been compiled
			unsigned char : 8; unsigned short : 16;

			const char * error; // is either null, points to error_buffer, or constant (non-DMA) string
			size_t error_offset;
			string256 error_buffer;

			bool compiled_source_is_dma; // compiled_source is dynamically allocated
			bool compiled_for_runtime_evaluate;
			bool cant_compile_thread_blocking; // in the current context, thread blocking statements can't be used
			bool cant_compile_variable_sets; // in the current context, the 'set' function can't be used
			bool postprocessing;
			unsigned char : 8; unsigned short : 16;
		};

		static_assert( sizeof(s_hs_compile_globals) == 0x12C );


		s_hs_compile_globals* HsCompileGlobals();
	};
