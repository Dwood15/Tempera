#pragma once


#include "../memory/datum_index.h"
#include "scenario.hpp"
#include "../cseries/yelo_base.h"
#include "hs.h"
#include "../memory/data_base.h"
#include "../game/configuration.h"
#include "../cseries/errors.h"
#include "library.h"

#define YELO_HS_RUNTIME_ASSERT(expression, thread, explanation)   YELO_ASSERT_DISPLAY(expression, "a problem occurred while executing the script %s: %s (%s)",   thread->GetDescriptionString(), explanation, #expression)

#define YELO_HS_THREAD_VALID_STACK(thread) YELO_HS_RUNTIME_ASSERT(thread->ValidThread(), thread, "corrupted stack.")

#define YELO_HS_THREAD_SCRIPT_ERROR(expression, thread, explanation)   ( !(expression) && !(thread)->ScriptError(explanation, #expression) )

namespace Yelo::Enums {
	enum {
		k_hs_thread_stack_size         = 0x200,

		_hs_thread_datum_sleep_forever = NONE,
		_hs_thread_datum_sleep_dormant = NONE - 1,
	};

	enum hs_thread_type : short {
		_hs_thread_type_script,
		_hs_thread_type_global_initialize, // script global initializer thunk
		_hs_thread_type_runtime_evaluate, // console_command in stock engine

		k_number_of_hs_thread_types,
	};
};

namespace Yelo::Flags {
	enum {
		_hs_thread_datum_has_new_stack_frame_bit,
		_hs_thread_datum_sleeping_bit,
	};
};

namespace Yelo::Scripting {
	struct hs_syntax_node;

	struct s_hs_thread_datum : Memory::s_datum_base {
		// 32bit packing only for stock code computability.
#include <PshPack4.h>

		struct s_stack_frame {
			s_stack_frame    *previous;
			datum_index      source_expression;
			s_hs_value_union *result_reference;
			unsigned short           size;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
			byte data[];
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

		static_assert(sizeof(s_stack_frame) == 0x10);

#include <PopPack.h>

		Enums::hs_thread_type type;
		unsigned short            flags;
		short                 script_index; // only when type==_hs_thread_type_script
		game_ticks_t          sleep_until;
		game_ticks_t          prev_sleep_state; // when the thread is put to (sleep), this becomes sleep_until's old value, and then current value when (wake)'d
		s_stack_frame         *stack_frame;
		// NOTE: the only place this is used/referenced is in hs_runtime_evaluate. We can potentially use the bits
		// here for our own purposes if we don't wish to change the size of this struct
		s_hs_value_union      result;

		byte stack_data[Enums::k_hs_thread_stack_size];

		void Initialize(Enums::hs_thread_type type) {
			this->type         = type;
			this->flags        = 0;
			this->script_index = NONE;
			this->sleep_until  = this->prev_sleep_state = 0;

			this->stack_frame = reinterpret_cast<s_stack_frame *>(this->stack_data);
			this->stack_frame->Initialize();

			result = k_none_value_union;
		}

		void InitializeForScript(hs_script_index_t script) {
			// TODO: update code when containers come online

			auto       *scenario          = blam::global_scenario_get();
			const auto &script_definition = scenario->scripts[script.script_index].script;

			this->script_index = script.script_index;

			if (script_definition.type == Enums::_hs_script_dormant)
				this->sleep_until = Enums::_hs_thread_datum_sleep_dormant;
			else
				this->sleep_until = 0;
		}

		const hs_syntax_node *GetSyntax(datum_index expression_index) const {
			// TODO: whenever we support hsc containers, this will abstract which container's nodes are to be referenced
			return blam::hs_syntax_get(expression_index);
		}

