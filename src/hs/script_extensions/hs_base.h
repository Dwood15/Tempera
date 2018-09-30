#pragma once


#include "../function_declarations.h"
#include "../globals_declarations.h"
#include "../structures.h"
#include "../../memory/memory_interface_base.hpp"
#include <iterator>
namespace Yelo {
	namespace Scripting {

		static hs_function_definition **hs_function_table;

		// [hs_function_table] in hs_eval_func_has_param
#define HS_EVAL_INDEX_HS_FUNC_TABLE_WITH_PARAM         (5+3)

		// [func] in hs_eval_func_no_param
#define HS_EVAL_INDEX_FUNC_NO_PARAM                  (0+2)

		// [func] in hs_eval_func_has_param
#define HS_EVAL_INDEX_FUNC_WITH_PARAM               (46+2)

		// [hs_return_address] in hs_eval_func_no_param
#define HS_EVAL_INDEX_HS_RETURN_NO_PARAM            (10+2)

		// [hs_return_address] in hs_eval_func_has_param
#define HS_EVAL_INDEX_HS_RETURN_WITH_PARAM            (55+2)

		// [hs_arguments_evaluate_address] in hs_eval_func_has_param
#define HS_EVAL_INDEX_HS_ARGUMENTS_EVALUTE_WITH_PARAM   (32+2)

		// opcodes for a hs_evaluate function that takes no parameters
		static const byte hs_eval_func_no_param[] = {
			0xFF, 0x15, 0x00, 0x00, 0x00, 0x00, // 00 6, call [func]
			0x8B, 0x4C, 0x24, 0x08,            // 06 4, mov ecx, [esp+8]
			0xFF, 0x15, 0x00, 0x00, 0x00, 0x00, // 10 6, call [hs_return_address]
			0xC3                        // 16 1, retn
		};

		// opcodes for a hs_evaluate function that takes parameters
		static const byte hs_eval_func_has_param[] = {
			0x0F, 0xBF, 0x44, 0x24, 0x04,            // 00 5, movsx   eax, [esp+arg_0]
			0x8B, 0x04, 0x85, 0x00, 0x00, 0x00, 0x00,   // 05 7, mov     eax, hs_function_table[eax*4]
			0x8B, 0x4C, 0x24, 0x0C,                  // 12 4, mov     ecx, [esp+arg_8]
			0x56,                              // 16 1, push    esi
			0x8B, 0x74, 0x24, 0x0C,                  // 17 4, mov     esi, [esp+4+arg_4]
			0x51,                              // 21 1, push    ecx
			0x8D, 0x50, 0x1C,                     // 22 3, lea     edx, [eax+1Ch]
			0x0F, 0xBF, 0x40, 0x1A,                  // 25 4, movsx   eax, word ptr [eax+1Ah]
			0x52,                              // 29 1, push	 edx
			0x50,                              // 30 1, push    eax
			0x56,                              // 31 1, push    esi
			0xFF, 0x15, 0x00, 0x00, 0x00, 0x00,         // 32 6, call    [hs_arguments_evaluate_address]
			0x83, 0xC4, 0x10,                     // 38 3, add     esp, 10h
			0x85, 0xC0,                           // 41 2, test    eax, eax
			0x74, 0x10,                           // 43 2, jz      short failed
			0x50,                              // 45 1, push    eax ; push the argument list
			0xFF, 0x15, 0x00, 0x00, 0x00, 0x00,         // 46 6, call    [func]
			0x8B, 0xCE,                           // 52 2, mov     ecx, esi
			0x5E,                              // 54 1, pop     esi
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,         // 55 6, jmp     [hs_return_address]
			// failed:
			0x5E,                              // 61 1, pop     esi
			0xC3,                              // 62 1, retn

			0, // 64 1, pad
		};

		// Initialize the [k_external_global_opensauce_override_name] global
		// Called from MemoryUpgrades (since we process the globals list there anyway)
		static void InitializeExternalGlobalOpenSauceOverride(hs_global_definition &global) {
			// Not really constant but should be treated as such
			static real k_opensauce_version = real(1.0);

			if (global.address == nullptr && global.type == Enums::_hs_type_real)
				global.address = &k_opensauce_version;
		}


		// Set the function's (with no parameters) evaluator to one which
		// does nothing and returns zero
		void NullifyScriptFunction(hs_function_definition &function);

		void NullifyScriptFunction(Yelo::Enums::hs_function_enumeration function);

		// Set the function's (which expects parameters) evaluator to one
		// which does nothing and returns zero
		void NullifyScriptFunctionWithParams(hs_function_definition &function);

		void NullifyScriptFunctionWithParams(Yelo::Enums::hs_function_enumeration function);

		// Only 39 functions in CE release are actually 'null'...but now in OS2, we no longer overwrite existing functions!
		// So ignore this comment :p
		static uint hs_eval_func_ptrs[Enums::k_hs_script_functions_count_upgrade - Enums::k_hs_functions_count];
		static long hs_eval_func = 0;

		// next free hs_eval_func_ptrs index
		static byte hs_func_pool[Enums::k_hs_script_functions_count_upgrade - Enums::k_hs_functions_count][sizeof(hs_eval_func_has_param)];

