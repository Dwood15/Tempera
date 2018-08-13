#pragma once

#include <cassert>
#include <enums/yelo_enums.h>
#include "enums/hs_enums.h"
#include "../memory/datum_index.h"
#include "../cseries/yelo_base.h"
#include "../memory/data_base.h"
#include "library.h"
#include "hs_value_union.h"
#include "../memory/array.h"

namespace Yelo::Scripting {
	struct hs_syntax_node;
	struct s_hs_thread_datum : Memory::s_datum_base {
		// 32bit packing only for stock code computability.
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"
//#pragma clang diagnostic pop

#define YELO_HS_RUNTIME_ASSERT(expression, thread, explanation)   //YELO_ASSERT_DISPLAY(expression, "a problem occurred while executing the script %s: %s (%s)",   thread->GetDescriptionString(), explanation, #expression)

#define YELO_HS_THREAD_VALID_STACK(thread) YELO_HS_RUNTIME_ASSERT(thread->ValidThread(), thread, "corrupted stack.")

#define YELO_HS_THREAD_SCRIPT_ERROR(expression, thread, explanation)   ( !(expression) && !(thread)->ScriptError(explanation, #expression) )

		struct hs_scenario_data_base {

			struct _script {
				Yelo::Enums::hs_script_type type;
				Yelo::Enums::hs_type        return_type;
				datum_index                 root_expression_index;
			};

			struct _global {
				Enums::hs_type type;
				unsigned short : 16;
				unsigned long  : 32;
				datum_index initialization_expression_index;
			};

			tag_string name;

			union {
				long    padded[15]; // 60
				_script script;
				_global global;
			};
		};

		struct hs_global_internal : hs_scenario_data_base {};
		static_assert(sizeof(hs_global_internal) == 0x5C);

		struct hs_script : hs_scenario_data_base {};
		STAT_ASSERT(hs_script, 0x5C)

		struct s_stack_frame {
			s_stack_frame    *previous;
			datum_index      source_expression;
			void *result_reference;
			unsigned short   size;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
			byte data[2];
#pragma warning( pop )

			void Initialize() {
				previous          = nullptr;
				source_expression = datum_index::null();
				result_reference  = nullptr;
				size              = 0;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Test whether the requested allocation would overflow. </summary>
			///
			/// <param name="stack_base_address">	The base address of the actual stack. </param>
			/// <param name="alignment">		 	The number of alignment bytes (if any) before [size]. </param>
			/// <param name="size">				 	The size of the allocation. </param>
			///
			/// <returns>	true if it allocation parameters would cause an overflow, false if they're good. </returns>
			bool AllocationWouldOverflow(const byte *stack_base_address, size_t size, size_t alignment) const {
				return (this->data + this->size + alignment + size) > (stack_base_address + Enums::k_hs_thread_stack_size);
			}
		};

		STAT_ASSERT(s_stack_frame, 0x10)

#ifdef __GNUC__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <PopPack.h>

#pragma clang diagnostic pop
#endif

		Yelo::Enums::hs_thread_type type;
		unsigned short              flags;
		short                       script_index; // only when type==_hs_thread_type_script
		game_ticks_t                sleep_until;
		game_ticks_t                prev_sleep_state; // when the thread is put to (sleep), this becomes sleep_until's old value, and then current value when (wake)'d
		s_stack_frame               *stack_frame;
		// NOTE: the only place this is used/referenced is in hs_runtime_evaluate. We can potentially use the bits
		// here for our own purposes if we don't wish to change the size of this struct
		::Yelo::Scripting::s_hs_value_union            result;

		byte stack_data[Enums::k_hs_thread_stack_size];

		void Initialize(Enums::hs_thread_type type);

		void InitializeForScript(s_hs_script_container_datum_index script);

		const hs_syntax_node *GetSyntax(datum_index expression_index) const {
			// TODO: whenever we support hsc containers, this will abstract which container's nodes are to be referenced
			return nullptr; //blam::hs_syntax_get(expression_index);
		}

		const hs_syntax_node *GetFrameSyntax() const {
			// assert(!StackIsEmpty());

			return GetSyntax(stack_frame->source_expression);
		}

		const hs_syntax_node *TryGetFrameSyntax() const { return !stack_frame->source_expression.IsNull() ? GetSyntax(stack_frame->source_expression) : nullptr; }

		const hs_syntax_node *TryGetPrevFrameSyntax() const {
			if (stack_frame->previous == nullptr)
				return nullptr;

			auto expression_index = stack_frame->previous->source_expression;
			if (expression_index.IsNull())
				return nullptr;

			return GetSyntax(expression_index);
		}

		bool ScriptError(const char *reason, const char *expression) const {
			// YELO_ERROR_FAILURE("script %s needs to be recompiled. (%s: %s)", GetDescriptionString(), reason != nullptr ? reason : "no reason given", expression);
			return false;
		}

		bool StackIsEmpty();

		bool NotKilledOrDormant();

		bool NotSleeping(game_ticks_t relative_to_game_time);

		bool NotSleeping();

		void Wake();

		bool ValidThread();

		const char *GetDescriptionString();

		void StackPush();

		void StackPop();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allocate memory from the thread's stack for script memory. </summary>
		///
		/// <param name="size">				The size of the allocation. </param>
		/// <param name="alignment_bit">	The alignment for the allocation. </param>
		/// <param name="stack_offset">
		/// 	[out] (Optional) If non-null, (Optional) the stack offset where the allocation begins.
		/// </param>
		///
		/// <returns>	null if it fails, else a pointer to the requested memory. </returns>
		// void *StackAllocate(size_t size, unsigned long alignment_bit = Flags::k_alignment_32bit, short *stack_offset = nullptr);
		//
		// template <typename T>
		// T *StackAllocate(size_t count = 1, unsigned long alignment_bit = Flags::k_alignment_32bit, short *stack_offset = nullptr);

		struct s_main_state {
			datum_index thread_index;
			const bool  &runtime_is_enabled;

			const hs_script *script;

			s_main_state(datum_index this_thread_index, const bool &runtime_globals_enabled) : thread_index(this_thread_index), runtime_is_enabled(runtime_globals_enabled), script(nullptr) {}
		};

		void MainPrologue(const s_main_state &state);

		void MainLoop(const s_main_state &state);

		void MainEpilogue(const s_main_state &state, bool &return_delete_thread);

		void Main(datum_index this_thread_index, const bool &runtime_globals_enabled, bool &return_delete_thread);

	};

	STAT_ASSERT(s_hs_thread_datum, 0x218)

	typedef Memory::DataArray<s_hs_thread_datum, 256> hs_thread_data_t;
};
