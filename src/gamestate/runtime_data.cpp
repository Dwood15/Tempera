
#include <cstddef>
#include <macros_generic.h>
#include <enums/hs_enums.h>
#include <cstring>
#include <utility>
#include "../math/real_math.h"
#include "../cseries/MacrosCpp.h"
#include "../memory/upgrades/game_state_yelo.hpp"

namespace Yelo {
	namespace GameState { namespace RuntimeData {

		struct s_runtime_data {

			template <typename T, size_t kValueCount = Enums::k_runtime_data_max_values_count>
			struct s_value_array {
				bool initialized;
				byte : 8;
				int16 count;      // Number of elements in the runtime data

				T values[kValueCount];

				void InitializeHeader() {
					initialized = true;
					count       = kValueCount;
				}

				void Clear() {
					InitializeHeader();

					memset(values, 0, sizeof(values));
				}

				// Validate that an operation can be performed at [value_index]
				bool OperationValidate(int16 value_index) {
					return initialized && value_index >= 0 && value_index < kValueCount;
				}
			};

			struct s_integers : public s_value_array<int32> {
			} integers;

			struct s_vectors : public s_value_array<real_vector3d> {
			} vectors;
		};
		// Allocated in the game state
		static s_runtime_data *runtime_data;

		// Validate that an operation can be performed at [value_index]
		static bool IntegerOperationValidate(int16 value_index) {
			return runtime_data != nullptr && runtime_data->integers.OperationValidate(value_index);
		}

		// Reset all integers back to zero
		static void IntegersReset() {
			if (runtime_data != nullptr)
				runtime_data->integers.Clear();
		}

		// Get the integer value at [value_index].
		// Returns NONE if this operation fails.
		static int32 IntegerGet(int16 value_index) {
			if (IntegerOperationValidate(value_index))
				return runtime_data->integers.values[value_index];

			return NONE;
		}

		// Get the integer value at [value_index], then set it equal [value].
		// Returns NONE if this operation fails.
		static int32 IntegerGetAndSet(int16 value_index, int32 value) {
			if (IntegerOperationValidate(value_index)) {
				// swap the original with the new and return the original
				auto old = runtime_data->integers.values[value_index];

				runtime_data->integers.values[value_index] = value;

				return old;
			}

			return NONE;
		}

		// Get the integer value at [value_index], then post-increment it.
		// Returns NONE if this operation fails.
		static int32 IntegerGetAndIncrement(int16 value_index) {
			if (IntegerOperationValidate(value_index))
				return runtime_data->integers.values[value_index]++;

			return NONE;
		}

		// Get the integer value at [value_index], then post-decrement it.
		// Returns NONE if this operation fails.
		static int32 IntegerGetAndDecrement(int16 value_index) {
			if (IntegerOperationValidate(value_index))
				return runtime_data->integers.values[value_index]--;

			return NONE;
		}

		static bool ValidBitShift(int32 bit_count) {
			return bit_count >= 0 && bit_count < 32;
		}

#define __INTEGER_OP_CASE(op_code_name)            !strcmp(op_name, op_code_name)
#define __INTEGER_OP_CASE_ARG_NONZERO(op_code_name)__INTEGER_OP_CASE(op_code_name) && op_value != 0