		// Creates a evaluate function that calls [func] and passes
		// an array of parameters to it if it [takes_params]
		//
		// [func] - pointer to a __stdcall type function
		// Returns: evaluate function address
		static void *CreateScriptFunction(void *func, bool takes_params) {
			if ((unsigned long)hs_eval_func >=::std::size(hs_eval_func_ptrs)) return nullptr; // we don't want to go over our set limit

			void *evaluate = nullptr;
			hs_eval_func_ptrs[hs_eval_func] = reinterpret_cast<uint>(func);
			uint *temp = nullptr;

			if (takes_params) {
				memory::WriteMemory(
					hs_func_pool[hs_eval_func],
					hs_eval_func_has_param);

				temp = reinterpret_cast<uint *>(&(hs_func_pool[hs_eval_func][(46 + 2)]));
				*temp = reinterpret_cast<uint>(&(hs_eval_func_ptrs[hs_eval_func]));
			} else {
				memory::WriteMemory(
					hs_func_pool[hs_eval_func],
					hs_eval_func_no_param);

				temp = reinterpret_cast<uint *>(&(hs_func_pool[hs_eval_func][(0 + 2)]));
				*temp = reinterpret_cast<uint>(&(hs_eval_func_ptrs[hs_eval_func]));
			}

			evaluate = &(*hs_func_pool[hs_eval_func]);
			hs_eval_func++; // go to the next free func ptr
			return evaluate;
		}


		static bool DefinitionsNameMatch(const char * name, const tag_string definition_name[2]) {
			size_t length = strlen(name);
			bool   result = !strncmp(name, definition_name[0], Enums::k_tag_string_length);

			// if the scripting name is longer than 31 characters, compare the other characters too
			if (length > Enums::k_tag_string_length)
				result &= !strncmp(name + (Enums::k_tag_string_length + 1),
										 definition_name[1],
										 Enums::k_tag_string_length);

			return result;
		}

#define YELO_INIT_SCRIPT_FUNCTION(function_index, proc)                     Scripting::InitializeScriptFunction(function_index, proc)
#define YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS(function_index, proc)            Scripting::InitializeScriptFunctionWithParams(function_index, proc)
#define YELO_INIT_SCRIPT_FUNCTION_USER(function_index, proc)            YELO_INIT_SCRIPT_FUNCTION(function_index, proc)
#define YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(function_index, proc)   YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS(function_index, proc)

#define YELO_INIT_SCRIPT_FUNCTION_DEDI(function_index, proc)            Scripting::NullifyScriptFunction(function_index)
#define YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(function_index, proc)   Scripting::NullifyScriptFunctionWithParams(function_index)

		static void ScriptFunctionSetEvaluteProc(hs_function_definition &function, proc_hs_evaluate proc) {
			if (function.paramc != 0)
				Print<true>("Setting script function which has parameters (expected none): %s", function.name);

			function.evaluate = proc;
		}

		static void ScriptFunctionWithParamsSetEvaluteProc(hs_function_definition &function, proc_hs_evaluate proc) {
			if (function.paramc == 0)
				Print<true>("Setting script function which has no parameters (expected 1 or more): %s", function.name);

			function.evaluate = proc;
		}

		// True if the script definitions in [data] match the functions/globals
		// defined by Yelo's code.
		// static bool DefinitionsMatch(const TagGroups::s_scripting_definitions &data) {
		// 	const byte k_developer_mode = *GameState::DeveloperMode();
		// 	char       developer_mode_msg[256];
		//
		// 	bool is_valid = false;
		//
		// 	for (const auto &def : data.new_functions) {
		// 		is_valid = false;
		// 		// continue checking each EXPORTED function with the yelo script definitions
		// 		for (long f = 0; f < K_HS_YELO_FUNCTION_COUNT && !is_valid; f++) {
		// 			const hs_function_definition &yelo_def = *hs_yelo_functions[f];
		//
		// 			if (DefinitionsNameMatch(yelo_def.name, def.name) &&         // check if names match...
		// 				 def.parameters.Count == yelo_def.paramc &&               // check if we have the same amount of parameters
		// 				 (is_valid = (def.return_type == yelo_def.return_type))      // check if we have the same return type
		// 				) {
		// 				const short *params = def.parameters.Definitions;         // check if the parameter types are the same
		// 				for (long p = 0;
		// 					  p < def.parameters.Count && is_valid;               // continue checking while things compare as a match
		// 					  p++)
		// 					is_valid = params[p] == yelo_def.params[p];
		// 			}
		// 		}
		//
		// 		if (!is_valid && k_developer_mode >= Yelo::Enums::k_developer_mode_level_debug_output) {
		// 			sprintf_s(developer_mode_msg, "Script (%s) mismatch info: %s", "function", def.name);
		// 			PrepareToDropError(developer_mode_msg);
		// 		}
		//
		// 		if (!is_valid)
		// 			return false;
		// 	}
		//
		// 	for (const auto &def : data.new_globals) {
		// 		is_valid = false;
		// 		for (long g = 0; g < K_HS_YELO_GLOBALS_COUNT && !is_valid; g++) {
		// 			const hs_global_definition &yelo_def = *hs_yelo_globals[g];
		//
		// 			is_valid = DefinitionsNameMatch(yelo_def.name, reinterpret_cast<tag_string const *>(def.name)) &&   // check if names match...
		// 						  def.type == yelo_def.type;                     // check if the global types match too
		// 		}
		//
		// 		if (!is_valid && k_developer_mode >= Enums::k_developer_mode_level_debug_output) {
		// 			sprintf_s(developer_mode_msg, "Script (%s) mismatch info: %s", "global", def.name);
		// 			PrepareToDropError(developer_mode_msg);
		// 		}
		//
		// 		if (!is_valid)
		// 			return false;
		// 	}
		//
		// 	return true;
		// }
	};
};
