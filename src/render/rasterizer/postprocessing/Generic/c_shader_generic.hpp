#pragma once

#include "../../dx9/shaders/shader_postprocess_generic_definitions.hpp"
#include "../c_shader_postprocess.hpp"

namespace Yelo::Rasterizer::PostProcessing::Generic
	{
		enum { 
			k_max_shader_count = 32, 
			k_max_effect_count = 32,
			k_variable_count = 16,
			k_bitmap_count = 4,
		};

		/*!
		 * \brief
		 * Generic shader class for managing a s_shader_postprocess_generic.
		 * 
		 * Generic shader class for managing a s_shader_postprocess_generic.
		 */
		class c_shader_generic : public c_shader_postprocess
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				TagGroups::s_shader_postprocess_generic* definition;
			}m_members_generic;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition);
			TagGroups::s_shader_postprocess_generic* GetShader();

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_shader_postprocess::Ctor();

				m_members_generic.definition = NULL;
			}
			void Dtor()
			{
				c_shader_postprocess::Dtor();

				m_members_generic.definition = NULL;
			}

			/////////////////////////////////////////////////
			// shader setup
		public:
			HRESULT	LoadBitmaps(IDirect3DDevice9* render_device);
			void	UnloadBitmaps();
			void	SetupShader();
		protected:
			void	GetHandles();
			void	ClearHandles();
			bool	ValidateImpl();
		private:
			void	GetParameterHandle(LPD3DXEFFECT effect, TagGroups::s_shader_postprocess_parameter& parameter);
			void	ClearParameterHandle(TagGroups::s_shader_postprocess_parameter& parameter);
			void	SetParameterVariable(LPD3DXEFFECT effect, TagGroups::s_shader_postprocess_parameter& parameter);

			/////////////////////////////////////////////////
			// shader application
		public:
			void	SetVariables();
		};
	};
