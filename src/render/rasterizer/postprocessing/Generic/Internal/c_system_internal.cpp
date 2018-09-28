/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_system_internal.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/Generic/Internal/c_settings_internal.hpp"
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/Fade/c_system_fade.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic { namespace Internal
	{
		/////////////////////////////////////////////////
		// static members
		c_system_internal c_system_internal::g_internal_system;

		/////////////////////////////////////////////////
		// static member accessors
		c_system_internal& c_system_internal::Instance()
		{
			return g_internal_system;
		}

		/////////////////////////////////////////////////
		// member accessors
		bool& c_system_internal::Enabled()
		{
			return m_members.m_flags.is_enabled;
		}

		bool c_system_internal::IsReady()
		{
			return m_members.m_flags.is_ready;
		}

		bool c_system_internal::IsUnloaded()
		{
			return m_members.m_flags.is_unloaded;
		}

		void c_system_internal::ClearMembers()
		{
			// initialize the systems variables to defaults
			m_members.status = Enums::pp_component_status_uninitialised;

			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_unloaded = false;

			// delete allocated memory and/or initialize values to defaults
			ClearShaderCollection();
			ClearInternalShaders();
			ClearInternalEffects();
		}

		/////////////////////////////////////////////////
		// IPostProcessingComponent
		/*!
		 * \brief
		 * Resets the system to defaults.
		 * 
		 * Resets the system to defaults.
		 */
		void c_system_internal::Initialize()
		{
			c_settings_internal::Register(Settings::Manager());

			ClearMembers();
		}

		/*!
		 * \brief
		 * Resets the system to defaults, releasing any allocated memory.
		 * 
		 * Resets the system to defaults, releasing any allocated memory.
		 */
		void c_system_internal::Dispose()
		{
			ClearMembers();

			c_settings_internal::Unregister(Settings::Manager());
		}

		/*!
		 * \brief
		 * Runs device lost logic of all the direct3d resources.
		 * 
		 * Runs device lost logic of all the direct3d resources.
		 */
		void c_system_internal::OnLostDevice_Base()
		{
			// if the system is unloaded the shaders have been released
			if(m_members.m_flags.is_unloaded)
				return;

			// do device lost logic on the shaders
			for(uint32 i = 0; i < m_members_internal.m_shaders.count; i++)
			{
				m_members_internal.m_shaders.shader_list[i].OnDeviceLost();
				m_members_internal.m_shaders.shader_list[i].Validate();
			}

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Runs device reset logic of all the direct3d resources.
		 * 
		 * Runs device reset logic of all the direct3d resources.
		 */
		void c_system_internal::OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters)
		{
			// if the system is unloaded the shaders have been released
			if(m_members.m_flags.is_unloaded)
				return;

			HRESULT hr = S_OK;
			// do device reset logic on the shaders
			for(uint32 i = 0; i < m_members_internal.m_shaders.count; i++)
			{
				hr |= m_members_internal.m_shaders.shader_list[i].OnDeviceReset();
				m_members_internal.m_shaders.shader_list[i].Validate();
			}

			if(SUCCEEDED(hr)) { m_members.status = Enums::pp_component_status_initialised; }
			else { m_members.status = Enums::pp_component_status_initialisation_failed; }

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Releases all of the systems Direct3D resources.
		 * 
		 * Releases all of the systems Direct3D resources.
		 */
		void c_system_internal::Unload()
		{
			UnloadEffects();
			UnloadShaders();
			ValidateSystem();
			ClearRenderSets();

			m_members.status = Enums::pp_component_status_uninitialised;
			m_members.m_flags.is_unloaded = true;

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Loads all of the systems Direct3D resources.
		 * 
		 * Loads all of the systems Direct3D resources.
		 */
		void c_system_internal::Load()
		{
			HRESULT hr = S_OK;

			// create the direct3d resources of the shader
			if(m_members.m_flags.is_unloaded)
			{
				// create all direct3d resources
				do
				{
					if(FAILED(hr |= LoadShaders())) break;
					if(FAILED(hr |= LoadEffects())) break;
				}while(false);

				ValidateSystem();
				SetupRenderSets();

				if(SUCCEEDED(hr)) { m_members.status = Enums::pp_component_status_initialised; }
				else { m_members.status = Enums::pp_component_status_initialisation_failed; }

				m_members.m_flags.is_unloaded = false;
			}

			UpdateStatus();
		}

		/////////////////////////////////////////////////
		// IPostProcessingRenderable
		/*!
		 * \brief
		 * Applies the effects for a specified render stage.
		 * 
		 * \param render_stage
		 * The current render stage.
		 * 
		 * \returns
		 * False if no effects were applied.
		 * 
		 * Applies the effects for a specified render stage.
		 */
		bool c_system_internal::Render(Enums::postprocess_render_stage render_stage)
		{
			if((render_stage > Enums::_postprocess_render_stage_post_ui) || !m_members.m_flags.is_enabled)
				return false;

			c_post_processing_main::s_post_processing_globals& globals = c_post_processing_main::Instance().Globals();

			// render all post processes for the current render point
			return m_members_internal.m_render_sets[render_stage].RenderSet(globals.render_device);
		}

		/////////////////////////////////////////////////
		// IPostProcessingUpdatable
		/*!
		 * \brief
		 * Updates all effects.
		 * 
		 * \param delta_time
		 * The time in seconds that passed since the last update.
		 * 
		 * Updates all effects.
		 */
		void c_system_internal::Update(real delta_time)
		{
			uint32 i = 0;
			// update the standard variables in each shader that can change each frame
			for(i = 0; i < m_members_internal.m_shaders.count; i++)
				m_members_internal.m_shaders.shader_list[i].UpdateVariables();

			// update time dependent elements of the effects
			for(i = 0; i < m_members_internal.m_effects.count; i++)
				m_members_internal.m_effects.effect_list[i].effect->Update(delta_time);

			// update time dependent elements of the effect instances
			for(i = 0; i < m_members_internal.m_effect_instances.count; i++)
				m_members_internal.m_effect_instances.effect_instance_list[i].UpdateEffectInstance(delta_time);
		}

		/////////////////////////////////////////////////
		// IPostProcessingCacheComponent
		/*!
		 * \brief
		 * Initializes the systems map dependent resources.
		 * 
		 * Initializes the systems map dependent resources.
		 */
		void c_system_internal::Initialize_Cache()
		{
			// get the maps shader collection if present
			GetShaderCollection();

			if(!m_members_internal.cache_shader_collection)
				return;

			if((m_members_internal.cache_shader_collection->shaders.Count == 0) ||
				(m_members_internal.cache_shader_collection->effects.Count == 0) ||
				(m_members_internal.cache_shader_collection->effect_instances.Count == 0))
				return;

			// create all shader and effects but do not create the direct3d resources
			GetInternalShaders();
			GetInternalEffects();
		}

		/*!
		 * \brief
		 * Disposes of the systems map dependent resources.
		 * 
		 * Disposes of the systems map dependent resources.
		 */
		void c_system_internal::Dispose_Cache()
		{
			// delete all shaders and effects, direct3d resources should have been released before this
			ClearShaderCollection();
			ClearInternalShaders();
			ClearInternalEffects();
		}

		/*!
		 * \brief
		 * Initializes the systems map dependent Direct3D resources.
		 * 
		 * Initializes the systems map dependent Direct3D resources.
		 */
		void c_system_internal::InitializeResources_Cache()
		{
			HRESULT hr = S_OK;

			// create the direct3d resources of the shader
			if(!m_members.m_flags.is_unloaded)
			{
				// create all direct3d resources
				do
				{
					if(FAILED(hr |= LoadShaders())) break;
					if(FAILED(hr |= LoadEffects())) break;
				}while(false);

				ValidateSystem();
				SetupRenderSets();
			}
			if(SUCCEEDED(hr)) { m_members.status = Enums::pp_component_status_initialised; }
			else { m_members.status = Enums::pp_component_status_initialisation_failed; }

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Releases the systems map dependent Direct3D resources.
		 * 
		 * Releases the systems map dependent Direct3D resources.
		 */
		void c_system_internal::ReleaseResources_Cache()
		{
			// destroy all direct3d resources
			UnloadEffects();
			UnloadShaders();
			ValidateSystem();
			ClearRenderSets();

			m_members.status = Enums::pp_component_status_uninitialised;

			UpdateStatus();
		}

		/////////////////////////////////////////////////
		// system setup
		/*!
		 * \brief
		 * Gets a maps shader collection tag pointer if present.
		 * 
		 * Gets a maps shader collection tag pointer if present.
		 */
		void c_system_internal::GetShaderCollection()
		{
			// look for a shader_postprocess_collection tag in the cache
			// get the first instance (there can be only one </highlander>, others will be ignored)
			// TODO: use new s_tag_iterator wrapper
			TagGroups::s_tag_iterator iter;
			blam::tag_iterator_new(iter, TagGroups::s_effect_postprocess_collection::k_group_tag);
			datum_index tag_index = blam::tag_iterator_next(iter);
			
			// set the current shader collection tag reference to the tag found
			if(!tag_index.IsNull())
				m_members_internal.cache_shader_collection = TagGroups::TagGetForModify<TagGroups::s_effect_postprocess_collection>(tag_index);
		}

		/*!
		 * \brief
		 * Clears the shader collection tag pointer.
		 * 
		 * Clears the shader collection tag pointer.
		 */
		void c_system_internal::ClearShaderCollection()
		{
			// null the shader collection pointer
			m_members_internal.cache_shader_collection = nullptr;
		}

		/*!
		 * \brief
		 * Load all of the shaders references in the collection tag.
		 * 
		 * Load all of the shaders references in the collection tag. For each tag a shader class is
		 * created, which are instanced by the effect class.
		 * 
		 * \see
		 * c_shader_internal | s_shader_postprocess_generic
		 */
		void c_system_internal::GetInternalShaders()
		{
			// the shader list in the collection tag is populated at build time by copying shader references from the effect tag

			// create the shader list
			m_members_internal.m_shaders.count = (uint16)m_members_internal.cache_shader_collection->shaders.Count;
			m_members_internal.m_shaders.shader_list = new c_shader_internal[m_members_internal.m_shaders.count];

			// set up each shader
			for(uint32 i = 0; i < m_members_internal.m_shaders.count; i++)
			{
				// initialise the shader class
				m_members_internal.m_shaders.shader_list[i].Ctor();

				datum_index shader_index = m_members_internal.cache_shader_collection->shaders[i].tag_index;
				if (TagGroups::TagIsInstanceOf<TagGroups::s_shader_postprocess_generic>(shader_index))
				{
					auto shpg = TagGroups::TagGetForModify<TagGroups::s_shader_postprocess_generic>(shader_index);

					m_members_internal.m_shaders.shader_list[i].SetShaderDefinition(shpg);
					m_members_internal.m_shaders.shader_list[i].SetDatumIndex(shader_index);
					m_members_internal.m_shaders.shader_list[i].SetShaderName(blam::tag_get_name(shader_index));
					m_members_internal.m_shaders.shader_list[i].SetupShader();
				}
				else
					YELO_ASSERT_DISPLAY(false, "internal shader reference is not a shader_postprocess_generic");
			}
		}

		void c_system_internal::ClearInternalShaders()
		{
			// destroy each shader
			for(uint32 i = 0; i < m_members_internal.m_shaders.count; i++)
				m_members_internal.m_shaders.shader_list[i].Dtor();

			// delete the shader list
			delete [] m_members_internal.m_shaders.shader_list;
			m_members_internal.m_shaders.shader_list = nullptr;
			m_members_internal.m_shaders.count = 0;
		}

		void c_system_internal::GetInternalEffects()
		{
			// create the effect list
			m_members_internal.m_effects.count = (uint16)m_members_internal.cache_shader_collection->effects.Count;
			m_members_internal.m_effects.effect_list = new s_effect_set[m_members_internal.m_effects.count];

			// create the effect instance list
			m_members_internal.m_effect_instances.count = (uint16)m_members_internal.cache_shader_collection->effect_instances.Count;
			m_members_internal.m_effect_instances.effect_instance_list = new c_effect_instance_generic[m_members_internal.m_effect_instances.count];

			// set up each effect
			for(uint32 i = 0; i < m_members_internal.m_effects.count; i++)
				SetupEffect(m_members_internal.m_effects.effect_list[i], &m_members_internal.cache_shader_collection->effects[i]);

			// set up each effect instance
			for(uint32 i = 0; i < m_members_internal.m_effect_instances.count; i++)
				SetupEffectInstance(&m_members_internal.m_effect_instances.effect_instance_list[i], &m_members_internal.cache_shader_collection->effect_instances[i]);
		}

		void c_system_internal::ClearInternalEffects()
		{
			// destroy the effect instances and delete allocated effect instance memory
			for(uint32 i = 0; i < m_members_internal.m_effect_instances.count; i++)
				DestroyEffectInstance(&m_members_internal.m_effect_instances.effect_instance_list[i]);

			// destroy the effects and delete allocated effect memory
			for(uint32 i = 0; i < m_members_internal.m_effects.count; i++)
				DestroyEffect(m_members_internal.m_effects.effect_list[i]);

			// delete the effect instance list
			delete [] m_members_internal.m_effect_instances.effect_instance_list;
			m_members_internal.m_effect_instances.effect_instance_list = NULL;
			m_members_internal.m_effect_instances.count = 0;

			// delete the effect list
			delete [] m_members_internal.m_effects.effect_list;
			m_members_internal.m_effects.effect_list = NULL;
			m_members_internal.m_effects.count = 0;
		}

		HRESULT c_system_internal::LoadShaders()
		{
			HRESULT success = S_OK;
			c_post_processing_main::s_post_processing_globals& globals = c_post_processing_main::Instance().Globals();

			// allocate direct3d resources for all shaders
			for(uint32 i = 0; i < m_members_internal.m_shaders.count; i++)
			{
				success |= m_members_internal.m_shaders.shader_list[i].LoadShader(globals.render_device);
				success |= m_members_internal.m_shaders.shader_list[i].LoadBitmaps(globals.render_device);
			}
			return success;
		}

		void c_system_internal::UnloadShaders()
		{
			// release all shader direct3d resources
			for(uint32 i = 0; i < m_members_internal.m_shaders.count; i++)
				m_members_internal.m_shaders.shader_list[i].UnloadShader();
		}

		HRESULT c_system_internal::LoadEffects()
		{
			HRESULT success = S_OK;

			uint32 i = 0;
			// set up data in the effects that require d3d resources to be allocated first
			for(i = 0; i < m_members_internal.m_effects.count; i++)
				m_members_internal.m_effects.effect_list[i].effect->SetupEffectPostCreation();

			// effect instances should get quad instances here
			for(i = 0; i < m_members_internal.m_effect_instances.count; i++)
				success &= m_members_internal.m_effect_instances.effect_instance_list[i].LoadEffectInstance();
			return success;
		}

		void c_system_internal::UnloadEffects()
		{
			// release all effect instance quad instances
			for(uint32 i = 0; i < m_members_internal.m_effect_instances.count; i++)
				m_members_internal.m_effect_instances.effect_instance_list[i].UnloadEffectInstance();
		}

		void c_system_internal::SetupEffect(s_effect_set& effect_set, TagGroups::s_effect_postprocess_collection_effect* definition)
		{
			auto effect_definition = TagGroups::TagGetForModify<TagGroups::s_effect_postprocess_generic>(definition->effect.tag_index);

			YELO_ASSERT_DISPLAY(effect_definition != nullptr, "effect collection block has no effect referenced");

			// create the internal effect class and initialize it
			effect_set.effect = new c_effect_internal();
			effect_set.effect->Ctor();
			effect_set.effect->SetEffectDefinition(effect_definition);
			// the scripted variables are not defined in a derived struct so must be referenced seperately
			effect_set.effect->SetEffectReferenceDefinition(definition);

			effect_set.shader_instance_count = 0;
			effect_set.shader_instances = nullptr;

			if(effect_definition->shader_indices.Count == 0)
				return;

			// create the effects shader instance objects
			effect_set.shader_instance_count = effect_definition->shader_indices.Count;
			effect_set.shader_instances = new c_shader_instance_generic[effect_definition->shader_indices.Count];

			// set up each instance and add it to the effect
			for(uint32 i = 0; i < effect_set.shader_instance_count; i++)
			{
				// since the shaders are loaded in a list created from all the effects
				// we need to use the datum index to find the shader in the list
				effect_set.shader_instances[i].Ctor();
				effect_set.shader_instances[i].SetShader(GetShaderByIndex(effect_definition->shaders[effect_definition->shader_indices[i]].tag_index));
				effect_set.shader_instances[i].SetupShaderInstance();
				effect_set.effect->AddShaderInstance(&effect_set.shader_instances[i]);
			}

			effect_set.effect->SetupEffect();
		}

		void c_system_internal::DestroyEffect(s_effect_set& effect_set)
		{
			// destroy each shader instance
			for(uint32 i = 0; i < effect_set.shader_instance_count; i++)
				effect_set.shader_instances[i].Dtor();

			// delete the effects shader instance list
			delete [] effect_set.shader_instances;
			effect_set.shader_instances = nullptr;
			effect_set.shader_instance_count = 0;

			// destroy the effect and delete it
			effect_set.effect->Dtor();
			delete effect_set.effect;
			effect_set.effect = nullptr;
		}

		void c_system_internal::SetupEffectInstance(c_effect_instance_generic* instance, TagGroups::s_effect_postprocess_generic_effect_instance* definition)
		{
			//initialise the effect instance and set its effect source and definition
			instance->Ctor();
			instance->SetEffect(m_members_internal.m_effects.effect_list[definition->effect_index].effect);
			instance->SetEffectInstanceDefinition(definition);
			instance->SetupEffectInstance();
		}

		void c_system_internal::DestroyEffectInstance(c_effect_instance_generic* instance)
		{
			instance->Dtor();
		}

		c_shader_postprocess* c_system_internal::GetShaderByIndex(datum_index index)
		{
			// find an existing shader by its datum index
			for(int i = 0; i < m_members_internal.m_shaders.count; i++)
				if(m_members_internal.m_shaders.shader_list[i].GetDatumIndex() == index)
					return &m_members_internal.m_shaders.shader_list[i];
			return nullptr;
		}

		void c_system_internal::SetupRenderSets()
		{
			// add effect instances to the render sets
			for(int i = 0; i < NUMBEROF(m_members_internal.m_render_sets); i++)
			{
				m_members_internal.m_render_sets[i].Ctor();
				SetRenderSet(m_members_internal.m_render_sets[i], (Enums::postprocess_render_stage)i);
			}
		}

		void c_system_internal::ClearRenderSets()
		{
			// reset the render sets to empty
			for (auto& render_set : m_members_internal.m_render_sets)
				render_set.Dtor();
		}

		void c_system_internal::SetRenderSet(c_effect_render_set& set, Enums::postprocess_render_stage render_stage)
		{
			// get the number of effects for this render stage
			uint32 count = 0;
			for(uint32 i = 0; i < m_members_internal.m_effect_instances.count; i++)
				if(m_members_internal.m_effect_instances.effect_instance_list[i].GetRenderStage() == render_stage)
					count++;

			// set the number of effects in the render set
			set.SetCount(count);

			if(count == 0)
				return;

			// add each effect instance for this render stage to the render set
			count = 0;
			for(uint32 i = 0; i < m_members_internal.m_effect_instances.count; i++)
				if(m_members_internal.m_effect_instances.effect_instance_list[i].GetRenderStage() == render_stage)
				{
					set.SetEffectInstance(count, &m_members_internal.m_effect_instances.effect_instance_list[i]);
					count++;
				}
		}

		void c_system_internal::ValidateSystem()
		{
			uint32 i = 0;
			for(i = 0; i < m_members_internal.m_shaders.count; i++)
				m_members_internal.m_shaders.shader_list[i].Validate();
			for(i = 0; i < m_members_internal.m_effects.count; i++)
				m_members_internal.m_effects.effect_list[i].effect->Validate();
			for(i = 0; i < m_members_internal.m_effect_instances.count; i++)
				m_members_internal.m_effect_instances.effect_instance_list[i].Validate();
		}

		void c_system_internal::UpdateStatus()
		{
			m_members.m_flags.is_ready = false;
			do
			{
				if(m_members.status != Enums::pp_component_status_initialised) break;
				if(!Fade::c_system_fade::Instance().IsReady()) break;

				m_members.m_flags.is_ready = true;
			}while(false);
		}

		/////////////////////////////////////////////////
		// scripting
		void c_system_internal::SetEffectInstanceActive(int16 index, bool active)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return;

			m_members_internal.m_effect_instances.effect_instance_list[index].SetIsActive(active);
		}

		void c_system_internal::SetEffectInstanceFade(int16 index, real start, real end, real time)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return;

			m_members_internal.m_effect_instances.effect_instance_list[index].SetEffectFade(start, end, time);
		}

		int16 c_system_internal::GetEffectIndexByName(const char* name)
		{
			for(int16 i = 0; i < m_members_internal.m_effects.count; i++)
				if(strcmp(name, m_members_internal.m_effects.effect_list[i].effect->GetName()) == 0)
					return i;
			return NONE;
		}

		int16 c_system_internal::GetEffectInstanceIndexByName(const char* name)
		{
			for(int16 i = 0; i < m_members_internal.m_effect_instances.count; i++)
				if(strcmp(name, m_members_internal.m_effect_instances.effect_instance_list[i].GetName()) == 0)
					return i;
			return NONE;
		}

		real c_system_internal::GetEffectInstanceCurrentFade(int16 index)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return false;

			return m_members_internal.m_effect_instances.effect_instance_list[index].GetCurrentFade();
		}

		int16 c_system_internal::GetEffectInstanceFadeDirection(int16 index)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return false;

			return m_members_internal.m_effect_instances.effect_instance_list[index].GetFadeDirection();
		}

		bool c_system_internal::GetEffectIsValid(int16 index)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return false;

			return m_members_internal.m_effects.effect_list[index].effect->IsValid();
		}

		int16 c_system_internal::GetEffectShaderVariableIndexByName(int16 index, const char* name)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return NONE;

			return m_members_internal.m_effects.effect_list[index].effect->GetScriptedVariableIndex(name);
		}

		void c_system_internal::SetEffectShaderVariableBoolean(int16 index, int16 variable_index, bool value, real time)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return;
			m_members_internal.m_effects.effect_list[index].effect->SetScriptedVariable(variable_index, time, value);
		}

		void c_system_internal::SetEffectShaderVariableInteger(int16 index, int16 variable_index, int32 value, real time)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return;
			m_members_internal.m_effects.effect_list[index].effect->SetScriptedVariable(variable_index, time, value);
		}

		void c_system_internal::SetEffectShaderVariableReal(int16 index, int16 variable_index, real value0, real value1, real value2, real value3, real time)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return;
			m_members_internal.m_effects.effect_list[index].effect->SetScriptedVariable(variable_index, time, value0, value1, value2, value3);
		}

		void c_system_internal::SetEffectShaderInstanceActive(int16 index, int16 instance_index, bool active)
		{
			if((index < 0) || (index >= (int16)m_members_internal.m_effect_instances.count))
				return;

			if((instance_index < 0) || (instance_index >= (int16)m_members_internal.m_effects.effect_list[index].shader_instance_count))
				return;

			m_members_internal.m_effects.effect_list[index].shader_instances[instance_index].IsActive() = active;
		}
	};};};};
};
#endif