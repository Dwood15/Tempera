#pragma once

#include <enums/postprocessing.h>
#include <d3dx9effect.h>
#include <vector>
#include "../../c_effect_render_set.hpp"
#include "../../../dx9/shaders/shader_postprocess_generic_definitions.hpp"
#include "../../../dx9/shaders/effect_postprocess_generic_definitions.hpp"
#include "../../../dx9/shaders/shader_postprocess_definitions.hpp"
#include "c_effect_external.hpp"
#include "s_effect_postprocess_external.hpp"
#include "../c_shader_instance_generic.hpp"
#include "../../../../../memory/linked_list.hpp"
#include "c_shader_external.hpp"
#include "../c_effect_instance_generic.hpp"

namespace Yelo {
	namespace Enums {
		enum shader_model_mask {
			_shader_model_mask_1_0 = 1 << 0,
			_shader_model_mask_2_0 = 1 << 1,
			_shader_model_mask_3_0 = 1 << 2,

			_shader_model_mask
		};
	};
	namespace Rasterizer::PostProcessing::Generic::External {
		extern cstring K_EXTERNAL_PP_SETTINGS_FILE;

		class c_external_pp_settings_container;

		class c_shader_container;

		class c_effect_container;

		class c_effect_instance_container;

		class c_system_external
			: public IPostProcessingComponent, public IPostProcessingRenderable, public IPostProcessingUpdatable {
		private:
			class s_parameter_handle : public LinkedListNode<s_parameter_handle> {
			public:
				D3DXHANDLE                      handle;
				TagGroups::shader_variable_type type;
			};

			/////////////////////////////////////////////////
			// static members
		private:
			static c_system_external g_external_system;

			/////////////////////////////////////////////////
			// static member accessors
		public:
			static c_system_external &Instance();

			/////////////////////////////////////////////////
			// members
			struct s_shader_set {
				TagGroups::s_shader_postprocess_generic *definition;
				c_shader_external                       *shader;
			};
			struct s_effect_set {
				s_effect_postprocess_external *definition;
				c_effect_external             *effect;
				uint32                        shader_instance_count;
				c_shader_instance_generic     *shader_instances;
			};
			struct s_effect_instance_set {
				TagGroups::s_effect_postprocess_generic_effect_instance *definition;
				c_effect_instance_generic                               *instance;
			};
		private:
			struct {
				struct {
					bool is_ready;
					bool is_unloaded;
					bool is_enabled;
					PAD8;
				} m_flags;

				Enums::pp_component_status status;
				short : 16;
			} m_members;

			struct {
				c_effect_render_set m_render_sets[5];

				struct {
					char shaders_path[MAX_PATH];
					char textures_path[MAX_PATH];
				}                   m_paths;

				struct {
					uint32       count;
					s_shader_set *shader_sets;
				}                   m_shaders;

				struct {
					uint32       count;
					s_effect_set *effect_sets;
				}                   m_effects;

				struct {
					uint32                count;
					s_effect_instance_set *effect_instance_sets;
				}                   m_effect_instances;
			} m_members_external;

			/////////////////////////////////////////////////
			// member accessors
		public:
			bool &Enabled();

			bool IsReady();

			bool IsUnloaded();

			/////////////////////////////////////////////////
			// IPostProcessingComponent
		public:
			void Initialize();

			void Dispose();

			void InitializeResources_Base(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *parameters);

			void OnLostDevice_Base();

			void OnResetDevice_Base(D3DPRESENT_PARAMETERS *parameters);

			void ReleaseResources_Base();

			void Unload();

			void Load();

			/////////////////////////////////////////////////
			// IPostProcessingRenderable
		public:
			bool Render(Enums::postprocess_render_stage render_stage);

			/////////////////////////////////////////////////
			// IPostProcessingUpdatable
		public:
			void Update(real delta_time);

			/////////////////////////////////////////////////
			// system setup
		private:
			HRESULT LoadExternal();

			void UnloadExternal();

			HRESULT LoadShaders();

			void UnloadShaders();

			HRESULT LoadEffects();

			void UnloadEffects();

			bool ReadUserPaths(const c_external_pp_settings_container &settings);

			void ResetUserPaths();

			bool ReadShaders(const std::vector<c_shader_container> &shaders);

			void DeleteShaders();

			bool ReadEffects(const std::vector<c_effect_container> &effects);

			void DeleteEffects();

			bool ReadEffectInstances(const std::vector<c_effect_instance_container> &effect_instances);

			void DeleteEffectInstances();

			/////////////////////////////////////////////////
			// shader definition
			void BuildShaderDefinition(TagGroups::s_shader_postprocess_generic *definition, const char *shader_path);

			void BuildTechnique(LPD3DXEFFECTCOMPILER compiler, D3DXHANDLE handle, TagGroups::s_technique_definition &technique);

			void BuildPass(LPD3DXEFFECTCOMPILER compiler, D3DXHANDLE handle, TagGroups::s_pass_definition &pass);

			void BuildParameters(LPD3DXEFFECTCOMPILER compiler, TagGroups::s_shader_postprocess_generic *definition);

			void DestroyShaderDefinition(TagGroups::s_shader_postprocess_generic *definition);

			void DestroyTechnique(TagGroups::s_technique_definition &technique);

			/////////////////////////////////////////////////
			// effect definition
			void BuildEffectDefinition(s_effect_postprocess_external *definition, const c_effect_container &effect);

			void DestroyEffectDefinition(s_effect_postprocess_external *definition);

			/////////////////////////////////////////////////
			// effect instance definition
			void BuildEffectInstanceDefinition(TagGroups::s_effect_postprocess_generic_effect_instance *definition,
														  const c_effect_instance_container &effect_instance);

			void DestroyEffectInstanceDefinition(TagGroups::s_effect_postprocess_generic_effect_instance *definition);

			/////////////////////////////////////////////////
			// shader parameters
			uint32 GetParameterHandles(LPD3DXEFFECTCOMPILER compiler,
												s_parameter_handle *&list,
												const char *semantic_format,
												const uint32 count,
												const int16 parameter_type,
												const int16 parameter_type_count);

			void SetupParameter(LPD3DXEFFECTCOMPILER compiler,
									  const s_parameter_handle &handle,
									  TagGroups::s_shader_postprocess_parameter &parameter);

			void SetupParameterValue(LPD3DXEFFECTCOMPILER compiler,
											 const s_parameter_handle &handle,
											 TagGroups::s_shader_postprocess_parameter &parameter);

			void SetupParameterRuntimeOverride(LPD3DXEFFECTCOMPILER compiler,
														  const s_parameter_handle &handle,
														  TagGroups::s_shader_postprocess_parameter &parameter);

			void SetupParameterAnimation(LPD3DXEFFECTCOMPILER compiler,
												  const s_parameter_handle &handle,
												  TagGroups::s_shader_postprocess_parameter &parameter);

			void SetupParameterBitmap(LPD3DXEFFECTCOMPILER compiler,
											  const s_parameter_handle &handle,
											  TagGroups::s_shader_postprocess_parameter &parameter);

			void DestroyParameter(TagGroups::s_shader_postprocess_parameter &parameter);

			void GetAnnotationValue(LPD3DXEFFECTCOMPILER compiler, D3DXHANDLE variable, const char *annotation, int default_value,
											int &output);

			void SetupRenderSets();

			void ClearRenderSets();

			void SetRenderSet(c_effect_render_set &set, Enums::postprocess_render_stage render_stage);

			void ValidateSystem();

			void UpdateStatus();
		};
	};
};
