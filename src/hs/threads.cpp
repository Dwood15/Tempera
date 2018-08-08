#include "../cseries/yelo_base.h"
#include "threads.h"
#include "../game/time/time.hpp"
namespace Yelo::Scripting {

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
	// void * s_hs_thread_datum::StackAllocate(size_t size, unsigned long alignment_bit = Flags::k_alignment_32bit, short *stack_offset = nullptr) {
	// 	//YELO_HS_THREAD_VALID_STACK(this);
	// 	//YELO_HS_RUNTIME_ASSERT(size, this, "attempt to allocate zero space from the stack.");
	//
	// 	// TODO: calc alignment of stack
	// 	size_t alignment = 0;
	//
	// 	//YELO_HS_RUNTIME_ASSERT(stack_frame->AllocationWouldOverflow(stack_data, size, alignment), this, "stack overflow.");
	//
	// 	// TODO: ZeroMemory alignment bytes
	//
	// 	// TODO: populate the start offset
	// 	if (stack_offset)
	// 		*stack_offset = NONE;
	//
	// 	return nullptr;
	// }

	bool s_hs_thread_datum::StackIsEmpty() { return reinterpret_cast<const void *>(stack_frame) == stack_data; }

	bool s_hs_thread_datum::NotKilledOrDormant() { return sleep_until > Enums::_hs_thread_datum_sleep_forever; }

	bool s_hs_thread_datum::NotSleeping(game_ticks_t relative_to_game_time) { return sleep_until <= relative_to_game_time; }

	bool s_hs_thread_datum::NotSleeping() { return NotSleeping(blam::game_time_get()); }

	void s_hs_thread_datum::Initialize(Enums::hs_thread_type type) {
		this->type         = type;
		this->flags        = 0;
		this->script_index = NONE;
		this->sleep_until  = this->prev_sleep_state = 0;

		this->stack_frame = reinterpret_cast<s_stack_frame *>(this->stack_data);
		this->stack_frame->Initialize();

		result = k_none_value_union;
	}

	void s_hs_thread_datum::Wake() {
		throw "WTF why are you waking a script function. just. don't. Yet, at least.";
		// if (sleep_until == Enums::_hs_thread_datum_sleep_forever)
		// 	return;
		//
		// sleep_until = 0;
		//
		// if (TEST_FLAG(flags, Flags::_hs_thread_datum_sleeping_bit)) {
		// 	SET_FLAG(flags, Flags::_hs_thread_datum_sleeping_bit, false);
		// 	sleep_until = prev_sleep_state;
		// 	return;
		// }
		//
		// auto *call_expression = TryGetFrameSyntax();
		// if (call_expression != nullptr &&
		// 	 call_expression->function_index == Enums::_hs_function_sleep_until) {
		// 	StackPop();
		// 	return;
		// }
		//
		// // IDK what case this is suppose to handle
		// call_expression = TryGetPrevFrameSyntax();
		// if (call_expression != nullptr &&
		// 	 call_expression->function_index == Enums::_hs_function_sleep_until) {
		// 	StackPop();
		// 	StackPop();
		// 	SET_FLAG(flags, Flags::_hs_thread_datum_has_new_stack_frame_bit, false);
		// }
	}

	// bool ValidThread() {
	// 	const Memory::s_data_array &threads = HsThreads().Header;
	//
	// 	const byte *base_address = reinterpret_cast<const byte *>(threads.base_address);
	//
	// 	// validate the thread is within the bounds of the data array
	// 	if (CAST(const void*, this) < base_address)
	// 		return false;
	// 	else if (CAST(const void*, this) > base_address + (threads.last_datum * threads.datum_size))
	// 		return false;
	//
	// 		// validate the stack pointer is within the thread's stack buffer
	// 	else if (CAST(const void*, stack_frame) < &stack_data[0])
	// 		return false;
	// 	else if (CAST(const void*, stack_frame) > this + 1)
	// 		return false;
	//
	// 		// validate the stack hasn't overflowed
	// 	else if (stack_frame->data + stack_frame->size > CAST(const void*, this + 1))
	// 		return false;
	//
	// 	return true;
	// }

