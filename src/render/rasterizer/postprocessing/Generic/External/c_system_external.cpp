/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/External/c_system_external.hpp"

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/math/periodic_functions.hpp>

#include "Rasterizer/PostProcessing/Generic/External/c_settings_external.hpp"
#include "Rasterizer/PostProcessing/PostProcessingErrorReporting.hpp"
#include "Rasterizer/PostProcessing/c_post_processing_main.hpp"
#include "Rasterizer/PostProcessing/c_shader_include_manager.hpp"
#include "Game/Console.hpp"

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_container_list.hpp>
#include <YeloLib/configuration/c_configuration_value_list.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/type_containers/c_point2d_container.hpp>
#include <YeloLib/configuration/type_containers/c_real_bounds_container.hpp>

// TODO: finish reimplementing for loops with ranged for loops, where applicable

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic { namespace External
	{
		cstring K_EXTERNAL_PP_SETTINGS_FILE = "OS_Settings.PostProcessing.xml";

#pragma region External PP Settings
		class c_shader_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_id;
			Configuration::c_configuration_value<std::string> m_file;

			c_shader_container()
				: Configuration::c_configuration_container("Shader")
				, m_id("ID", "")
				, m_file("File", "")
			{ }

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override
			{
				return std::vector<Configuration::i_configuration_value* const> { &m_id, &m_file };
			}
		};

		class c_effect_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_id;
			Configuration::c_configuration_value_list<std::string> m_shader_ids;

			c_effect_container()
				: Configuration::c_configuration_container("Effect")
				, m_id("ID", "")
				, m_shader_ids("ShaderID", "")
			{ }

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override
			{
				return std::vector<Configuration::i_configuration_value* const> { &m_id, &m_shader_ids };
			}
		};

		class c_quad_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_point2d_container m_tessellation;
			Configuration::c_real_bounds_container m_bounds_x;
			Configuration::c_real_bounds_container m_bounds_y;

			c_quad_container()
				: Configuration::c_configuration_container("Quad")
				, m_tessellation("Tessellation", 4, 4)
				, m_bounds_x("BoundsX", 0.0f, 1.0f)
				, m_bounds_y("BoundsY", 0.0f, 1.0f)
			{ }

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override
			{
				return std::vector<Configuration::i_configuration_value* const> { &m_tessellation, &m_bounds_x, &m_bounds_y };
			}
		};

		class c_activation_control_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<uint16> m_activation_state;
			Configuration::c_configuration_value<bool> m_invert;

			c_activation_control_container()
				: Configuration::c_configuration_container("ActivationControl")
				, m_activation_state("State", 0)
				, m_invert("Invert", false)
			{ }

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override
			{
				return std::vector<Configuration::i_configuration_value* const> { &m_activation_state, &m_invert };
			}
		};

		class c_effect_instance_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_name;
			Configuration::c_configuration_value<std::string> m_effect;
			Configuration::c_configuration_value<uint16> m_render_stage;
			Configuration::c_configuration_value<uint16> m_activation_operation;
			c_quad_container m_quad;
			Configuration::c_configuration_container_list<c_activation_control_container> m_activation_controls;

			c_effect_instance_container()
				: Configuration::c_configuration_container("EffectInstance")
				, m_name("Name", "")
				, m_effect("Effect", "")
				, m_render_stage("RenderStage", 0)
				, m_activation_operation("ActivationOperation", 0)
				, m_quad()
				, m_activation_controls("ActivationControl", []() { return c_activation_control_container(); })
			{ }

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override
			{
				return std::vector<Configuration::i_configuration_value* const>
				{
					&m_name,
					&m_effect,
					&m_render_stage,
					&m_activation_operation,
					&m_quad,
					&m_activation_controls,
				};
			}
		};

		class c_external_pp_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_shader_directory;
			Configuration::c_configuration_value<std::string> m_texture_directory;
			Configuration::c_configuration_container_list<c_shader_container> m_shaders;
			Configuration::c_configuration_container_list<c_effect_container> m_effects;
			Configuration::c_configuration_container_list<c_effect_instance_container> m_effect_instances;

			c_external_pp_settings_container()
				: Configuration::c_configuration_container("PostProcessing")
				, m_shader_directory("ShaderDirectory", "")
				, m_texture_directory("TextureDirectory", "")
				, m_shaders("Shader", []() { return c_shader_container(); })
				, m_effects("Effect", []() { return c_effect_container(); })
				, m_effect_instances("EffectInstance", []() { return c_effect_instance_container(); })
			{ }

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override
			{
				return std::vector<Configuration::i_configuration_value* const>
				{
					&m_shader_directory,
					&m_texture_directory,
					&m_shaders,
					&m_effects,
					&m_effect_instances
				};
			}
		};
