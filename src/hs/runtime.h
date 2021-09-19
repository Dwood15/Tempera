
#include "threads.h"
#include "structures.h"
#include "hs.h"
#include "../memory/array.h"

#include "../game/time/time.hpp"
#include "object_lists.h"
#include "yelo_definitions.h"
#include "types/types.h"

#pragma once

namespace Yelo {
	// namespace Scripting {
	// 	hs_thread_data_t &HsThreads();
	//  	hs_globals_data_t &HsGlobals();
	// 	s_hs_runtime_globals *HsRuntimeGlobals();
	// };

	namespace blam {
		using namespace Scripting;

		// void hs_evaluate(datum_index thread_index, datum_index expression_index, Scripting::s_hs_value_union *destination);

		// void hs_call_evaluate(datum_index thread_index);

		// void hs_runtime_update();

		// Scripting::s_hs_value_union *hs_macro_function_evaluate(short function_index, datum_index thread_index, bool initialize_stack);

		// void hs_return(datum_index thread_index, Scripting::s_hs_value_union value);

		// bool hs_can_cast(Enums::hs_type actual_type, Enums::hs_type desired_type);

		// Scripting::s_hs_value_union hs_cast(datum_index thread_index, Enums::hs_type actual_type, Enums::hs_type desired_type, Scripting::s_hs_value_union value);

		// TODO: remove default parameter when containers come online
		// Scripting::s_hs_value_union hs_runtime_evaluate(datum_index expression_index, datum_index script_container_index = datum_index::null());

		// datum_index hs_find_thread_by_script(Scripting::s_hs_script_container_datum_index script);

		// Scripting::s_hs_thread_datum *hs_thread_get(datum_index thread_index);
		//
		// bool hs_type_is_object_name(short type) { return type >= Yelo::Enums::_hs_type_object_name__first && type < Enums::_hs_type_object_name__last; }
		// bool hs_type_is_object(short type) { return type >= Enums::_hs_type_object__first && type < Enums::_hs_type_object__last; }


		// static s_hs_value_union hs_global_evaluate(long global_index) {
		// 	// bool is_internal = (global_index & Enums::_hs_global_index_is_external_mask) == 0;
		// 	// global_index &= Enums::_hs_global_index_mask;
		// 	//
		// 	// // the global is internal, so it comes after all the defined external globals in the hs_globals array
		// 	// if (is_internal) {
		// 	// 	global_index += CAST(long, c_hs_library::GetExternalGlobals().size());
		// 	// }
		// 	//
		// 	// datum_index hs_global_index;
		// 	// hs_global_index.index = CAST(datum_index::index_t, global_index);
		// 	//
		// 	// return HsGlobals()[hs_global_index]->value;
		// }