		static int32 IntegerDoOperation(int16 value_index, cstring op_name, int32 op_value) {
			int32 result = NONE;

			if (IntegerOperationValidate(value_index)) {
				int32 &value = runtime_data->integers.values[value_index];

				bool valid_bs = ValidBitShift(op_value);

				//////////////////////////////////////////////////////////////////////////
				// mutable
				if (__INTEGER_OP_CASE("=")) result = value = op_value;
				else if (__INTEGER_OP_CASE("+=")) result = value += op_value;
				else if (__INTEGER_OP_CASE("-=")) result = value -= op_value;
				else if (__INTEGER_OP_CASE("*=")) result = value *= op_value;
				else if (__INTEGER_OP_CASE_ARG_NONZERO("/=")) result = value /= op_value;
				else if (__INTEGER_OP_CASE("++")) result = value++;
				else if (__INTEGER_OP_CASE("--")) result = value--;
				else if (__INTEGER_OP_CASE("&=")) result = value &= op_value;
				else if (__INTEGER_OP_CASE("|=")) result = value |= op_value;
				else if (__INTEGER_OP_CASE("^=")) result = value ^= op_value;
				else if (__INTEGER_OP_CASE("<<=") && valid_bs) result = value <<= op_value;
				else if (__INTEGER_OP_CASE(">>=") && valid_bs) result = value >>= op_value;
					//////////////////////////////////////////////////////////////////////////
					// immutable
				else if (__INTEGER_OP_CASE("+")) result = value + op_value;
				else if (__INTEGER_OP_CASE("-")) result = value - op_value;
				else if (__INTEGER_OP_CASE("*")) result = value * op_value;
				else if (__INTEGER_OP_CASE_ARG_NONZERO("/")) result = value / op_value;
				else if (__INTEGER_OP_CASE("&")) result = value & op_value;
				else if (__INTEGER_OP_CASE("|")) result = value | op_value;
				else if (__INTEGER_OP_CASE("^")) result = value ^ op_value;
				else if (__INTEGER_OP_CASE("<<") && valid_bs) result = value << op_value;
				else if (__INTEGER_OP_CASE(">>") && valid_bs) result = value >> op_value;
			}

			return result;
		}

#undef __INTEGER_OP_CASE
#undef __INTEGER_OP_CASE_ARG_NONZERO

#define __VECTOR_OP_CASE(op_code_name)            result && !strcmp(op_name, op_code_name)
#define __VECTOR_OP_CASE_ARG(op_code_name)         __VECTOR_OP_CASE(op_code_name) && (result = (vec_rhs != nullptr))
#define __VECTOR_OP_CASE_ARG_I_NONZERO(op_code_name)__VECTOR_OP_CASE_ARG(op_code_name) && (result = (vec_rhs->i != 0.0f))

		// Validate that an operation can be performed at [value_index]
		static bool VectorOperationValidate(int16 value_index, int16 element_index = 0) {
			return runtime_data != nullptr && runtime_data->vectors.OperationValidate(value_index) &&
					 element_index >= 0 && element_index < 3; // NOTE: 0=x, 1=y, 2=z
		}

		// Reset all vectors back to zero
		static void VectorsReset() {
			if (runtime_data != nullptr)
				runtime_data->vectors.Clear();
		}

		static real VectorGetElement(int16 value_index, int16 element_index) {
			if (VectorOperationValidate(value_index, element_index)) {
				const real_vector3d &vector = runtime_data->vectors.values[value_index];

				switch (element_index) {
					case 0:
						return vector.i;
					case 1:
						return vector.j;
					case 2:
						return vector.k;
				}
			}

			return static_cast<float>(-1);
		}

		static bool VectorSetElement(int16 value_index, int16 element_index,
											  cstring op_name, real op_value) {
			bool result = false;

			if (VectorOperationValidate(value_index, element_index)) {
				real_vector3d &vector      = runtime_data->vectors.values[value_index];
				real          *element_ptr = nullptr;

				result = true;

				switch (element_index) {
					case 0:
						element_ptr = &vector.i;
						break;
					case 1:
						element_ptr = &vector.j;
						break;
					case 2:
						element_ptr = &vector.k;
						break;
				}

				if (__VECTOR_OP_CASE("=")) *element_ptr = op_value;
				else if (__VECTOR_OP_CASE("+=")) *element_ptr += op_value;
				else if (__VECTOR_OP_CASE("-=")) *element_ptr -= op_value;
				else if (__VECTOR_OP_CASE("*=")) *element_ptr *= op_value;
				else if (__VECTOR_OP_CASE("/=") && op_value != 0.0f) *element_ptr /= op_value;
				else result = false;
			}

			return result;
		}

