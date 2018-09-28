#pragma once

#include "../c_effect_generic.hpp"

namespace Yelo::Rasterizer::PostProcessing::Generic::Internal {
	class c_effect_internal : public c_effect_generic {
		/////////////////////////////////////////////////
		// members
	protected:
		struct {
			TagGroups::s_effect_postprocess_collection_effect *definition;
		} m_members_internal;

		/////////////////////////////////////////////////
		// member accessors
	public:
		void SetEffectReferenceDefinition(TagGroups::s_effect_postprocess_collection_effect *definition);

		const char *GetName();

		int16 GetScriptedVariableIndex(const char *name);

		/////////////////////////////////////////////////
		// initializers
	public:
		void Ctor() {
			c_effect_generic::Ctor();

			m_members_internal.definition = NULL;
		}

		void Dtor() {
			c_effect_generic::Dtor();

			m_members_internal.definition = NULL;
		}

		/////////////////////////////////////////////////
		// effect setup
	public:
		void SetupEffect();

	private:
		void SetupScriptedVariables();

		/////////////////////////////////////////////////
		// scripting
	public:
		void SetScriptedVariable(int16 index, real change_time, bool value);

		void SetScriptedVariable(int16 index, real change_time, int32 value);

		void SetScriptedVariable(int16 index, real change_time, real value0, real value1, real value2, real value3);
	};
};
