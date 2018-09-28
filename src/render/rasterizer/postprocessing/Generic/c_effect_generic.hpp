#pragma once

#include "../c_effect_postprocess.hpp"
#include "c_parameter_instance.hpp"

namespace Yelo::Rasterizer::PostProcessing::Generic {
	class c_effect_generic : public c_effect_postprocess {
		/////////////////////////////////////////////////
		// members
	protected:
		struct {
			TagGroups::s_effect_postprocess_generic *definition;
		} m_members_generic;

		/////////////////////////////////////////////////
		// member accessors
	public:
		void SetEffectDefinition(TagGroups::s_effect_postprocess_definition *definition);

		/////////////////////////////////////////////////
		// initializers
	public:
		void Ctor() {
			c_effect_postprocess::Ctor();
		}

		void Dtor() {
			c_effect_postprocess::Dtor();
		}

		/////////////////////////////////////////////////
		// effect setup
	public:
		void SetupEffect();

	private:
		void SetupExposedParameters();

	protected:
		c_parameter_instance *GetExposedParameter(const char *exposed_name);
	};
};