#pragma endregion

		/////////////////////////////////////////////////
		// static members
		c_system_external c_system_external::g_external_system;

		/////////////////////////////////////////////////
		// static member accessors
		c_system_external& c_system_external::Instance()
		{
			return g_external_system;
		}

		/////////////////////////////////////////////////
		// member accessors
		bool& c_system_external::Enabled()
		{
			return m_members.m_flags.is_enabled;
		}

		bool c_system_external::IsReady()
		{
			return m_members.m_flags.is_ready;
		}

		bool c_system_external::IsUnloaded()
		{
			return m_members.m_flags.is_unloaded;
		}

		/////////////////////////////////////////////////
		// IPostProcessingComponent
		/*!
		 * \brief
		 * Resets the system to defaults.
		 * 
		 * Resets the system to defaults.
		 */
		void c_system_external::Initialize()
		{
			c_settings_external::Register(Settings::Manager());

			// initialize the systems variables to defaults
			m_members.status = Enums::pp_component_status_uninitialised;

			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_unloaded = false;

			ResetUserPaths();
		}

		/*!
		 * \brief
		 * Resets the system to defaults, releasing any allocated memory.
		 * 
		 * Resets the system to defaults, releasing any allocated memory.
		 */
		void c_system_external::Dispose()
		{
			// initialize the systems variables to defaults
			m_members.status = Enums::pp_component_status_uninitialised;

			m_members.m_flags.is_ready = false;
			m_members.m_flags.is_unloaded = false;

			// delete allocated memory
			UnloadExternal();

			c_settings_external::Unregister(Settings::Manager());
		}

		/*!
		 * \brief
		 * Loads the external system, and allocates D3D resources.
		 * 
		 * \param parameters
		 * The parameters the render device was created with.
		 * 
		 * Loads the external system, and allocates D3D resources.
		 */
		void c_system_external::InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters)
		{
			HRESULT hr = S_OK;
			// create the direct3d resources of the shader

			do
			{
				if(FAILED(hr |= LoadExternal())) break;
				if(FAILED(hr |= LoadShaders())) break;
				if(FAILED(hr |= LoadEffects())) break;
			}while(false);

			ValidateSystem();

			SetupRenderSets();

			if(SUCCEEDED(hr)) { m_members.status = Enums::pp_component_status_initialised; }
			else { m_members.status = Enums::pp_component_status_initialisation_failed; }

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Runs device lost logic of all the direct3d resources.
		 * 
		 * Runs device lost logic of all the direct3d resources.
		 */
		void c_system_external::OnLostDevice_Base()
		{
			// if the system is unloaded the shaders have been released
			if(m_members.m_flags.is_unloaded)
				return;

			// do device lost logic on the shaders
			for(uint32 i = 0; i < m_members_external.m_shaders.count; i++)
			{
				m_members_external.m_shaders.shader_sets[i].shader->OnDeviceLost();
				m_members_external.m_shaders.shader_sets[i].shader->Validate();
			}

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Runs device reset logic of all the direct3d resources.
		 * 
		 * Runs device reset logic of all the direct3d resources.
		 */
		void c_system_external::OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters)
		{
			// if the system is unloaded the shaders have been released
			if(m_members.m_flags.is_unloaded)
				return;

			HRESULT hr = S_OK;
			// do device reset logic on the shaders
			for(uint32 i = 0; i < m_members_external.m_shaders.count; i++)
			{
				hr |= m_members_external.m_shaders.shader_sets[i].shader->OnDeviceReset();
				m_members_external.m_shaders.shader_sets[i].shader->Validate();
			}

			if(SUCCEEDED(hr)) { m_members.status = Enums::pp_component_status_initialised; }
			else { m_members.status = Enums::pp_component_status_initialisation_failed; }

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Releases D3D resources and unloads the system.
		 * 
		 * Releases D3D resources and unloads the system.
		 */
		void c_system_external::ReleaseResources_Base()
		{
			UnloadEffects();
			UnloadShaders();
			UnloadExternal();
			ValidateSystem();
			ClearRenderSets();

			m_members.status = Enums::pp_component_status_uninitialised;

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Releases all of the systems Direct3D resources and unloads the effect tree.
		 * 
		 * Releases all of the systems Direct3D resources and unloads the effect tree.
		 */
		void c_system_external::Unload()
		{
			UnloadEffects();
			UnloadShaders();
			UnloadExternal();
			ValidateSystem();
			ClearRenderSets();

			m_members.status = Enums::pp_component_status_uninitialised;
			m_members.m_flags.is_unloaded = true;

			UpdateStatus();
		}

		/*!
		 * \brief
		 * Reloads the effect tree and allocates D3D resources.
		 * 
		 * Reloads the effect tree and allocates D3D resources.
		 */
		void c_system_external::Load()
		{
			HRESULT hr = S_OK;

			// create the direct3d resources of the system
			if(m_members.m_flags.is_unloaded)
			{
				do
				{
					if(FAILED(hr |= LoadExternal())) break;
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
		bool c_system_external::Render(Enums::postprocess_render_stage render_stage)
		{
			if((render_stage > Enums::_postprocess_render_stage_post_ui) || !m_members.m_flags.is_enabled)
				return false;

			c_post_processing_main::s_post_processing_globals& globals = c_post_processing_main::Instance().Globals();

			// render all post processes for the current render point
			return m_members_external.m_render_sets[render_stage].RenderSet(globals.render_device);
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
		void c_system_external::Update(real delta_time)
		{
			uint32 i = 0;
			// update the standard variables in each shader that can change each frame
			for(i = 0; i < m_members_external.m_shaders.count; i++)
				m_members_external.m_shaders.shader_sets[i].shader->UpdateVariables();

			// update time dependent elements of the effects
			for(i = 0; i < m_members_external.m_effects.count; i++)
				m_members_external.m_effects.effect_sets[i].effect->Update(delta_time);

			// update time dependent elements of the effect instances
			for(i = 0; i < m_members_external.m_effect_instances.count; i++)
				m_members_external.m_effect_instances.effect_instance_sets[i].instance->UpdateEffectInstance(delta_time);
		}

		/////////////////////////////////////////////////
		// system setup
		/*!
		 * \brief
		 * Reads the external system settings and creates the shaders and effects.
		 * 
		 * \returns
		 * Non-null if an error occured.
		 * 
		 * Reads the external system settings and creates the shaders and effects.
		 */
		HRESULT c_system_external::LoadExternal()
		{
			// load the xml file
			char file_path[MAX_PATH];
			if( !Settings::GetSettingsFilePath(K_EXTERNAL_PP_SETTINGS_FILE, file_path))
			{
				return E_FAIL;
			}

			auto config_file = Configuration::c_configuration_file_factory::CreateConfigurationFile(file_path);
			if(!config_file->Load())
			{
				return E_FAIL;
			}

			auto root_node = config_file->Root();
			auto pp_node = root_node->GetChild("OpenSauce.PostProcessing");

			if(!pp_node)
			{
				return E_FAIL;
			}

			c_external_pp_settings_container settings;
			settings.GetValue(*pp_node);

			// Get the users shader and texture directories
			if(!ReadUserPaths(settings))
			{
				return E_FAIL;
			}
			
			// Read the shaders and create shader classes for each shader
			if(!ReadShaders(settings.m_shaders.GetConst()))
			{
				return E_FAIL;
			}
			
			// Read the effects create effect classes for each effect
			if(!ReadEffects(settings.m_effects.GetConst()))
			{
				return E_FAIL;
			}

			// Read the effect instances create effect instance classes for each instance
			if(!ReadEffectInstances(settings.m_effect_instances.GetConst()))
			{
				return E_FAIL;
			}

			return S_OK;
		}

		/*!
		 * \brief
		 * Deletes all shaders, effects and effect instances.
		 * 
		 * Deletes all shaders, effects and effect instances.
		 */
		void c_system_external::UnloadExternal()
		{
			DeleteEffectInstances();
			DeleteEffects();
			DeleteShaders();
			ResetUserPaths();
		}

		HRESULT c_system_external::LoadShaders()
		{
			HRESULT success = S_OK;
			c_post_processing_main::s_post_processing_globals& globals = c_post_processing_main::Instance().Globals();

			// allocate direct3d resources for all shaders
			for(uint32 i = 0; i < m_members_external.m_shaders.count; i++)
			{
				success |= m_members_external.m_shaders.shader_sets[i].shader->LoadShader(globals.render_device);
				success |= m_members_external.m_shaders.shader_sets[i].shader->LoadBitmaps(globals.render_device);
			}
			return success;
		}

		void c_system_external::UnloadShaders()
		{
			// release all shader direct3d resources
			for(uint32 i = 0; i < m_members_external.m_shaders.count; i++)
			{
				m_members_external.m_shaders.shader_sets[i].shader->UnloadBitmaps();
				m_members_external.m_shaders.shader_sets[i].shader->UnloadShader();
			}
		}

		HRESULT c_system_external::LoadEffects()
		{
			HRESULT success = S_OK;

			uint32 i = 0;
			// set up data in the effects that require d3d resources to be allocated first
			for(i = 0; i < m_members_external.m_effects.count; i++)
				m_members_external.m_effects.effect_sets[i].effect->SetupEffectPostCreation();

			// effect instances should get quad instances here
			for(i = 0; i < m_members_external.m_effect_instances.count; i++)
				success &= m_members_external.m_effect_instances.effect_instance_sets[i].instance->LoadEffectInstance();
			return success;
		}

		void c_system_external::UnloadEffects()
		{
			// release all effect instance quad instances
			for(uint32 i = 0; i < m_members_external.m_effect_instances.count; i++)
				m_members_external.m_effect_instances.effect_instance_sets[i].instance->UnloadEffectInstance();
		}

		bool c_system_external::ReadUserPaths(const c_external_pp_settings_container& settings)
		{
			// Read the users paths
			auto& shader_directory = settings.m_shader_directory.GetConst();
			auto& texture_directory = settings.m_texture_directory.GetConst();

			if(shader_directory.empty() || texture_directory.empty())
			{
				return false;
			}

			// Copy the paths into memory
			ResetUserPaths();

			strcat_s(m_members_external.m_paths.shaders_path, sizeof(m_members_external.m_paths.shaders_path), shader_directory.c_str());
			strcat_s(m_members_external.m_paths.textures_path, sizeof(m_members_external.m_paths.textures_path), texture_directory.c_str());

			FileIO::AppendDirectorySlash(m_members_external.m_paths.shaders_path, sizeof(m_members_external.m_paths.shaders_path));
			FileIO::AppendDirectorySlash(m_members_external.m_paths.textures_path, sizeof(m_members_external.m_paths.textures_path));

			if(!FileIO::PathExists(m_members_external.m_paths.shaders_path) || !FileIO::PathExists(m_members_external.m_paths.textures_path))
			{
				blam::console_printf(false, "The external post processing shader or texture path does not exist:");
				blam::console_printf(false, "%s", m_members_external.m_paths.shaders_path);
				blam::console_printf(false, "%s", m_members_external.m_paths.textures_path);
				return false;
			}
			return true;
		}

		void c_system_external::ResetUserPaths()
		{
			m_members_external.m_paths.shaders_path[0] = '\0';
			m_members_external.m_paths.textures_path[0] = '\0';
		}

		bool c_system_external::ReadShaders(const std::vector<c_shader_container>& shaders)
		{
			m_members_external.m_shaders.count = shaders.size();
			if(shaders.size() == 0)
			{
				return false;
			}

			// Allocate memory for the shader definitions
			m_members_external.m_shaders.shader_sets = new s_shader_set[shaders.size()];

			// Read the shaders
			uint32 index = 0;
			for(auto& shader : shaders)
			{
				s_shader_set& set = m_members_external.m_shaders.shader_sets[index];

				// Allocate memory for the shader definition and class
				set.definition = new TagGroups::s_shader_postprocess_generic();
				set.shader = new c_shader_external();

				// Reset the definition and class to default values
				ZeroMemory(set.definition, sizeof(TagGroups::s_shader_postprocess_generic));
				set.shader->Ctor();
				set.shader->SetShaderDefinition(set.definition);
				
				auto& shader_id = shader.m_id.GetConst();
				auto& shader_path = shader.m_file.GetConst();

				if(shader_id.empty() || shader_path.empty())
				{
					blam::console_printf(false, "An external post processing shaders id or path are not set");
					return false;
				}

				// Build the absolute shader path
				char absolute_shader_path[MAX_PATH];
				absolute_shader_path[0] = '\0';
				strcpy_s(absolute_shader_path, sizeof(absolute_shader_path), m_members_external.m_paths.shaders_path);
				PathAppend(absolute_shader_path, shader_path.c_str());
				strcat_s(absolute_shader_path, sizeof(absolute_shader_path), ".fx");

				if(!FileIO::PathExists(absolute_shader_path))
				{
					blam::console_printf(false, "An external post processing shader path doesn't exist:");
					blam::console_printf(false, "%s", absolute_shader_path);
					return false;
				}

				// Create the shader definition
				BuildShaderDefinition(set.definition, absolute_shader_path);

				// Setup the shader class
				set.shader->SetShaderPath(absolute_shader_path);
				set.shader->SetShaderID(shader_id.c_str());
				set.shader->SetupShader();

				index++;
			}

			return true;
		}

		void c_system_external::DeleteShaders()
		{
			for(uint32 i =0; i < m_members_external.m_shaders.count; i++)
			{
				s_shader_set& set = m_members_external.m_shaders.shader_sets[i];

				// delete memory allocated in the shader definition
				DestroyShaderDefinition(set.definition);

				// delete the definition
				delete set.definition;
				set.definition = nullptr;

				// destruct the shader class and delete it
				set.shader->Dtor();
				delete set.shader;
				set.shader = nullptr;
			}

			// delete the shader set list
			delete [] m_members_external.m_shaders.shader_sets;
			m_members_external.m_shaders.shader_sets = nullptr;
			m_members_external.m_shaders.count = 0;
		}

		bool c_system_external::ReadEffects(const std::vector<c_effect_container>& effects)
		{
			m_members_external.m_effects.count = effects.size();
			if(effects.size() == 0)
			{
				return false;
			}

			// Allocate memory for the effect sets
			m_members_external.m_effects.effect_sets = new s_effect_set[m_members_external.m_effects.count];

			// Read the effects
			uint32 index = 0;
			for(auto& effect : effects)
			{
				s_effect_set& set = m_members_external.m_effects.effect_sets[index];

				set.definition = new s_effect_postprocess_external();
				set.effect = new c_effect_external();
				set.shader_instances = nullptr;
				set.shader_instance_count = 0;

				// Reset the definition and class to default values
				ZeroMemory(set.definition, sizeof(s_effect_postprocess_external));
				set.effect->Ctor();
				set.effect->SetEffectDefinition(set.definition);

				BuildEffectDefinition(set.definition, effect);

				// If the effect does not reference any shaders it is useless
				if(!set.definition->shader_ids.Count)
				{
					blam::console_printf(false, "An external post processing effect has no shader ID's");
					return false;
				}

				auto& effect_id = effect.m_id.GetConst();

				// If the effect has no id it cannot be referenced
				if(effect_id.empty())
				{
					blam::console_printf(false, "An external post processing effect has no ID");
					return false;
				}

				set.effect->SetEffectID(effect_id.c_str());

				set.shader_instances = new c_shader_instance_generic[set.definition->shader_ids.Count];
				set.shader_instance_count = set.definition->shader_ids.Count;

				int32 i = 0;
				//run constructor on shader instances
				for(i = 0; i < set.definition->shader_ids.Count; i++)
				{
					set.shader_instances[i].Ctor();
					set.effect->AddShaderInstance(&set.shader_instances[i]);
				}

				for(i = 0; i < set.definition->shader_ids.Count; i++)
				{
					c_shader_instance_generic& shader_instance = set.shader_instances[i];
					tag_string& id = set.definition->shader_ids[i];

					// find a shader set with a matching id
					s_shader_set* shader_set = nullptr;
					for(uint32 j = 0; (j < m_members_external.m_shaders.count) && !shader_set; j++)
						if(strcmp(m_members_external.m_shaders.shader_sets[j].shader->GetShaderID(), id) == 0)
							shader_set = &m_members_external.m_shaders.shader_sets[j];
					// if any shader is missing the effect is invalid
					if(!shader_set)
					{
						blam::console_printf(false, "An external post processing effect references a non-existing shader set: %s", id);
						return false;
					}

					shader_instance.SetShader(shader_set->shader);
					shader_instance.SetupShaderInstance();
				}

				set.effect->SetupEffect();

				index++;
			}

			return true;
		}

		void c_system_external::DeleteEffects()
		{
			for(uint32 i = 0; i < m_members_external.m_effects.count; i++)
			{
				s_effect_set& set = m_members_external.m_effects.effect_sets[i];

				// destroy the shader instances
				for(uint32 j = 0; j < set.shader_instance_count; j++)
					set.shader_instances[j].Dtor();

				// delete the shader instances
				delete [] set.shader_instances;
				set.shader_instances = nullptr;

				// delete memory allocated in the effect definition then delete it
				DestroyEffectDefinition(set.definition);
				delete set.definition;
				set.definition = nullptr;

				//destruct the effect class then delete it
				set.effect->Dtor();
				delete set.effect;
				set.effect = nullptr;
			}

			// delete the effect set list
			delete [] m_members_external.m_effects.effect_sets;
			m_members_external.m_effects.effect_sets = nullptr;
			m_members_external.m_effects.count = 0;
		}

		bool c_system_external::ReadEffectInstances(const std::vector<c_effect_instance_container>& effect_instances)
		{
			m_members_external.m_effect_instances.count = effect_instances.size();
			if(effect_instances.size() == 0)
			{
				return false;
			}

			// Allocate memory for the effect instance sets
			m_members_external.m_effect_instances.effect_instance_sets = new s_effect_instance_set[effect_instances.size()];

			// Read the effect instances
			uint32 index = 0;
			for(auto& effect_instance : effect_instances)
			{
				s_effect_instance_set& set = m_members_external.m_effect_instances.effect_instance_sets[index];

				set.definition = new TagGroups::s_effect_postprocess_generic_effect_instance();
				set.instance = new c_effect_instance_generic();

				// Clear the definition
				ZeroMemory(set.definition, sizeof(TagGroups::s_effect_postprocess_generic_effect_instance));
				set.instance->Ctor();

				bool effect_found = false;
				auto& effect_ref = effect_instance.m_effect.GetConst();
				for(uint32 i = 0; (i < m_members_external.m_effects.count) && !effect_ref.empty(); i++)
				{
					s_effect_set& effect_set = m_members_external.m_effects.effect_sets[i];

					if(effect_ref == effect_set.effect->GetEffectID())
					{
						set.instance->SetEffect(effect_set.effect);
						effect_found = true;
						break;
					}
				}

				if(!effect_ref.empty() && effect_found)
				{
					set.instance->SetEffectInstanceDefinition(set.definition);

					// Build the effect instance definition
					BuildEffectInstanceDefinition(set.definition, effect_instance);
				}

				set.instance->SetupEffectInstance();

				index++;
			}

			return true;
		}

		void c_system_external::DeleteEffectInstances()
		{
			for(uint32 i = 0; i < m_members_external.m_effect_instances.count; i++)
			{
				s_effect_instance_set& set = m_members_external.m_effect_instances.effect_instance_sets[i];

				// delete memory allocate in the effect instance definition then delete it
				DestroyEffectInstanceDefinition(set.definition);
				delete set.definition;
				set.definition = nullptr;

				// destroy the instance class then delete it
				set.instance->Dtor();
				delete set.instance;
				set.instance = nullptr;
			}

			// delete the effect instance sets
			delete [] m_members_external.m_effect_instances.effect_instance_sets;
			m_members_external.m_effect_instances.effect_instance_sets = nullptr;
			m_members_external.m_effect_instances.count = 0;
		}

		/////////////////////////////////////////////////
		// shader definition
		void c_system_external::BuildShaderDefinition(TagGroups::s_shader_postprocess_generic* definition, const char* shader_path)
		{
			// this is basically doing what the c_shader_postprocess does but without creating a D3D effect

			// get the relative include path
			char include_path[MAX_PATH];
			FileIO::GetDirectoryPath(include_path, sizeof(include_path), shader_path);

			c_shader_data_external shader_data;
			shader_data.Ctor();
			shader_data.SetFilePath(shader_path, include_path);

			c_shader_include_manager include_manager(include_path);

			LPD3DXEFFECTCOMPILER compiler = nullptr;
			LPD3DXBUFFER error_buffer = nullptr;
			do
			{
				// read the shader into memory
				uint32 data_size = 0;
				const void* data_pointer = shader_data.GetData(data_size);

				if(!data_pointer || (data_size == 0))
					break;

				// build the shader compiler
				HRESULT success = D3DXCreateEffectCompiler(CAST_PTR(const char*, data_pointer),
					data_size,
					shader_data.GetMacros(),
					&include_manager,
					D3DXSHADER_OPTIMIZATION_LEVEL3,
					&compiler,
					&error_buffer);

				if(FAILED(success))
				{
					ErrorReporting::Write("Failed to load shader:");
					ErrorReporting::Write(shader_data.DataSourceID());
					if(error_buffer)
						ErrorReporting::WriteD3DXErrors(error_buffer);
					break;
				}

				D3DXEFFECT_DESC effect_desc;
				success = compiler->GetDesc(&effect_desc);

				if(FAILED(success)) break;

				// set up techniques
				uint32 technique_count = (effect_desc.Techniques > 3 ? 3 : effect_desc.Techniques);
				definition->techniques.Address = new TagGroups::s_technique_definition[technique_count];
				definition->techniques.Count = technique_count;

				for(int32 i = 0; i < definition->techniques.Count; i++)
				{
					D3DXHANDLE handle = compiler->GetTechnique(i);
					if(handle == nullptr)
						continue;

					BuildTechnique(compiler, handle, definition->techniques[i]);
				}

				BuildParameters(compiler, definition);

			}while(false);

			safe_release(error_buffer);
			safe_release(compiler);

			shader_data.Dispose();
			shader_data.Dtor();
		}

		void c_system_external::BuildTechnique(LPD3DXEFFECTCOMPILER compiler, D3DXHANDLE handle, TagGroups::s_technique_definition& technique)
		{
			D3DXTECHNIQUE_DESC technique_desc;
			HRESULT success = compiler->GetTechniqueDesc(handle, &technique_desc);
			
			YELO_ASSERT_DISPLAY(SUCCEEDED(success), "failed to get a techniques description");

			// reset to defaults
			ZeroMemory(&technique, sizeof(technique));

			// set the blocks fields
			strncpy(technique.name, technique_desc.Name, sizeof(technique.name));

			INT shader_model_mask;
			GetAnnotationValue(compiler, handle, "shader_model_mask", 
				Yelo::Enums::_shader_model_mask_1_0 | Yelo::Enums::_shader_model_mask_2_0 | Yelo::Enums::_shader_model_mask_3_0, shader_model_mask);
			
			technique.shader_model.sm_1_0_bit = (shader_model_mask & Yelo::Enums::_shader_model_mask_1_0) ==  Yelo::Enums::_shader_model_mask_1_0;
			technique.shader_model.sm_2_0_bit = (shader_model_mask & Yelo::Enums::_shader_model_mask_2_0) ==  Yelo::Enums::_shader_model_mask_2_0;
			technique.shader_model.sm_3_0_bit = (shader_model_mask & Yelo::Enums::_shader_model_mask_3_0) ==  Yelo::Enums::_shader_model_mask_3_0;

			if(technique_desc.Passes == 0)
				return;

			// add passes to the technique block
			technique.passes.Address = new TagGroups::s_pass_definition[technique_desc.Passes];
			technique.passes.Count = technique_desc.Passes;

			for(int32 i = 0; i < technique.passes.Count; i++)
			{
				D3DXHANDLE pass = compiler->GetPass(handle, i);
				if(pass == nullptr)
					continue;

				BuildPass(compiler, pass, technique.passes[i]);
			}
		}

		void c_system_external::BuildPass(LPD3DXEFFECTCOMPILER compiler, D3DXHANDLE handle, TagGroups::s_pass_definition& pass)
		{
			D3DXPASS_DESC pass_desc;
			HRESULT success = compiler->GetPassDesc(handle, &pass_desc);
			
			YELO_ASSERT_DISPLAY(SUCCEEDED(success), "failed to get a pass' description");

			// reset to zeros
			ZeroMemory(&pass, sizeof(pass));

			// set the blocks fields
			strncpy(pass.name, pass_desc.Name, Yelo::Enums::k_tag_string_length);

			// should the target be cleared prior to this pass
			BOOL clear_target;
			GetAnnotationValue(compiler, handle, "clear_target", false, clear_target);
			pass.flags.clear_target_bit = clear_target;

			// should the scene be copied to the target on this pass
			BOOL copy_scene_to_target;
			GetAnnotationValue(compiler, handle, "copy_scene_to_target", false, copy_scene_to_target);
			pass.flags.copy_scene_to_target_bit = copy_scene_to_target;

			// which chain should this pass render to
			INT render_chain;
			GetAnnotationValue(compiler, handle, "render_chain", 0, render_chain);
			pass.render_chain = (Yelo::_enum)render_chain;

			// should the buffer be clear on this pass
			BOOL clear_buffer_texture;
			GetAnnotationValue(compiler, handle, "clear_buffer_texture", false, clear_buffer_texture);
			pass.flags.clear_buffer_texture_bit = clear_buffer_texture;
		}

		void c_system_external::BuildParameters(LPD3DXEFFECTCOMPILER compiler, TagGroups::s_shader_postprocess_generic* definition)
		{
			uint32 parameter_count = 0;
			s_parameter_handle* parameter_list = nullptr;
			//textures
			parameter_count += GetParameterHandles(compiler, parameter_list, "VARTEXTURE_%i", 4, Enums::_shader_variable_base_type_texture, 1);
			//booleans
			parameter_count += GetParameterHandles(compiler, parameter_list, "VARBOOL_%i", 16, Enums::_shader_variable_base_type_boolean, 1);
			//integers
			parameter_count += GetParameterHandles(compiler, parameter_list, "VARINT_%i", 16, Enums::_shader_variable_base_type_integer, 1);
			//floats
			parameter_count += GetParameterHandles(compiler, parameter_list, "VARFLOAT_%i", 16, Enums::_shader_variable_base_type_float, 1);
			//float2s
			parameter_count += GetParameterHandles(compiler, parameter_list, "VARFLOAT2_%i", 16, Enums::_shader_variable_base_type_float, 2);
			//float3s
			parameter_count += GetParameterHandles(compiler, parameter_list, "VARFLOAT3_%i", 16, Enums::_shader_variable_base_type_float, 3);
			//float4s
			parameter_count += GetParameterHandles(compiler, parameter_list, "VARFLOAT4_%i", 16, Enums::_shader_variable_base_type_float, 4);
			//colors
			parameter_count += GetParameterHandles(compiler, parameter_list, "VARCOLOR_%i", 16, Enums::_shader_variable_base_type_argb_color, 1);

			if(parameter_count == 0)
				return;

			definition->parameters.Address = new TagGroups::s_shader_postprocess_parameter[parameter_count];
			definition->parameters.Count = parameter_count;

			s_parameter_handle* current = parameter_list;
			for(int i = 0; (i < definition->parameters.Count) && current; i++)
			{
				SetupParameter(compiler, *current, definition->parameters[i]);
				current = current->GetNext();
			}

			// delete the parameter handles
			DeleteLinkedList(parameter_list);
		}

		void c_system_external::DestroyShaderDefinition(TagGroups::s_shader_postprocess_generic* definition)
		{
			for(int32 i = 0; i < definition->parameters.Count; i++)
				DestroyParameter(definition->parameters[i]);
			delete [] definition->parameters.Address;
			definition->parameters.Address = nullptr;
			definition->parameters.Count = 0;

			// delete allocated block memory
			for(int32 i = 0; i < definition->techniques.Count; i++)
				DestroyTechnique(definition->techniques[i]);
			delete [] definition->techniques.Address;
			definition->techniques.Address = nullptr;
			definition->techniques.Count = 0;

			ZeroMemory(definition, sizeof(TagGroups::s_shader_postprocess_generic));
		}

		void c_system_external::DestroyTechnique(TagGroups::s_technique_definition& technique)
		{
			delete [] technique.passes.Address;
			technique.passes.Address = nullptr;
			technique.passes.Count = 0;

			ZeroMemory(&technique, sizeof(TagGroups::s_technique_definition));
		}

		void c_system_external::DestroyParameter(TagGroups::s_shader_postprocess_parameter& parameter)
		{
			delete [] parameter.bitmap_value.runtime.external.source;
			parameter.bitmap_value.runtime.external.source = nullptr;

			ZeroMemory(&parameter.bitmap_value, sizeof(parameter.bitmap_value));
			ZeroMemory(&parameter.animation_function, sizeof(parameter.animation_function));
			ZeroMemory(&parameter.runtime_value, sizeof(parameter.runtime_value));
			ZeroMemory(&parameter.value, sizeof(parameter.value));
		}

		/////////////////////////////////////////////////
		// effect definition
		void c_system_external::BuildEffectDefinition(s_effect_postprocess_external* definition, const c_effect_container& effect)
		{
			auto& shader_ids = effect.m_shader_ids.GetConst();

			if(shader_ids.size() == 0)
			{
				return;
			}
			
			definition->shader_ids.Address = new tag_string[shader_ids.size()];
			definition->shader_ids.Count = shader_ids.size();

			uint32 index = 0;
			for(auto& id : shader_ids)
			{
				tag_string& shader_id = definition->shader_ids[index];

				ZeroMemory(shader_id, sizeof(shader_id));

				auto& id_string = id.GetConst();
				if(!id_string.empty())
				{
					strncpy_s(shader_id, sizeof(shader_id), id_string.c_str(), sizeof(shader_id));
				}

				index++;
			}
		}

		void c_system_external::DestroyEffectDefinition(s_effect_postprocess_external* definition)
		{
			delete [] definition->shader_ids.Address;
			definition->shader_ids.Address = nullptr;
			definition->shader_ids.Count = 0;
		}

		/////////////////////////////////////////////////
		// effect instance definition
		void c_system_external::BuildEffectInstanceDefinition(TagGroups::s_effect_postprocess_generic_effect_instance* definition
			, const c_effect_instance_container& effect_instance)
		{
			auto& name = effect_instance.m_name.GetConst();
			if(!name.empty())
			{
				strncpy_s(definition->name, sizeof(definition->name), name.c_str(), sizeof(definition->name));
			}

			int render_stage = effect_instance.m_render_stage.GetConst();
			if(render_stage >= Enums::_postprocess_render_stage)
			{
				render_stage = Enums::_postprocess_render_stage_pre_hud;
			}
			definition->render_stage = render_stage;

			int activation_operation = effect_instance.m_activation_operation.GetConst();
			if(activation_operation > 1)
			{
				activation_operation = 0;
			}
			definition->activation_operation = activation_operation;
			
			// Set quad values
			auto& quad = effect_instance.m_quad;

			definition->quad_definition.tessellation.x = quad.m_tessellation.GetConst().x;
			definition->quad_definition.tessellation.y = quad.m_tessellation.GetConst().y;
			definition->quad_definition.x_bounds.lower = quad.m_bounds_x.GetConst().lower;
			definition->quad_definition.x_bounds.upper = quad.m_bounds_x.GetConst().upper;
			definition->quad_definition.y_bounds.lower = quad.m_bounds_y.GetConst().lower;
			definition->quad_definition.y_bounds.upper = quad.m_bounds_y.GetConst().upper;

			int activation_control_count = effect_instance.m_activation_controls.size();
			if(activation_control_count != 0)
			{
				definition->activation_controls.Address = new TagGroups::s_effect_postprocess_effect_activation_control[activation_control_count];
				definition->activation_controls.Count = activation_control_count;

				int32 index = 0;
				for(auto& activation_control : effect_instance.m_activation_controls.GetConst())
				{
					TagGroups::s_effect_postprocess_effect_activation_control& control = definition->activation_controls[index];

					int state = activation_control.m_activation_state.GetConst();
					if(state >= Enums::_effect_activation_control_state)
					{
						state = 0;
					}
					control.state = state;

					control.flags.invert_bit = activation_control.m_invert.GetConst();

					index++;
				}
			}
		}

		void c_system_external::DestroyEffectInstanceDefinition(TagGroups::s_effect_postprocess_generic_effect_instance* definition)
		{
			delete [] definition->activation_controls.Address;
			definition->activation_controls.Address = nullptr;
			definition->activation_controls.Count = 0;
		}

		/////////////////////////////////////////////////
		// parameters
		uint32 c_system_external::GetParameterHandles(LPD3DXEFFECTCOMPILER compiler,
			s_parameter_handle*& list,
			const char* semantic_format,
			const uint32 count,
			const int16 parameter_type,
			const int16 parameter_type_count)
		{
			tag_string semantic;
			semantic[0] = '\0';

			uint32 parameter_count = 0;
			for(uint32 i = 0; i < count; i++)
			{
				sprintf_s(semantic, sizeof(semantic), semantic_format, i + 1);
				D3DXHANDLE handle = compiler->GetParameterBySemantic(nullptr, semantic);
				if(!handle) continue;

				s_parameter_handle* parameter = new s_parameter_handle();
				parameter->SetNext(nullptr);
				parameter->SetPrevious(nullptr);
				parameter->handle = handle;
				parameter->type.type = parameter_type;
				parameter->type.count = parameter_type_count;

				AppendLinkedListNode(list, parameter);
				parameter_count++;
			}
			return parameter_count;
		}

		void c_system_external::SetupParameter(LPD3DXEFFECTCOMPILER compiler,
			const s_parameter_handle& handle,
			TagGroups::s_shader_postprocess_parameter& parameter)
		{
			// clear the parameter name
			parameter.value_name[0] = '\0';

			// copy the parameter name from the parameter description
			D3DXPARAMETER_DESC parameter_desc;
			HRESULT success = compiler->GetParameterDesc(handle.handle, &parameter_desc);
			if(SUCCEEDED(success))
				strncpy_s(parameter.value_name, sizeof(parameter.value_name), parameter_desc.Name, sizeof(parameter.value_name));

			// set the value type
			parameter.value_type = handle.type;

			SetupParameterValue(compiler, handle, parameter);
			SetupParameterRuntimeOverride(compiler, handle, parameter);
			SetupParameterAnimation(compiler, handle, parameter);
			SetupParameterBitmap(compiler, handle, parameter);
		}

		void c_system_external::SetupParameterValue(LPD3DXEFFECTCOMPILER compiler,
			const s_parameter_handle& handle,
			TagGroups::s_shader_postprocess_parameter& parameter)
		{
			union {
				BOOL boolean;
				INT integer;
				real_quaternion quaternion;
			};
			// zero out the quaternion so that random values are not copied into the parameter values
			ZeroMemory(&quaternion, sizeof(quaternion));
			// reset the parameter values to zero, vector4d is the largest value type so will null all others
			ZeroMemory(&parameter.value.vector4d, sizeof(parameter.value.vector4d));

			D3DXHANDLE value_upper_bound, value_lower_bound;
			value_upper_bound =			compiler->GetAnnotationByName(handle.handle, "value_upper_bound");
			value_lower_bound =			compiler->GetAnnotationByName(handle.handle, "value_lower_bound");
			
			HRESULT success;
			// read the upper and lower values from the compiler
			switch(handle.type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				// boolean only uses the upper bound annotation
				success = compiler->GetBool(value_upper_bound, &boolean);
				if(SUCCEEDED(success))
				{
					parameter.value.boolean.enabled = (boolean != FALSE);
					parameter.value.boolean.inverse = !parameter.value.boolean.enabled;
				}
				break;
			case Enums::_shader_variable_base_type_integer:
				// read the upper and lower bounds
				success = compiler->GetInt(value_upper_bound, &integer);
				if(SUCCEEDED(success)) parameter.value.integer32.upper_bound = integer;

				success = compiler->GetInt(value_lower_bound, &integer);
				if(SUCCEEDED(success)) parameter.value.integer32.lower_bound = integer;
				break;
			case Enums::_shader_variable_base_type_argb_color:
			case Enums::_shader_variable_base_type_float:
				{
					// if the type is a color, read 4 floats
					uint32 count = (handle.type.type == Enums::_shader_variable_base_type_argb_color ? 4 : handle.type.count);

					// having to memcpy from the quaternion as the upper and lower bounds to not match up
					success = compiler->GetFloatArray(value_upper_bound, CAST_PTR(FLOAT*, &quaternion), count);
					if(SUCCEEDED(success))
					{
						if(handle.type.type == Enums::_shader_variable_base_type_argb_color)
						{
							parameter.value.color4d.upper_bound.alpha = quaternion.j;
							parameter.value.color4d.upper_bound.red = quaternion.k;
							parameter.value.color4d.upper_bound.green = quaternion.w;
							parameter.value.color4d.upper_bound.blue = quaternion.i;
						}
						else
							parameter.value.vector4d.upper_bound = quaternion;
					}

					success = compiler->GetFloatArray(value_lower_bound, CAST_PTR(FLOAT*, &quaternion), count);
					if(SUCCEEDED(success))
					{
						if(handle.type.type == Enums::_shader_variable_base_type_argb_color)
						{
							parameter.value.color4d.lower_bound.alpha = quaternion.j;
							parameter.value.color4d.lower_bound.red = quaternion.k;
							parameter.value.color4d.lower_bound.green = quaternion.w;
							parameter.value.color4d.lower_bound.blue = quaternion.i;
						}
						else
							parameter.value.vector4d.lower_bound = quaternion;
					}
				};
				break;
			}
		}

		void c_system_external::SetupParameterRuntimeOverride(LPD3DXEFFECTCOMPILER compiler,
			const s_parameter_handle& handle,
			TagGroups::s_shader_postprocess_parameter& parameter)
		{
			// rest to defaults
			parameter.runtime_value.value = 0;
			parameter.runtime_value.flags.invert_bit = false;

			// get the runtime value annotation handles
			D3DXHANDLE runtime_value_override, runtime_value_invert;
			runtime_value_override =	compiler->GetAnnotationByName(handle.handle, "runtime_value_override");
			runtime_value_invert =		compiler->GetAnnotationByName(handle.handle, "runtime_value_invert");

			// ge the runtime override enum value
			INT value = 0;
			HRESULT success = compiler->GetInt(runtime_value_override, &value);
			if(FAILED(success)) return;

			// determine if the value is valid for the variable type
			bool valid = false;
			switch(handle.type.type)
			{
			case Enums::_shader_variable_base_type_boolean: valid = (value < Enums::_parameter_runtime_value_bool); break;
			case Enums::_shader_variable_base_type_integer: valid = (value < Enums::_parameter_runtime_value_int); break;
			case Enums::_shader_variable_base_type_argb_color: valid = (value < Enums::_parameter_runtime_value_color); break;
			case Enums::_shader_variable_base_type_float:
				{
					switch(handle.type.count)
					{
					case 1: valid = (value < Enums::_parameter_runtime_value_float); break;
					case 2: valid = (value < Enums::_parameter_runtime_value_float2); break;
					case 3: valid = (value < Enums::_parameter_runtime_value_float3); break;
					case 4: valid = (value < Enums::_parameter_runtime_value_float4); break;
					};
				}
				break;
			}
			if(!valid) return;

			// clamp to zero if negative
			parameter.runtime_value.value = (value < 0 ? 0 : value);

			if(!parameter.runtime_value.value)
				return;

			BOOL invert;
			success = compiler->GetBool(runtime_value_invert, &invert);
			if(SUCCEEDED(success))
				parameter.runtime_value.flags.invert_bit = (invert != FALSE);
		}

		void c_system_external::SetupParameterAnimation(LPD3DXEFFECTCOMPILER compiler,
			const s_parameter_handle& handle,
			TagGroups::s_shader_postprocess_parameter& parameter)
		{
			// reset to defaults
			parameter.animation_function.function = 0;
			parameter.animation_function.flags.inverted_bit = false;
			parameter.animation_function.flags.multichannel_noise_bit = false;
			parameter.animation_function.flags.ignore_alpha_bit = false;
			parameter.animation_function.animation_duration = 0.0f;
			parameter.animation_function.animation_rate = 0.0f;

			// get handles to the animation annotations
			D3DXHANDLE animation_function, animation_flags, animation_duration, animation_rate;
			animation_function =		compiler->GetAnnotationByName(handle.handle, "animation_function");
			animation_flags =			compiler->GetAnnotationByName(handle.handle, "animation_flags");
			animation_duration =		compiler->GetAnnotationByName(handle.handle, "animation_duration");
			animation_rate =			compiler->GetAnnotationByName(handle.handle, "animation_rate");

			HRESULT success;

			INT integer32;
			FLOAT real32;
			// read the animation values from the compiler
			success = compiler->GetInt(animation_function, &integer32);
			if(SUCCEEDED(success) && integer32)
			{
				if((integer32 >= 0) && (integer32 < Enums::k_number_of_periodic_functions))
					parameter.animation_function.function = integer32;
			}
			if(!parameter.animation_function.function) return;

			// get the animation flags
			success = compiler->GetInt(animation_function, &integer32);
			if(SUCCEEDED(success))
			{
				union{
					TagGroups::s_shader_postprocess_value_animation_function::_flags flags;
					byte value;
				};
				// set up the bitmask
				flags.inverted_bit = true;
				flags.multichannel_noise_bit = true;
				flags.ignore_alpha_bit = true;

				// cast the int to a byte and mask bits that are unused
				value = CAST(byte, integer32) & value;

				// set the flags value
				parameter.animation_function.flags = flags;
			}

			// get the animation duration
			success = compiler->GetFloat(animation_duration, &real32);
			if(SUCCEEDED(success)) { parameter.animation_function.animation_duration = real32; }

			// get the animation rate
			success = compiler->GetFloat(animation_rate, &real32);
			if(SUCCEEDED(success)) { parameter.animation_function.animation_rate = real32; }
		}

		void c_system_external::SetupParameterBitmap(LPD3DXEFFECTCOMPILER compiler,
			const s_parameter_handle& handle,
			TagGroups::s_shader_postprocess_parameter& parameter)
		{
			ZeroMemory(&parameter.bitmap_value, sizeof(parameter.bitmap_value));

			D3DXHANDLE texture_location;
			texture_location =			compiler->GetAnnotationByName(handle.handle, "texture_location");

			// get the texture string
			YELO_ASSERT_DISPLAY(parameter.bitmap_value.runtime.external.source == nullptr, "allocating string memory before the previous value is deleted");
			const char* location;
			HRESULT success = compiler->GetString(texture_location, &location);
			if(SUCCEEDED(success) && location)
			{
				// allocate memory for the string
				int32 length = strlen(m_members_external.m_paths.textures_path) + strlen(location) + 1;
				parameter.bitmap_value.runtime.external.source = new char[length];
				parameter.bitmap_value.runtime.external.source[0] = '\0';

				// copy the textures path into the runtime source strng
				strcpy_s(parameter.bitmap_value.runtime.external.source, length, m_members_external.m_paths.textures_path);
				PathAppend(parameter.bitmap_value.runtime.external.source, location);

				parameter.bitmap_value.flags.is_external_bit = true;
			}
		}

		void c_system_external::GetAnnotationValue(LPD3DXEFFECTCOMPILER compiler, D3DXHANDLE variable, const char* annotation, INT default_value, INT& output)
		{
			// should the buffer be clear on this pass
			D3DXHANDLE annotation_handle = compiler->GetAnnotationByName(variable, annotation);

			if(annotation_handle == nullptr)
				output = default_value;
			else
				compiler->GetInt(annotation_handle, &output);
		}

		void c_system_external::SetupRenderSets()
		{
			// add effect instances to the render sets
			for(int i = 0; i < NUMBEROF(m_members_external.m_render_sets); i++)
			{
				m_members_external.m_render_sets[i].Ctor();
				SetRenderSet(m_members_external.m_render_sets[i], (Enums::postprocess_render_stage)i);
			}
		}

		void c_system_external::ClearRenderSets()
		{
			// reset the render sets to empty
			for(int i = 0; i < NUMBEROF(m_members_external.m_render_sets); i++)
				m_members_external.m_render_sets[i].Dtor();
		}
		
		void c_system_external::SetRenderSet(c_effect_render_set& set, Enums::postprocess_render_stage render_stage)
		{
			// get the number of effects for this render stage
			uint32 count = 0;
			for(uint32 i = 0; i < m_members_external.m_effect_instances.count; i++)
				if(m_members_external.m_effect_instances.effect_instance_sets[i].instance->GetRenderStage() == render_stage)
					count++;

			// set the number of effects in the render set
			set.SetCount(count);

			if(count == 0)
				return;

			// add each effect instance for this render stage to the render set
			count = 0;
			for(uint32 i = 0; i < m_members_external.m_effect_instances.count; i++)
				if(m_members_external.m_effect_instances.effect_instance_sets[i].instance->GetRenderStage() == render_stage)
				{
					set.SetEffectInstance(count, m_members_external.m_effect_instances.effect_instance_sets[i].instance);
					count++;
				}
		}

		void c_system_external::ValidateSystem()
		{
			uint32 i = 0;
			for(i = 0; i < m_members_external.m_shaders.count; i++)
				m_members_external.m_shaders.shader_sets[i].shader->Validate();
			for(i = 0; i < m_members_external.m_effects.count; i++)
				m_members_external.m_effects.effect_sets[i].effect->Validate();
			for(i = 0; i < m_members_external.m_effect_instances.count; i++)
				m_members_external.m_effect_instances.effect_instance_sets[i].instance->Validate();
		}

		/////////////////////////////////////////////////
		// system setup
		void c_system_external::UpdateStatus()
		{
			m_members.m_flags.is_ready = false;
			do
			{
				if(m_members.status != Enums::pp_component_status_initialised) break;

				m_members.m_flags.is_ready = true;
			}while(false);
		}
	};};};};
};
#endif