		// void hs_evaluate(datum_index thread_index, datum_index expression_index, s_hs_value_union *destination) {
			// s_hs_thread_datum    *thread     = hs_thread_get(thread_index);
			// const hs_syntax_node *expression = thread->GetSyntax(expression_index);
			//
			// YELO_HS_THREAD_VALID_STACK(thread);
			// // assert(destination); // engine doesn't do this. I kinda want to make destination a ref...
			//
			// if (TEST_FLAG(expression->flags, Flags::_hs_syntax_node_primitive_bit)) {
			// 	if (TEST_FLAG(expression->flags, Flags::_hs_syntax_node_global_index_bit)) {
			// 		short global_index = expression->value.shurt;
			// 		auto  global_value = hs_global_evaluate(global_index);
			// 		*destination = hs_cast(thread_index, Yelo::blam::hs_global_get_type(global_index), expression->type, global_value);
			// 	} else // else, it's a constant
			// 	{
			// 		*destination = hs_cast(thread_index, expression->constant_type, expression->type, expression->value);
			// 	}
			// } else {
			// 	thread->StackPush();
			// 	thread->flags |= FLAG(Flags::_hs_thread_datum_has_new_stack_frame_bit);
			//
			// 	auto *stack_frame = thread->stack_frame;
			// 	stack_frame->source_expression = expression_index;
			// 	stack_frame->result_reference  = destination;
			// }
		// }
		//
		// static s_hs_value_union *hs_arguments_evaluate(datum_index thread_index, size_t formal_parameters_count, const Enums::hs_type *formal_parameters, bool initialize_stack) {
		// 	s_hs_thread_datum *thread = hs_thread_get(thread_index);
		//
		// 	auto *formal_parameters_values =
		// 			  thread->StackAllocate<s_hs_value_union>(formal_parameters_count);
		// 	auto *argument_index           =
		// 			  thread->StackAllocate<uint>();
		// 	auto *expression_index         =
		// 			  thread->StackAllocate<datum_index>();
		//
		// 	if (initialize_stack) {
		// 		*argument_index = 0;
		// 		const auto *call_expression = thread->GetFrameSyntax();
		// 		datum_index first_arg_expression_index = call_expression->value.ToExpressionIndex();
		// 		*expression_index = thread->GetSyntax(first_arg_expression_index)->next_expression;
		// 	}
		//
		// 	if (*argument_index < formal_parameters_count) {
		// 		YELO_HS_RUNTIME_ASSERT(!expression_index->IsNull(), thread, "corrupted syntax tree.");
		//
		// 		if (YELO_HS_THREAD_SCRIPT_ERROR(
		// 			thread->GetSyntax(*expression_index)->type == formal_parameters[*argument_index],
		// 			thread, "unexpected actual parameters")) {
		// 			return formal_parameters_values;
		// 		}
		//
		// 		hs_evaluate(thread_index, *expression_index, &formal_parameters_values[*argument_index]);
		//
		// 		// set the args evaluator state to the next parameter
		// 		const auto *parameter_expression = thread->GetSyntax(*expression_index);
		// 		*expression_index = parameter_expression->next_expression;
		// 		(*argument_index)++;
		//
		// 		// we haven't evaluated all the parameters just yet
		// 		return nullptr;
		// 	}
		//
		// 	// the last parameter's next_expression index should be NONE, so this should be true at this point
		// 	YELO_HS_RUNTIME_ASSERT(expression_index->IsNull(), thread, "corrupted syntax tree.");
		//
		// 	return formal_parameters_values;
		// }
		//
		// static void hs_script_evaluate(datum_index thread_index, bool initialize_stack, short script_index) {
		// 	// s_hs_thread_datum *thread = hs_thread_get(thread_index);
		// 	// datum_index root_expression_index = global_scenario_get()->scripts[script_index].script.root_expression_index;
		// 	//
		// 	// auto *result = thread->StackAllocate<s_hs_value_union>();
		// 	// if (initialize_stack) {
		// 	// 	hs_evaluate(thread_index, root_expression_index, result);
		// 	// } else {
		// 	// 	hs_return(thread_index, *result);
		// 	// }
		// }

		// static void hs_function_evaluate(datum_index thread_index, bool initialize_stack, short function_index) {
		// 	// auto *function = Yelo::blam::hs_function_get(function_index);
		// 	//assert(function->evaluate);
		//
		// 	// function->evaluate(function_index, thread_index, initialize_stack);
		// }

		// void hs_call_evaluate(datum_index thread_index) {
			// s_hs_thread_datum *thread = hs_thread_get(thread_index);
			//
			// const hs_syntax_node *expression = thread->GetFrameSyntax();
			//
			// bool initialize_stack = TEST_FLAG(thread->flags, Flags::_hs_thread_datum_has_new_stack_frame_bit);
			// SET_FLAG(thread->flags, Flags::_hs_thread_datum_has_new_stack_frame_bit, false);
			//
			// if (expression->IsScriptCall()) {
			// 	hs_script_evaluate(thread_index, initialize_stack, expression->script_index);
			// } else // else, a function index
			// {
			// 	hs_function_evaluate(thread_index, initialize_stack, expression->function_index);
			// }
		// }

		// take hs_runtime_globals as a parameter until we define it in our own code
		// static void hs_thread_main(datum_index thread_index, s_hs_runtime_globals &hs_runtime_globals) {
		// 	hs_runtime_globals.executing_thread_index = thread_index.index;
		//
		// 	s_hs_thread_datum *thread = hs_thread_get(thread_index);
		// 	bool delete_thread;
		// 	thread->Main(thread_index, hs_runtime_globals.enabled, delete_thread);
		//
		// 	if (delete_thread)
		// 		HsThreads().Delete(thread_index);
		//
		// 	hs_runtime_globals.executing_thread_index = NONE;
		// }

		// static void hs_runtime_garbage_collect_nodes() {
		// 	// // only try to gc twice a second
		// 	// const auto k_update_rate = K_TICKS_PER_SECOND / 2;
		// 	//
		// 	// auto game_time = blam::game_time_get();
		// 	//
		// 	// if (game_time / k_update_rate)
		// 	// 	hs_nodes_garbage_collect();
		// }

