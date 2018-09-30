#pragma once

#include "../dx9/shaders/shader_postprocess_definitions.hpp"
#include "c_quad_manager.h"
#include "ShaderSources/c_shader_data_base.hpp"

namespace Yelo::Rasterizer::PostProcessing {
	class c_shader_postprocess {
		/////////////////////////////////////////////////
		// members
	protected:
		struct {
			TagGroups::s_shader_postprocess_definition *definition;
			c_shader_data_base                         *source_data;
		} m_members;

	private:
		void ClearMembers() {
			m_members.definition  = nullptr;
			m_members.source_data = nullptr;
		}

		/////////////////////////////////////////////////
		// member accessors
	public:
		virtual void SetShaderDefinition(TagGroups::s_shader_postprocess_definition *definition);

		template <class T>
		T *GetShaderDefinition() { return CAST_PTR(T*, m_members.definition); }

		void SetSourceData(c_shader_data_base *source);

		LPD3DXEFFECT GetEffect();

		bool IsValid();

		/////////////////////////////////////////////////
		// initializers
	public:
		virtual void Ctor() {
			ClearMembers();
		}

		virtual void Dtor() {
			ClearMembers();
		}

		/////////////////////////////////////////////////
		// shader setup
	public:
		virtual void SetupShader() {}

		HRESULT LoadShader(IDirect3DDevice9 *render_device);

		void UnloadShader();

		void OnDeviceLost();

		HRESULT OnDeviceReset();

		void Validate();

	protected:
		virtual void GetHandles();

		virtual void ClearHandles();

		virtual bool ValidateImpl();

	private:
		HRESULT LoadShaderImpl(IDirect3DDevice9 *render_device);

		void UnloadShaderImpl();

		void OnDeviceLostImpl();

		HRESULT OnDeviceResetImpl();

		bool ValidateShaderVersions();

		TagGroups::s_technique_definition *
		GetActiveTechnique();

		D3DXHANDLE GetTechniqueHandle();

		/////////////////////////////////////////////////
		// shader application
	public:
		virtual void SetVariables();

		virtual void UpdateVariables();

		HRESULT Render(IDirect3DDevice9 *render_device, c_quad_instance *render_quad);
	};
};