		// static bool VectorSet(int16 value_index, cstring op_name, real x, real y, real z) {
		// 	bool result = false;
		//
		// 	if (VectorOperationValidate(value_index)) {
		// 		real_vector3d &vector = runtime_data->vectors.values[value_index];
		//
		// 		result = true;
		//
		// 		if (__VECTOR_OP_CASE("=")) vector.Set(x, y, z);
		// 		else if (__VECTOR_OP_CASE("+=")) vector.Set(vector.i + x, vector.j + y, vector.k + z);
		// 		else if (__VECTOR_OP_CASE("-=")) vector.Set(vector.i - x, vector.j - y, vector.k - z);
		// 		else if (__VECTOR_OP_CASE("*=")) vector.Set(vector.i * x, vector.j * y, vector.k * z);
		// 		else if (__VECTOR_OP_CASE("/=")) {
		// 			if (x == 0.0f) x = 1.0f;
		// 			if (y == 0.0f) y = 1.0f;
		// 			if (z == 0.0f) z = 1.0f;
		// 			vector.Set(vector.i / x, vector.j / y, vector.k / z);
		// 		} else result = false;
		// 	}
		//
		// 	return result;
		// }

		// static bool VectorDoOperation(int16 value_index, cstring op_name, int16 op_arg_vector_index) {
		// 	bool result = false;
		//
		// 	if (VectorOperationValidate(value_index)) {
		// 		real_vector3d &vector  = runtime_data->vectors.values[value_index];
		// 		real_vector3d *vec_rhs = VectorOperationValidate(op_arg_vector_index) ?
		// 										 &runtime_data->vectors.values[op_arg_vector_index] : nullptr;
		//
		// 		result = true;
		//
		// 		if (__VECTOR_OP_CASE("normalize")) vector.Normalize();
		// 		else if (__VECTOR_OP_CASE("inverse")) vector.Inverse();
		// 		else if (__VECTOR_OP_CASE("conjugate")) vector = -vector;
		// 		else if (__VECTOR_OP_CASE_ARG("+=")) vector += *vec_rhs;
		// 		else if (__VECTOR_OP_CASE_ARG("-=")) vector -= *vec_rhs;
		// 		else if (__VECTOR_OP_CASE_ARG("*=")) vector *= vec_rhs->i;
		// 		else if (__VECTOR_OP_CASE_ARG_I_NONZERO("/=")) vector /= vec_rhs->i;
		// 		else if (__VECTOR_OP_CASE_ARG("cross_product")) vector = vector ^ *vec_rhs;
		// 		else if (__VECTOR_OP_CASE_ARG("dot_product")) vec_rhs->i = vector * *vec_rhs;
		// 		else if (__VECTOR_OP_CASE_ARG("scalar_mul")) vector = vector * vec_rhs->i;
		// 		else if (__VECTOR_OP_CASE_ARG_I_NONZERO("scalar_div")) vector = vector / vec_rhs->i;
		// 		else if (__VECTOR_OP_CASE_ARG("magnitude")) vec_rhs->i = vector.Magnitude();
		// 		else result = false;
		// 	}
		//
		// 	return result;
		// }

		static char *VectorToString(int16 value_index) {
			static char static_buffer[128];
			static_buffer[0] = '\0';

			if (VectorOperationValidate(value_index)) {
				const real_vector3d &vector = runtime_data->vectors.values[value_index];

				sprintf_s(static_buffer, "%f %f %f", vector.i, vector.j, vector.k);
			}

			return static_buffer;
		}

#undef __VECTOR_OP_CASE
#undef __VECTOR_OP_CASE_ARG
#undef __VECTOR_OP_CASE_ARG_I_NONZERO

		void Initialize() {
		}

		void Dispose() {
		}

		void InitializeForNewGameState() {
			runtime_data = GameState::GameStateMalloc<s_runtime_data>();
		}

		void InitializeForNewMap() {
			IntegersReset();
			VectorsReset();
		}