		// void hs_runtime_update() {
		// 	auto hs_runtime_globals = HsRuntimeGlobals();
		// 	if (!hs_runtime_globals->enabled)
		// 		return;
		//
		// 	auto game_time = blam::game_time_get();
		// 	bool gc_nodes  = false;
		//
		// 	// NOTE: engine seems to use a for(;;) using data_next_index() on threads
		// 	for (auto thread : HsThreads()) {
		// 		// execute all threads related to console commands
		// 		if (thread->type == Enums::_hs_thread_type_runtime_evaluate) {
		// 			gc_nodes = true;
		// 		}
		// 			// by default, the editor doesn't run game scripts
		// 		else if (!hs_runtime_globals->run_game_scripts || CurrentRuntime->IsSapien()) {
		// 			continue;
		// 		}
		//
		// 		if (thread->NotKilledOrDormant() && thread->NotSleeping(game_time)) {
		// 			hs_thread_main(thread.index, *hs_runtime_globals);
		// 		}
		//
		// 		// something executed in the thread which disabled the runtime, stop updating
		// 		if (!hs_runtime_globals->enabled)
		// 			break;
		// 	}
		//
		// 	object_lists_garbage_collect();
		// 	if (gc_nodes)
		// 		hs_runtime_garbage_collect_nodes();
		// }

		// s_hs_value_union *hs_macro_function_evaluate(short function_index, datum_index thread_index, bool initialize_stack) {
		// 	// const auto *function = hs_function_get(function_index);
		// 	//
		// 	// return hs_arguments_evaluate(thread_index, CAST(size_t, function->paramc), function->params, initialize_stack);
		// 	return nullptr;
		// }
		//
		// void hs_return(datum_index thread_index, s_hs_value_union value) {
		// // 	s_hs_thread_datum *thread = hs_thread_get(thread_index);
		// // 	// engine did this after it the hs_syntax_get call, but that makes no sense!
		// // 	YELO_HS_THREAD_VALID_STACK(thread);
		// //
		// // 	const hs_syntax_node *call_expression = thread->GetSyntax(thread->stack_frame->source_expression);
		// //
		// // 	Enums::hs_type actual_type;
		// // 	if (call_expression->IsScriptCall()) {
		// // 		const auto &script = global_scenario_get()->scripts[call_expression->script_index];
		// // 		actual_type = script.script.return_type;
		// // 	} else // hs_function call
		// // 	{
		// // 		const hs_function_definition *function = ::Yelo::blam::hs_function_get(call_expression->function_index);
		// // 		actual_type = function->return_type;
		// // 	}
		// //
		// // 	*thread->stack_frame->previous->result_reference = hs_cast(thread_index, actual_type, call_expression->type, value);
		// //
		// // 	thread->StackPop();
	}
	//
	// static bool hs_object_type_can_cast(Enums::hs_object_type actual_type, Enums::hs_object_type desired_type) {
	// 	// assert(actual_type >= 0 && actual_type < Enums::k_number_of_hs_object_types);
	// 	// assert(desired_type >= 0 && desired_type < Enums::k_number_of_hs_object_types);
	//
	// 	// auto actual_type_mask = k_hs_object_type_masks[actual_type];
	// 	// auto desired_type_mask = k_hs_object_type_masks[desired_type];
	//
	// 	// return (actual_type_mask & desired_type) == desired_type;
	// 	return false;
	// }

	// bool hs_can_cast(Enums::hs_type actual_type, Enums::hs_type desired_type) {
	// 	// assert(actual_type==Enums::_hs_passthrough || hs_type_valid(actual_type));
	// 	// assert(hs_type_valid(desired_type));
	//
	// 	if (actual_type == Enums::_hs_passthrough || actual_type == desired_type)
	// 		return true;
	// 		// desired object is an object reference
	// 	// else if (hs_type_is_object(desired_type)) {
	// 	// 	auto desired_object_type = CAST(Enums::hs_object_type, desired_type - Enums::_hs_type_object__first);
	// 	//
	// 	// 	// can the desired object type be casted to the actual object type?
	// 	// 	if (hs_type_is_object(actual_type)) {
	// 	// 		auto actual_object_type = CAST(Enums::hs_object_type, actual_type - Enums::_hs_type_object__first);
	// 	//
	// 	// 		return hs_object_type_can_cast(actual_object_type, desired_object_type);
	// 	// 	}
	// 	// 		// can the desired object type be casted to the actual object type via an object name?
	// 	// 	else if (hs_type_is_object_name(actual_type)) {
	// 	// 		auto actual_object_type = CAST(Enums::hs_object_type, actual_type - Enums::_hs_type_object_name__first);
	// 	//
	// 	// 		return hs_object_type_can_cast(actual_object_type, desired_object_type);
	// 	// 	}
	// 	//
	// 	// 	// no other means of converting an hs object
	// 	// 	return false;
	// 	// }
	// 		// desired type is an object name
	// 	// else if (hs_type_is_object_name(desired_type)) {
	// 	// 	auto desired_object_type = CAST(Enums::hs_object_type, desired_type - Enums::_hs_type_object_name__first);
	// 	//
	// 	// 	// can the desired named object type be casted to the actual object type via an object name?
	// 	// 	if (hs_type_is_object_name(actual_type)) {
	// 	// 		auto actual_object_type = CAST(Enums::hs_object_type, actual_type - Enums::_hs_type_object_name__first);
	// 	//
	// 	// 		return hs_object_type_can_cast(actual_object_type, desired_object_type);
	// 	// 	}
	// 	//
	// 	// 	// no other means of converting an hs object
	// 	// 	return false;
	// 	// }
	//
	// 	// return k_hs_typecasting_procedures[actual_type][desired_type] != nullptr;
	// 	return false;
	// }

