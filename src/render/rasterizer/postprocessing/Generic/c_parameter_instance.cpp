/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/c_parameter_instance.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/time/interpolation/interpolation.hpp>
#include <YeloLib/Halo1/time/interpolation/c_interp_linear.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>

#include "Rasterizer/PostProcessing/Interpolation/c_interp_function_pp_anim.hpp"

// runtime value includes
#include "Game/Players.hpp"
#include "Objects/Objects.hpp"

using namespace Yelo::Time::Interpolation;

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic
	{
#include "Rasterizer/PostProcessing/Generic/PostProcessingGeneric_EvaluateRuntimeValue.inl"

		/////////////////////////////////////////////////
		// member accessors
		TagGroups::s_shader_postprocess_parameter* c_parameter_instance::GetParameter()
		{
			return m_members.parameter;
		}

		void c_parameter_instance::SetParameter(TagGroups::s_shader_postprocess_parameter* parameter)
		{
			m_members.parameter = parameter;
		}

		/////////////////////////////////////////////////
		// parameter instance setup
		void c_parameter_instance::SetupParameterInstance()
		{
			// perform the relevant setup logic for the value type
			switch(m_members.parameter->value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
			case Enums::_shader_variable_base_type_integer:
			case Enums::_shader_variable_base_type_float:
			case Enums::_shader_variable_base_type_argb_color:
				if(m_members.parameter->runtime_value.value != 0)
					SetupRuntimeValue();
				else if(m_members.parameter->animation_function.function == Enums::_periodic_function_one)
					SetupStaticValue();
				else
					SetupAnimatedValue();
				break;
			case Enums::_shader_variable_base_type_texture:
				SetupTextureValue();
				break;

			YELO_ASSERT_CASE_UNREACHABLE();
			};
		}

		void c_parameter_instance::SetupParameterOverride()
		{
			// set the parameter type to animated to enable value interpolation
			m_members.parameter_type = Enums::_parameter_process_type_animated;

			// delete the currently allocated interpolator if present and create a linear one
			DeleteInterpolator();
			CreateInterpolatorLinear();

			// allocate memory for the current and target variables, initialising the current value to the parameters upper bound
			switch(m_members.parameter->value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				AllocateValueMemory<bool>();
				*m_members.m_values[0].boolean = *m_members.m_values[1].boolean = m_members.parameter->value.boolean.enabled;
				break;
			case Enums::_shader_variable_base_type_integer:
				AllocateValueMemory<int32>();
				*m_members.m_values[0].integer32 = *m_members.m_values[1].integer32 = m_members.parameter->value.integer32.upper_bound;
				break;
			case Enums::_shader_variable_base_type_float:
				switch(m_members.parameter->value_type.count)
				{
				case 1:
					AllocateValueMemory<real>();
					*m_members.m_values[0].real32 = *m_members.m_values[1].real32 = m_members.parameter->value.real32.upper_bound;
					break;
				case 2:
					AllocateValueMemory<real_vector2d>();
					*m_members.m_values[0].vector2d = *m_members.m_values[1].vector2d = m_members.parameter->value.vector2d.upper_bound;
					break;
				case 3:
					AllocateValueMemory<real_vector3d>();
					*m_members.m_values[0].vector3d = *m_members.m_values[1].vector3d = m_members.parameter->value.vector3d.upper_bound;
					break;
				case 4:
					AllocateValueMemory<real_quaternion>();
					*m_members.m_values[0].quaternion = *m_members.m_values[1].quaternion = m_members.parameter->value.vector4d.upper_bound;
					break;
				default:
					YELO_ASSERT_DISPLAY(m_members.parameter->value_type.count != 0, "Invalid value type count");
					break;
				};
			case Enums::_shader_variable_base_type_argb_color:
				AllocateValueMemory<real_argb_color>();
				*m_members.m_values[0].color = *m_members.m_values[1].color = m_members.parameter->value.color4d.upper_bound;
				break;
			case Enums::_shader_variable_base_type_matrix:
				YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type != Enums::_shader_variable_base_type_matrix, "Scripted intepolation of matrices is not supported");
				return;

			YELO_ASSERT_CASE_UNREACHABLE();
			}
			m_members.m_flags.is_overriden = true;
		}

		void c_parameter_instance::SetupStaticValue()
		{
			// the parameters vaue will not change, no further setup is necessary
			m_members.parameter_type = Enums::_parameter_process_type_static;
		}

		void c_parameter_instance::SetupAnimatedValue()
		{
			// the parameter will interpolate using halos animation functions so create a function interpolator
			m_members.parameter_type = Enums::_parameter_process_type_animated;
			CreateInterpolatorFunction();
		}

		void c_parameter_instance::SetupRuntimeValue()
		{
			// the parameter is set with an engine value at runtime
			m_members.parameter_type = Enums::_parameter_process_type_runtime_value;
			
			// set the runtime evaluation function pointer
			switch(m_members.parameter->value_type.type)
			{
			default:
				return;
			//case Enums::_shader_variable_base_type_boolean:
			//case Enums::_shader_variable_base_type_integer:
			//case Enums::_shader_variable_base_type_float:
			case Enums::_shader_variable_base_type_argb_color:
				switch(m_members.parameter->runtime_value.value)
				{
				case Enums::_parameter_runtime_value_color_player_team_color:
					m_members.evaluate_runtime_value = EvaluateRuntimeValue::Color_PlayerTeamColor;
				}
			}
		}

		void c_parameter_instance::SetupTextureValue()
		{
			// textures require no extra setup as they are not animated
			m_members.parameter_type = Enums::_parameter_process_type_texture;
		}

		void c_parameter_instance::CreateInterpolatorFunction()
		{
			YELO_ASSERT_DISPLAY(m_members.interpolator == nullptr, "interpolator being created before the current interpolator has been deleted");

			// create an interpolator that uses halos function code for interpolating between two values with up to 4 channels
			switch(m_members.parameter->value_type.count)
			{
			default:
				{
					if(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_argb_color)
						m_members.interpolator = new c_interp_function_pp_anim<4>(&m_members.parameter->animation_function);
					else
						m_members.interpolator = new c_interp_function_pp_anim<1>(&m_members.parameter->animation_function);
					break;
				}
			case 2: m_members.interpolator = new c_interp_function_pp_anim<2>(&m_members.parameter->animation_function); break;
			case 3: m_members.interpolator = new c_interp_function_pp_anim<3>(&m_members.parameter->animation_function); break;
			case 4: m_members.interpolator = new c_interp_function_pp_anim<4>(&m_members.parameter->animation_function); break;
			}

			m_members.interpolator->Ctor();
		}

		void c_parameter_instance::CreateInterpolatorLinear()
		{
			YELO_ASSERT_DISPLAY(m_members.interpolator == nullptr, "interpolator being created before the current interpolator has been deleted");

			// create an interpolator that linearly interpolates between 0 and 1 for up to 4 channels
			switch(m_members.parameter->value_type.count)
			{
			default:
				{
					if(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_argb_color)
						m_members.interpolator = new c_interp_linear<4>();
					else
						m_members.interpolator = new c_interp_linear<1>();
					break;
				}
			case 2: m_members.interpolator = new c_interp_linear<2>(); break;
			case 3: m_members.interpolator = new c_interp_linear<3>(); break;
			case 4: m_members.interpolator = new c_interp_linear<4>(); break;
			}

			m_members.interpolator->Ctor();
		}

		void c_parameter_instance::DeleteInterpolator()
		{
			if(m_members.interpolator)
				m_members.interpolator->Dtor();
			// delete the interpolator currently allocated for the parameter
			delete m_members.interpolator;
			m_members.interpolator = nullptr;
		}

		template<class T>
		void c_parameter_instance::AllocateValueMemory()
		{
			// allocate memory for the parameter override values
			for (auto& value : m_members.m_values)
			{
				value.pointer = new T();
				memset(value.pointer, 0, sizeof(T));
			}
		}

		void c_parameter_instance::DeleteValueMemory()
		{
			// delete memory allocated for the parameter override
			for (auto& value : m_members.m_values)
			{
				delete value.pointer;
				value.pointer = nullptr;
			}
		}

		/////////////////////////////////////////////////
		// parameter instance application
		void c_parameter_instance::Update(real delta_time)
		{
			// animated parameters are the only type that need to be updated
			switch(m_members.parameter_type)
			{
			case Enums::_parameter_process_type_static:
			case Enums::_parameter_process_type_runtime_value:
			case Enums::_parameter_process_type_texture:
				return;
			case Enums::_parameter_process_type_animated:
				{
					YELO_ASSERT_DISPLAY(m_members.interpolator != nullptr, "updating a parameter instance that has no interpolator allocated");
					if(m_members.interpolator)
						m_members.interpolator->Update(delta_time);
				}
				break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}
		}

		void c_parameter_instance::SetParameterVariable(LPD3DXEFFECT effect)
		{
			// textures do not currently animate so set and return
			if(m_members.parameter_type == Enums::_parameter_process_type_texture)
			{
				m_members.parameter->value.bitmap.handle.SetVariable(effect, m_members.parameter->GetTexture());
				return;
			}

			// runtime values are taken directly from the engine
			if(m_members.parameter_type == Enums::_parameter_process_type_runtime_value)
			{
				YELO_ASSERT_DISPLAY(m_members.evaluate_runtime_value != nullptr, "parameter set as a runtime value, has no evaluate function assigned");

				// using a real_quaternion as it is the largest possible variable size in a parameter
				real_quaternion value;
				// retrieve the runtime value, then set the variable
				m_members.evaluate_runtime_value(&value);
				m_members.parameter->SetVariable(effect, &value);
				return;
			}

			// if the parameter has been overridden use the parameter instances variables
			// otherwise use those in the parameters tag block
			void* lower;
			void* upper;
			if(m_members.m_flags.is_overriden)
			{
				lower = m_members.m_values[0].pointer;
				upper = m_members.m_values[1].pointer;
			}
			else
				m_members.parameter->GetBoundingValues(lower, upper);

			// set the parameter variables, interpolating if animated
			if(m_members.parameter_type == Enums::_parameter_process_type_animated)
				m_members.parameter->SetVariableInterp(effect, lower, upper, m_members.interpolator->GetValues());
			else
				m_members.parameter->SetVariable(effect, upper);
		}

		/////////////////////////////////////////////////
		// parameter instance override application
		void c_parameter_instance::SetOverrideInterp(bool value, real change_time)
		{
			YELO_ASSERT_DISPLAY(m_members.m_flags.is_overriden, "parameter instance not set up to be overriden");
			YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_boolean, "parameter being overriden with an incorrect type");

			// set the start value using the current values and interpolation amount
			SetStartValueToCurrent();

			// set the target value
			*m_members.m_values[1].boolean = value;

			// reset the interpolator to the current time frame
			auto interp_linear = CAST_PTR(c_interp_linear<1>*, m_members.interpolator);
			interp_linear->Begin(change_time);
		}

		void c_parameter_instance::SetOverrideInterp(int32 value, real change_time)
		{
			YELO_ASSERT_DISPLAY(m_members.m_flags.is_overriden, "parameter instance not set up to be overriden");
			YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_integer, "parameter being overriden with an incorrect type");

			// set the start value using the current values and interpolation amount
			SetStartValueToCurrent();

			// set the target value
			*m_members.m_values[1].integer32 = value;

			// reset the interpolator to the current time frame
			auto interp_linear = CAST_PTR(c_interp_linear<1>*, m_members.interpolator);
			interp_linear->Begin(change_time);
		}

		void c_parameter_instance::SetOverrideInterp(real value, real change_time)
		{
			YELO_ASSERT_DISPLAY(m_members.m_flags.is_overriden, "parameter instance not set up to be overriden");
			YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_float, "parameter being overriden with an incorrect type");

			// set the start value using the current values and interpolation amount
			SetStartValueToCurrent();

			// set the target value
			*m_members.m_values[1].real32 = value;

			// reset the interpolator to the current time frame
			auto interp_linear = CAST_PTR(c_interp_linear<1>*, m_members.interpolator);
			interp_linear->Begin(change_time);
		}

		void c_parameter_instance::SetOverrideInterp(const real_vector2d& value, real change_time)
		{
			YELO_ASSERT_DISPLAY(m_members.m_flags.is_overriden, "parameter instance not set up to be overriden");
			YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_float, "parameter being overriden with an incorrect type");

			// set the start value using the current values and interpolation amount
			SetStartValueToCurrent();

			// set the target value
			*m_members.m_values[1].vector2d = value;

			// reset the interpolator to the current time frame
			auto interp_linear = CAST_PTR(c_interp_linear<2>*, m_members.interpolator);
			interp_linear->Begin(change_time);
		}

		void c_parameter_instance::SetOverrideInterp(const real_vector3d& value, real change_time)
		{
			YELO_ASSERT_DISPLAY(m_members.m_flags.is_overriden, "parameter instance not set up to be overriden");
			YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_float, "parameter being overriden with an incorrect type");

			// set the start value using the current values and interpolation amount
			SetStartValueToCurrent();

			// set the target value
			*m_members.m_values[1].vector3d = value;

			// reset the interpolator to the current time frame
			auto interp_linear = CAST_PTR(c_interp_linear<3>*, m_members.interpolator);
			interp_linear->Begin(change_time);
		}

		void c_parameter_instance::SetOverrideInterp(const real_quaternion& value, real change_time)
		{
			YELO_ASSERT_DISPLAY(m_members.m_flags.is_overriden, "parameter instance not set up to be overriden");
			YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_float, "parameter being overriden with an incorrect type");

			// set the start value using the current values and interpolation amount
			SetStartValueToCurrent();

			// set the target value
			*m_members.m_values[1].quaternion = value;

			// reset the interpolator to the current time frame
			auto interp_linear = CAST_PTR(c_interp_linear<4>*, m_members.interpolator);
			interp_linear->Begin(change_time);
		}

		void c_parameter_instance::SetOverrideInterp(const real_argb_color& value, real change_time)
		{
			YELO_ASSERT_DISPLAY(m_members.m_flags.is_overriden, "parameter instance not set up to be overriden");
			YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type == Enums::_shader_variable_base_type_argb_color, "parameter being overriden with an incorrect type");

			// set the start value using the current values and interpolation amount
			SetStartValueToCurrent();

			// set the target value
			*m_members.m_values[1].color = value;

			// reset the interpolator to the current time frame
			auto interp_linear = CAST_PTR(c_interp_linear<4>*, m_members.interpolator);
			interp_linear->Begin(change_time);
		}

		void c_parameter_instance::SetStartValueToCurrent()
		{
			switch(m_members.parameter->value_type.type)
			{
			default:
				return;
			case Enums::_shader_variable_base_type_boolean:
				InterpolateValues<1>(m_members.m_values[0].boolean,
					m_members.m_values[1].boolean,
					m_members.interpolator->GetValues(),
					m_members.m_values[0].boolean);
				break;
			case Enums::_shader_variable_base_type_integer:
				InterpolateValues<1>(m_members.m_values[0].integer32,
					m_members.m_values[1].integer32,
					m_members.interpolator->GetValues(),
					m_members.m_values[0].integer32);
				break;
			case Enums::_shader_variable_base_type_float:
				switch(m_members.parameter->value_type.count)
				{
				case 1:
					InterpolateValues<1>(m_members.m_values[0].real32,
						m_members.m_values[1].real32,
						m_members.interpolator->GetValues(),
						m_members.m_values[0].real32);
					break;
				case 2:
					InterpolateValues<2>(m_members.m_values[0].real32,
						m_members.m_values[1].real32,
						m_members.interpolator->GetValues(),
						m_members.m_values[0].real32);
					break;
				case 3:
					InterpolateValues<3>(m_members.m_values[0].real32,
						m_members.m_values[1].real32,
						m_members.interpolator->GetValues(),
						m_members.m_values[0].real32);
					break;
				case 4:
					InterpolateValues<4>(m_members.m_values[0].real32,
						m_members.m_values[1].real32,
						m_members.interpolator->GetValues(),
						m_members.m_values[0].real32);
					break;
				default:
					YELO_ASSERT_DISPLAY(m_members.parameter->value_type.count != 0, "invalid value type count");
					break;
				};
			case Enums::_shader_variable_base_type_argb_color:
				InterpolateValues<4>(m_members.m_values[0].real32,
					m_members.m_values[1].real32,
					m_members.interpolator->GetValues(),
					m_members.m_values[0].real32);
				break;
			case Enums::_shader_variable_base_type_matrix:
				YELO_ASSERT_DISPLAY(m_members.parameter->value_type.type != Enums::_shader_variable_base_type_matrix, "scripted interpolation of matrices is not supported");
				return;
			}
		}
	};};};
};
#endif