		//////////////////////////////////////////////////////////////////////////
		// integers
		static void* scripting_runtime_integers_reset_evaluate()
		{
			IntegersReset();

			return nullptr;
		}

		static void* scripting_runtime_integer_get_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
						short : 16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.int32 = IntegerGet(args->value_index);

			return result.pointer;
		}

		static void* scripting_runtime_integer_set_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
						short : 16;
				int32 value;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.int32 = IntegerGetAndSet(args->value_index, args->value);

			return result.pointer;
		}

		static void* scripting_runtime_integer_inc_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
						short : 16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.int32 = IntegerGetAndIncrement(args->value_index);

			return result.pointer;
		}

		static void* scripting_runtime_integer_dec_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
						short : 16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.int32 = IntegerGetAndDecrement(args->value_index);

			return result.pointer;
		}

		static void* scripting_runtime_integer_operation_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
						short : 16;
				cstring op_name;
				int32 op_value;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.int32 = IntegerDoOperation(args->value_index, args->op_name, args->op_value);

			return result.pointer;
		}


		//////////////////////////////////////////////////////////////////////////
		// vectors
		static void* scripting_runtime_vectors_reset_evaluate()
		{
			VectorsReset();

			return nullptr;
		}
		static void* scripting_runtime_vector_get_element_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
						short : 16;
				int16 element_index;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.real = VectorGetElement(args->value_index, args->element_index);

			return result.pointer;
		}
		static void* scripting_runtime_vector_set_element_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
				short : 16;
				int16 element_index;
				short : 16;
				cstring op_name;
				real op_value;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.boolean = VectorSetElement(args->value_index, args->element_index,
														 args->op_name, args->op_value);

			return result.pointer;
		}


		static void* scripting_runtime_vector_set_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
				short : 16;
				cstring op_name;
				real op_x; real op_y; real op_z;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			//result.boolean = VectorSet(args->value_index, args->op_name, args->op_x, args->op_y, args->op_z);

			return result.pointer;
		}
		static void* scripting_runtime_vector_operation_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
				short : 16;
				cstring op_name;
				int16 op_arg_vector_index;
				short : 16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			//result.boolean = VectorDoOperation(args->value_index, args->op_name, args->op_arg_vector_index);

			return result.pointer;
		}
		static void* scripting_runtime_vector_to_string_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 value_index;
				short : 16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.pointer = VectorToString(args->value_index);

			return result.pointer;
		}

#include "../hs/script_extensions/hs_base.h"

			void InitializeScripting() {
				Scripting::InitializeScriptFunction(Enums::hs_function_enumeration::_hs_function_runtime_integers_reset, scripting_runtime_integers_reset_evaluate);
				Scripting::InitializeScriptFunctionWithParams(::Yelo::Enums::_hs_function_runtime_integer_get, scripting_runtime_integer_get_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_integer_set, scripting_runtime_integer_set_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_integer_inc, scripting_runtime_integer_inc_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_integer_dec, scripting_runtime_integer_dec_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_integer_operation,
																			 scripting_runtime_integer_operation_evaluate);

				Scripting::InitializeScriptFunction(Enums::_hs_function_runtime_vectors_reset, scripting_runtime_vectors_reset_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_vector_get_element,
																			 scripting_runtime_vector_get_element_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_vector_set_element,
																			 scripting_runtime_vector_set_element_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_vector_set, scripting_runtime_vector_set_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_vector_operation, scripting_runtime_vector_operation_evaluate);
				Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_vector_to_string, scripting_runtime_vector_to_string_evaluate);
			}

			real_vector3d *VectorValueGetForModify(int16 value_index) {
				if (VectorOperationValidate(value_index))
					return &runtime_data->vectors.values[value_index];

				return nullptr;
			}

			const real_vector3d *VectorValueGet(int16 value_index) {
				if (VectorOperationValidate(value_index))
					return &runtime_data->vectors.values[value_index];

				return nullptr;
			}

		}; };
};