	// s_hs_value_union hs_cast(datum_index thread_index, Enums::hs_type actual_type, Enums::hs_type desired_type, s_hs_value_union value) {
	// 	YELO_HS_RUNTIME_ASSERT(hs_can_cast(actual_type, desired_type),
	// 								  hs_thread_get(thread_index),
	// 								  "bad typecast.");
	//
	// 	if (actual_type == desired_type || actual_type == Enums::_hs_passthrough)
	// 		return value;
	// 	else if (!Yelo::blam::hs_type_is_object_name(desired_type) && Yelo::blam::hs_type_is_object(desired_type) && hs_type_is_object_name(actual_type)) {
	// 		const auto obj_idx_from_name = CurrentRuntime->getFunctionBegin("object_index_from_name_index");
	//
	// 		if (!obj_idx_from_name) {
	// 			throw "Could not find engine function: " "object_index_from_name_index";
	// 		}
	//
	// 		value.datum = ::calls::DoCall<Yelo::Enums::m_cdecl, int, short>(*obj_idx_from_name, value.shurt);
	// 		return value;
	// 	}
	//
	// 	return k_hs_typecasting_procedures[actual_type][desired_type](value);
	// }

	// static datum_index hs_thread_new(Enums::hs_thread_type type, s_hs_script_container_datum_index script = s_hs_script_container_datum_index::k_null) {
	// 	assert(type >= 0 && type < Enums::k_number_of_hs_thread_types);
	// 	assert(type != Enums::_hs_thread_type_script || !script.IsNull());
	//
	// 	datum_index thread_index = HsThreads().New();
	// 	if (thread_index.IsNull())
	// 		return thread_index;
	//
	// 	s_hs_thread_datum *thread = hs_thread_get(thread_index);
	// 	thread->Initialize(type);
	// 	if (type == Enums::_hs_thread_type_script)
	// 		thread->InitializeForScript(script);
	//
	// 	return thread_index;
	// }

	// s_hs_value_union hs_runtime_evaluate(datum_index expression_index, datum_index script_container_index) {
	// 	auto hs_runtime_globals = HsRuntimeGlobals();
	// 	if (!hs_runtime_globals->enabled || expression_index.IsNull())
	// 		return k_none_value_union;
	//
	// 	datum_index thread_index = hs_thread_new(Enums::_hs_thread_type_runtime_evaluate);
	// 	if (thread_index.IsNull()) {
	// 		// NOTE: added parameters to error. still need a better description to display
	// 		// YELO_ERROR_FAILURE("there are not enough threads to execute that command (%X in %X).", expression_index, script_container_index);
	//
	// 		return k_none_value_union;
	// 	}
	//
	// 	s_hs_thread_datum *thread = hs_thread_get(thread_index);
	// 	hs_evaluate(thread_index, expression_index, &thread->result);
	//
	// 	if (TEST_FLAG(thread->flags, Flags::_hs_thread_datum_has_new_stack_frame_bit)) {
	// 		hs_thread_main(thread_index, *hs_runtime_globals);
	// 		return k_none_value_union;
	// 	}
	//
	// 	return thread->result;
	// }

	// datum_index hs_find_thread_by_script(Yelo::Scripting::s_hs_script_container_datum_index script) {
	//
	// 	// NOTE: engine seems to use a for(;;) using data_next_index() on threads
	// 	for (auto thread : HsThreads()) {
	// 		// NOTE: engine doesn't validate type before comparing the index
	// 		if (thread->type == Enums::_hs_thread_type_script &&
	// 			 thread->script_index == script.idx.script_index) {
	// 			return thread.index;
	// 		}
	// 	}
	//
	// 	return datum_index::null();
	// }
};

