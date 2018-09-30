/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum runtime_data_integer_operation
		{
			_runtime_data_integer_operation_invalid,

			//////////////////////////////////////////////////////////////////////////
			// mutable
			_runtime_data_integer_operation_eq,
			_runtime_data_integer_operation_plus_eq,
			_runtime_data_integer_operation_minus_eq,
			_runtime_data_integer_operation_mul_eq,
			_runtime_data_integer_operation_div_eq,
			_runtime_data_integer_operation_plus_plus,
			_runtime_data_integer_operation_minus_minus,
			_runtime_data_integer_operation_and_eq,
			_runtime_data_integer_operation_or_eq,
			_runtime_data_integer_operation_xor_eq,
			_runtime_data_integer_operation_lhs_eq,
			_runtime_data_integer_operation_rhs_eq,
			//////////////////////////////////////////////////////////////////////////
			// immutable
			_runtime_data_integer_operation_plus,
			_runtime_data_integer_operation_minus,
			_runtime_data_integer_operation_mul,
			_runtime_data_integer_operation_div,
			_runtime_data_integer_operation_and,
			_runtime_data_integer_operation_or,
			_runtime_data_integer_operation_xor,
			_runtime_data_integer_operation_lhs,
			_runtime_data_integer_operation_rhs,

			_runtime_data_integer_operation,
		};

		enum runtime_data_vector_set_operation
		{
			_runtime_data_vector_set_operation_invalid,

			_runtime_data_vector_set_operation_eq,
			_runtime_data_vector_set_operation_plus_eq,
			_runtime_data_vector_set_operation_minus_eq,
			_runtime_data_vector_set_operation_mul_eq,
			_runtime_data_vector_set_operation_div_eq,

			_runtime_data_vector_set_operation,
		};

		enum runtime_data_vector_operation
		{
			_runtime_data_vector_operation_invalid,

			_runtime_data_vector_operation_normalize,
			_runtime_data_vector_operation_inverse,
			_runtime_data_vector_operation_conjugate,
			_runtime_data_vector_operation_plus_eq,
			_runtime_data_vector_operation_minus_eq,
			_runtime_data_vector_operation_mul_eq,
			_runtime_data_vector_operation_div_eq,
			_runtime_data_vector_operation_cross_product,
			_runtime_data_vector_operation_dot_product,
			_runtime_data_vector_operation_scalar_mul,
			_runtime_data_vector_operation_scalar_div,
			_runtime_data_vector_operation_magnitude,

			_runtime_data_vector_operation,
		};
	};

	namespace GameState { namespace RuntimeData {

		void Initialize();
		void Dispose();

		void InitializeForNewGameState();
		void InitializeForNewMap();

		void InitializeScripting();

		real_vector3d* VectorValueGetForModify(int16 value_index);
		const real_vector3d* VectorValueGet(int16 value_index);

	}; };
};