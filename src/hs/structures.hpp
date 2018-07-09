#pragma once

#include "../cseries/base.h"
#include "hs.h"
#include "../memory/upgrades/blam_memory_upgrades.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum {
			_hs_global_index_is_external_mask = FLAG(15),
			_hs_global_index_mask = _hs_global_index_is_external_mask - 1,
		};

		static_assert(0x8000 == _hs_global_index_is_external_mask);
		static_assert(0x7FFF == _hs_global_index_mask);
	};

	namespace Flags
	{
		enum hs_syntax_node_flags : unsigned short
		{
			_hs_syntax_node_primitive_bit,
			_hs_syntax_node_script_index_bit,
			_hs_syntax_node_global_index_bit,
			_hs_syntax_node_dont_gc_bit,

			/// <summary>	node is an index to a script parameter. </summary>
			_hs_syntax_node_script_parameter_index_yelo_bit,
			/// <summary>	node is an index to a script's local variable. </summary>
			_hs_syntax_node_script_local_index_yelo_bit,
			/// <summary>	node is an index to a constant. </summary>
			_hs_syntax_node_const_index_yelo_bit,
		};

		enum hs_access_flags : unsigned short
		{
			_hs_access_enabled_bit,
			_hs_access_sent_to_server_bit,	///< automatically sent to server
			_hs_access_rconable_bit,		///< can be executed from an rcon command
			_hs_access_client_bit,			///< only a client connection can execute it
			_hs_access_server_bit,			///< only a server connection can execute it
		};
	};

	namespace Scripting
	{
		typedef void (__cdecl* proc_hs_parse)(long function_index, datum_index expression_index);
		typedef void (__cdecl* proc_hs_evaluate)(long function_index, datum_index thread_index, bool initialize_stack);

		// halo script function definition
		struct hs_function_definition
		{
			Yelo::Enums::hs_type return_type;
			unsigned short yelo_flags; // padding in halo, special flags in project yellow
			const char * name;
			proc_hs_parse parse;
			proc_hs_evaluate evaluate;
			const char * info;
			const char * param_info;
			unsigned short access;
			short paramc;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
			// don't access directly, use GetParameter
			Enums::hs_type params[];
#pragma warning( pop )

			Enums::hs_type GetParameter(short index)
			{
				assert(index >= 0 && index<paramc);

				return params[index];
			}

			//////////////////////////////////////////////////////////////////////////
			// STL-like APIs
			typedef Enums::hs_type*			iterator;
			typedef const Enums::hs_type*	const_iterator;

			const_iterator	begin() const		{ return params; }
			iterator		begin()				{ return params; }
			const_iterator	const_begin() const	{ return params; }
			const_iterator	const_begin()		{ return params; }
			const_iterator	end() const			{ return params + paramc; }
			iterator		end()				{ return params + paramc; }
			const_iterator	const_end() const	{ return params + paramc; }
			const_iterator	const_end()			{ return params + paramc; }

		}; static_assert( sizeof(hs_function_definition) == 0x1C ); // size doesn't include [params]

		// halo script accessible value
		struct hs_global_definition
		{
			const char * name;
			Enums::hs_type type;
			unsigned short flags; // padding in halo, special flags in project yellow
			union {
				void* address;

				s_hs_value_union value;
			};
			unsigned short access;
			unsigned short : 16;
		}; static_assert( sizeof(hs_global_definition) == 0x10 );


		struct hs_syntax_node : Memory::s_datum_base
		{
			union {
				Enums::hs_type constant_type;
				short function_index;
				short script_index;
			};
			Enums::hs_type type;
			union {
				unsigned short flags;
				short pointer_type;
			};
			datum_index next_expression;
			long source_offset;
			union {
				void* address;

				s_hs_value_union value;
			};

			bool IsScriptCall() const
			{
				return TEST_FLAG(flags, Flags::_hs_syntax_node_script_index_bit);
			}
			bool IsGarbageCollectable() const
			{
				return !TEST_FLAG(flags, Flags::_hs_syntax_node_dont_gc_bit);
			}
		}; static_assert( sizeof(hs_syntax_node) == 0x14 );
	};
};