		const hs_syntax_node *GetFrameSyntax() const {
			assert(!StackIsEmpty());

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

		bool ScriptError(const char * reason, const char * expression) const {
			YELO_ERROR_FAILURE("script %s needs to be recompiled. (%s: %s)", GetDescriptionString(), reason != nullptr ? reason : "no reason given", expression);
			return false;
		}

		bool StackIsEmpty() const { return reinterpret_cast<const void *>(stack_frame) == stack_data; }

		bool NotKilledOrDormant() const { return sleep_until > Enums::_hs_thread_datum_sleep_forever; }

		bool NotSleeping(game_ticks_t relative_to_game_time) const { return sleep_until <= relative_to_game_time; }

		bool NotSleeping() const { return NotSleeping(blam::game_time_get()); }

		void Wake() {
			if (sleep_until == Enums::_hs_thread_datum_sleep_forever)
				return;

			sleep_until = 0;

			if (TEST_FLAG(flags, Flags::_hs_thread_datum_sleeping_bit)) {
				SET_FLAG(flags, Flags::_hs_thread_datum_sleeping_bit, false);
				sleep_until = prev_sleep_state;
				return;
			}

			auto *call_expression = TryGetFrameSyntax();
			if (call_expression != nullptr &&
				 call_expression->function_index == Enums::_hs_function_sleep_until) {
				StackPop();
				return;
			}

			// IDK what case this is suppose to handle
			call_expression = TryGetPrevFrameSyntax();
			if (call_expression != nullptr &&
				 call_expression->function_index == Enums::_hs_function_sleep_until) {
				StackPop();
				StackPop();
				SET_FLAG(flags, Flags::_hs_thread_datum_has_new_stack_frame_bit, false);
			}
		}

		bool ValidThread() const {
			const Memory::s_data_array &threads = HsThreads().Header;

			const byte *base_address = reinterpret_cast<const byte *>(threads.base_address);

			// validate the thread is within the bounds of the data array
			if (CAST(const void*, this) < base_address)
				return false;
			else if (CAST(const void*, this) > base_address + (threads.last_datum * threads.datum_size))
				return false;

				// validate the stack pointer is within the thread's stack buffer
			else if (CAST(const void*, stack_frame) < &stack_data[0])
				return false;
			else if (CAST(const void*, stack_frame) > this + 1)
				return false;

				// validate the stack hasn't overflowed
			else if (stack_frame->data + stack_frame->size > CAST(const void*, this + 1))
				return false;

			return true;
		}

		const char * GetDescriptionString() const {
			switch (type) {
				case Enums::_hs_thread_type_script:
					return Scenario::Scenario()->scripts[script_index].name;

				case Enums::_hs_thread_type_global_initialize:
					return "[global initialize]";

				case Enums::_hs_thread_type_runtime_evaluate:
					return "[runtime evaluate]";

				default:
					return "[unknown thread type]";
			}
		}

		void StackPush() {
			auto *new_frame = stack_frame + 1;
			//YELO_HS_RUNTIME_ASSERT(CAST_PTR(byte*, new_frame + 1) < stack_data + Enums::k_hs_thread_stack_size, this, "stack overflow.");
			(void) ((!!(reinterpret_cast<byte *>(new_frame + 1) < stack_data + Enums::k_hs_thread_stack_size)) ||
					  (_wassert(L"CAST_PTR(byte * , new_frame + 1) < stack_data + Enums::k_hs_thread_stack_size", L"_file_name_", (unsigned) (231)), 0));

			new_frame->previous = stack_frame;
			new_frame->size     = 0;
			this->stack_frame   = new_frame;
		}

		void StackPop() { stack_frame = stack_frame->previous; }

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
		void *StackAllocate(size_t size, unsigned long alignment_bit = Flags::k_alignment_32bit, _Out_opt_ short *stack_offset = nullptr) {
			YELO_HS_THREAD_VALID_STACK(this);
			YELO_HS_RUNTIME_ASSERT(size, this, "attempt to allocate zero space from the stack.");

			// TODO: calc alignment of stack
			size_t alignment = 0;

			YELO_HS_RUNTIME_ASSERT(stack_frame->AllocationWouldOverflow(stack_data, size, alignment), this, "stack overflow.");

			// TODO: ZeroMemory alignment bytes

			// TODO: populate the start offset
			if (stack_offset)
				*stack_offset = NONE;

			return nullptr;
		}

		template <typename T>
		T *StackAllocate(size_t count = 1, unsigned long alignment_bit = Flags::k_alignment_32bit, _Out_opt_ short *stack_offset = nullptr) {
			return reinterpret_cast<T *>(StackAllocate(sizeof(T) * count, alignment_bit, stack_offset));
		}

		struct s_main_state {
			datum_index thread_index;
			const bool  &runtime_is_enabled;

			const TagGroups::hs_script *script;

			s_main_state(datum_index this_thread_index, const bool &runtime_globals_enabled) : thread_index(this_thread_index), runtime_is_enabled(runtime_globals_enabled), script(nullptr) {}
		};

		void MainPrologue(const s_main_state &state) {
			this->sleep_until = 0;

			if (this->StackIsEmpty()) {
				assert(state.script);

				this->stack_frame->size = 0;
				auto *result = this->StackAllocate<s_hs_value_union>();

				blam::hs_evaluate(state.thread_index, state.script->script.root_expression_index, result);
			}
		}

		void MainLoop(const s_main_state &state) {
			if (!this->StackIsEmpty()) {
				while (this->NotKilledOrDormant() &&
						 (!blam::game_in_progress() || this->NotSleeping()) &&
						 state.runtime_is_enabled) {
					blam::hs_call_evaluate(state.thread_index);
				}
			}
		}

		void MainEpilogue(const s_main_state &state, _Out_opt_ bool &return_delete_thread) {
			if (this->StackIsEmpty()) {
				if (this->type == Enums::_hs_thread_type_script) {
					const auto &_script = state.script->script;

					if (_script.type == Enums::_hs_script_startup || _script.type == Enums::_hs_script_dormant)
						this->sleep_until = Enums::_hs_thread_datum_sleep_forever;
				} else if (this->type == Enums::_hs_thread_type_runtime_evaluate) {
					return_delete_thread = true;
				}
			}
		}

		void Main(datum_index this_thread_index, const bool &runtime_globals_enabled, _Out_opt_ bool &return_delete_thread) {
			s_main_state state(this_thread_index, runtime_globals_enabled);
			return_delete_thread = false;

			if (this->type == Enums::_hs_thread_type_script) {
				state.script = &(blam::global_scenario_get()->scripts[this->script_index]);

				const auto &_script = state.script->script;
				YELO_HS_RUNTIME_ASSERT(_script.type != Enums::_hs_script_static && _script.type != Enums::_hs_script_stub, this, "found a static script at toplevel.");
			}

			YELO_HS_THREAD_VALID_STACK(this);

			MainPrologue(state);
			MainLoop(state);
			MainEpilogue(state, return_delete_thread);
		}

	};

	static_assert(sizeof(s_hs_thread_datum) == 0x218);
};