	// const char *GetDescriptionString() {
	// 	switch (type) {
	// 		case Enums::_hs_thread_type_script:
	// 			return Scenario::Scenario()->scripts[script_index].name;
	//
	// 		case Enums::_hs_thread_type_global_initialize:
	// 			return "[global initialize]";
	//
	// 		case Enums::_hs_thread_type_runtime_evaluate:
	// 			return "[runtime evaluate]";
	//
	// 		default:
	// 			return "[unknown thread type]";
	// 	}
	// }

	void StackPush() {
		// auto *new_frame = stack_frame + 1;
		// //YELO_HS_RUNTIME_ASSERT(CAST_PTR(byte*, new_frame + 1) < stack_data + Enums::k_hs_thread_stack_size, this, "stack overflow.");
		// (void) (reinterpret_cast<byte *>(new_frame + 1) < stack_data + Enums::k_hs_thread_stack_size ||
		// 		  (_wassert(L"CAST_PTR(byte * , new_frame + 1) < stack_data + Enums::k_hs_thread_stack_size", L"_file_name_", (unsigned) (231)), 0));
		//
		// new_frame->previous = stack_frame;
		// new_frame->size     = 0;
		// this->stack_frame   = new_frame;
	}

	void StackPop() {}// stack_frame = stack_frame->previous; }

	void s_hs_thread_datum::InitializeForScript(s_hs_script_container_datum_index script) {
		// TODO: update code when containers come online

		// auto       *scenario          = blam::global_scenario_get();
		//
		// const auto &script_definition = scenario->scripts[script.idx.script_index].script;
		//
		// this->script_index = script.idx.script_index;
		//
		// if (script_definition.type == Enums::_hs_script_dormant)
		// 	this->sleep_until = Enums::_hs_thread_datum_sleep_dormant;
		// else
		// 	this->sleep_until = 0;
	}

	// template <typename T>
	// T * s_hs_thread_datum::StackAllocate(size_t count, unsigned long alignment_bit, _Out_opt_ short *stack_offset) {
	// 	return reinterpret_cast<T *>(StackAllocate(sizeof(T) * count, alignment_bit, stack_offset));
	// }

	void s_hs_thread_datum::MainPrologue(const s_main_state &state) {
		this->sleep_until = 0;

		if (this->StackIsEmpty()) {
			// assert(state.script);

			this->stack_frame->size = 0;
			//auto *result = //this->StackAllocate<s_hs_value_union>();

			//blam::hs_evaluate(state.thread_index, state.script->script.root_expression_index, nullptr);
		}
	}

	void s_hs_thread_datum::MainLoop(const s_main_state &state) {
		// if (!this->StackIsEmpty()) {
		// 	while (this->NotKilledOrDormant() &&
		// 			 (!blam::game_in_progress() || this->NotSleeping()) &&
		// 			 state.runtime_is_enabled) {
		// 		blam::hs_call_evaluate(state.thread_index);
		// 	}
		// }
	}

	void s_hs_thread_datum::MainEpilogue(const s_main_state &state, _Out_opt_ bool &return_delete_thread) {
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

	void s_hs_thread_datum::Main(datum_index this_thread_index, const bool &runtime_globals_enabled, _Out_opt_ bool &return_delete_thread) {
		// s_main_state state(this_thread_index, runtime_globals_enabled);
		// return_delete_thread = false;
		//
		// if (this->type == Enums::_hs_thread_type_script) {
		// 	state.script = &(blam::global_scenario_get()->scripts[this->script_index]);
		//
		// 	const auto &_script = state.script->script;
		// 	YELO_HS_RUNTIME_ASSERT(_script.type != Enums::_hs_script_static && _script.type != Enums::_hs_script_stub, this, "found a static script at toplevel.");
		// }
		//
		// YELO_HS_THREAD_VALID_STACK(this);
		//
		// MainPrologue(state);
		// MainLoop(state);
		// MainEpilogue(state, return_delete_thread);
	}

};
