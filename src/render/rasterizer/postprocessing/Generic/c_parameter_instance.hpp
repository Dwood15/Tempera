#pragma once

#include "../../../../gamestate/time/interpolation/i_interpolator.h"

namespace Yelo::Rasterizer::PostProcessing::Generic {
	typedef void (*t_evaluate_runtime_value)(void *);

	class c_parameter_instance {
		/////////////////////////////////////////////////
		// members
		struct {
			struct {
				bool is_overriden;
				char  : 8;
				short : 16;
			} m_flags;

			Enums::parameter_process_type parameter_type;
			short : 16;

			TagGroups::s_shader_postprocess_parameter *parameter;

			Time::Interpolation::i_interpolator *interpolator;

			t_evaluate_runtime_value evaluate_runtime_value;

			union {
				void            *pointer;
				bool            *boolean;
				int32           *integer32;
				real            *real32;
				real_vector2d   *vector2d;
				real_vector3d   *vector3d;
				real_quaternion *quaternion;
				real_argb_color *color;
			} m_values[2];
		} m_members;

		/////////////////////////////////////////////////
		// member accessors
	public:
		TagGroups::s_shader_postprocess_parameter *GetParameter();

		void SetParameter(TagGroups::s_shader_postprocess_parameter *parameter);

		/////////////////////////////////////////////////
		// initializers
	public:
		void Ctor() {
			m_members.parameter_type         = Enums::_parameter_process_type_none;
			m_members.m_flags.is_overriden   = false;
			m_members.parameter              = NULL;
			m_members.interpolator           = NULL;
			m_members.evaluate_runtime_value = NULL;

			m_members.m_values[0].pointer = NULL;
			m_members.m_values[1].pointer = NULL;
		}

		void Dtor() {
			m_members.parameter_type       = Enums::_parameter_process_type_none;
			m_members.m_flags.is_overriden = false;
			m_members.parameter            = NULL;

			DeleteInterpolator();

			m_members.evaluate_runtime_value = NULL;

			DeleteValueMemory();
		}

		/////////////////////////////////////////////////
		// parameter instance setup
	public:
		virtual void SetupParameterInstance();

		void SetupParameterOverride();

	private:
		void SetupStaticValue();

		void SetupAnimatedValue();

		void SetupRuntimeValue();

		void SetupTextureValue();

		void CreateInterpolatorFunction();

		void CreateInterpolatorLinear();

		void DeleteInterpolator();

		template <class T>
		void AllocateValueMemory();

		void DeleteValueMemory();

		/////////////////////////////////////////////////
		// parameter instance application
	public:
		virtual void Update(real delta_time);

		virtual void SetParameterVariable(LPD3DXEFFECT effect);

		/////////////////////////////////////////////////
		// parameter instance override application
	public:
		void SetOverrideInterp(bool value, real change_time);

		void SetOverrideInterp(int32 value, real change_time);

		void SetOverrideInterp(real value, real change_time);

		void SetOverrideInterp(const real_vector2d &value, real change_time);

		void SetOverrideInterp(const real_vector3d &value, real change_time);

		void SetOverrideInterp(const real_quaternion &value, real change_time);

		void SetOverrideInterp(const real_argb_color &value, real change_time);

	private:
		void SetStartValueToCurrent();
	};
};
