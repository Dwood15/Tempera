/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <sys/stat.h>

#include <engine_interface.h>
#include <d3d9types.h>
#include <d3d9caps.h>
#include <d3d9.h>
#include <enums/shaders.h>

namespace Yelo::Rasterizer::ShaderExtension {

#include "../../../memory/datum_index.h"
#include "../dx9/DX9.hpp"
#include "../../../memory/files.hpp"
#include "../../../cseries/MacrosCpp.h"
#include "../../../memory/memory_interface_base.hpp"
#include "../../../CurrentEngine.h"
#include "../../../bitmaps/bitmap_group.h"
#include "../../../memory/packed_file.hpp"



	typedef char shader_usage_id_t[128];

	typedef void (__cdecl *shadersetup_func)(void *, void *, void *, void *, void *, void *, void *);

	static const char *g_ps_collection_format_a = "shaders\\EffectCollection_ps_%d_a.enc";
	static const char *g_ps_collection_format_b = "shaders\\EffectCollection_ps_%d_b.enc";
	static const char *g_vs_collection_path     = "shaders\\vsh_OS.enc";

	static const char *g_shader_collection_ids[] =
								{
									"SE_EffectCollection_PS2a",
									"SE_EffectCollection_PS2b",
									"SE_EffectCollection_VS"
								};

	static bool       g_shader_files_present             = false;
	static Yelo::Enums::ps_support g_ps_support                       = Yelo::Enums::ps_support::_ps_support_legacy;
	static bool       g_extensions_enabled               = true;
	static bool       g_extensions_enabled_user_override = true;
	static DWORD      g_ps_version_iterator_start        = 0xFFFF0101;

	bool ExtensionsEnabled() {
		return g_extensions_enabled;
	}

	void SetTexture(IDirect3DDevice9 *pDevice, unsigned short sampler, datum_index bitmap_tag_index);

	// #include "Rasterizer/ShaderExtension/ShaderExtension_Model.inl"
	// #include "Rasterizer/ShaderExtension/ShaderExtension_Environment.inl"
	// #include "Rasterizer/ShaderExtension/ShaderExtension_Effect.inl"

	__declspec(naked) static void Hook_RenderObject_ForceInvertBackfaceNormals() {
		static const uintptr_t RETN_ADDRESS = 0x52DFCB; //RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN;

		__asm{
		mov     al, 1
		test    al, al

		jmp      RETN_ADDRESS
		};
	}

	void SetTexture(IDirect3DDevice9 *pDevice, ushort sampler, datum_index bitmap_tag_index) {
		// get the bitmap datum pointer
		auto group  = TagGroups::TagGetUnsafe<TagGroups::s_bitmap_group>(bitmap_tag_index);
		auto bitmap = CAST_PTR(TagGroups::s_bitmap_data *, &group->bitmaps[0]);

		// set the texture to the device

		{
			auto bitmap_data = get_bitmap_data(bitmap_tag_index, lightmap_index);
			auto stage = sampler;
			auto got = CurrentEngine.getFunctionBegin("rasterizer_set_texture_bitmap_data");

			if (got) {
				__asm mov esi, bitmap_data
				::calls::DoCall<Convention::m_cdecl, bool, short>(*got, stage);
			}

			//blam::rasterizer_set_texture_bitmap_data(sampler, bitmap);

			pDevice->SetSamplerState(sampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			pDevice->SetSamplerState(sampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

			pDevice->SetSamplerState(sampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			pDevice->SetSamplerState(sampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pDevice->SetSamplerState(sampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}

		int BuildEffectCollectionPath(char *string_out, const char *format_string, const int major, const int minor) {
			const char *format = format_string;

			if ((major >= 2) && (minor >= 0) && (g_ps_support > _ps_support_2_0)) {
				switch (g_ps_support) {
					case _ps_support_3_0:
					case _ps_support_2_a:
						format = g_ps_collection_format_a;
						break;
					case _ps_support_2_b:
						format = g_ps_collection_format_b;
						break;
					default:
						break;
				}
			}
			return sprintf(string_out, format, major, minor);
		}

		class c_shader_extension_container final : public c_configuration_container {
			class c_shader_model_settings final : public c_configuration_container {
			public:
				c_configuration_value<bool> m_normal_maps;
				c_configuration_value<bool> m_detail_normal_maps;
				c_configuration_value<bool> m_specular_maps;
				c_configuration_value<bool> m_specular_lighting;

				c_shader_model_settings() : c_configuration_container("Object"), m_normal_maps("NormalMaps", true),
													 m_detail_normal_maps("DetailNormalMaps", true), m_specular_maps("SpecularMaps", true),
													 m_specular_lighting("SpecularLighting", true) {}

			protected:
				const std::vector<i_configuration_value *const> GetMembers() override {
					return std::vector<i_configuration_value *const>{
						&m_normal_maps,
						&m_detail_normal_maps,
						&m_specular_maps,
						&m_specular_lighting
					};
				}
			};

			class c_shader_environment_settings final : public c_configuration_container {
			public:
				c_configuration_value<bool> m_diffuse_directional_lightmaps;
				c_configuration_value<bool> m_specular_directional_lightmaps;

				c_shader_environment_settings() : c_configuration_container("Environment"),
															 m_diffuse_directional_lightmaps("DiffuseDirectionalLightmaps", true),
															 m_specular_directional_lightmaps("SpecularDirectionalLightmaps", true) {}

			protected:
				const std::vector<i_configuration_value *const> GetMembers() override {
					return std::vector<i_configuration_value *const>{
						&m_diffuse_directional_lightmaps,
						&m_specular_directional_lightmaps
					};
				}
			};

			class c_shader_effect_settings final : public c_configuration_container {
			public:
				c_configuration_value<bool> m_depth_fade;

				c_shader_effect_settings() : c_configuration_container("Effect"), m_depth_fade("DepthFade", true) {}

			protected:
				const std::vector<i_configuration_value *const> GetMembers() override {
					return std::vector<i_configuration_value *const>{
						&m_depth_fade
					};
				}
			};

		public:
			c_configuration_value<bool>   m_enabled;
			c_shader_model_settings       m_shader_model;
			c_shader_environment_settings m_shader_environment;
			c_shader_effect_settings      m_shader_effect;

			c_shader_extension_container()
				: c_configuration_container("Rasterizer.ShaderExtensions"), m_enabled("Enabled", true), m_shader_model(),
				  m_shader_environment(), m_shader_effect() {}

		protected:
			const std::vector<i_configuration_value *const> GetMembers() override {
				return std::vector<i_configuration_value *const>
					{
						&m_enabled,
						&m_shader_model,
						&m_shader_environment,
						&m_shader_effect,
					};
			}
		};

		class c_settings_shaderextension final
			: public Settings::c_settings_singleton<c_shader_extension_container, c_settings_shaderextension> {

			void SetModelUsage() {
				Model::g_extension_usage_mask = Flags::_shader_extension_usage_normal_map | Flags::_shader_extension_usage_detail_normal |
														  Flags::_shader_extension_usage_specular_map | Flags::_shader_extension_usage_specular_lighting;

				int32 usage_mask = Flags::_shader_extension_usage_none;

				auto &settings_instance = Get();
				usage_mask |= (settings_instance.m_shader_model.m_normal_maps ? Flags::_shader_extension_usage_normal_map
																								  : Flags::_shader_extension_usage_none);
				usage_mask |= (settings_instance.m_shader_model.m_detail_normal_maps ? Flags::_shader_extension_usage_detail_normal
																											: Flags::_shader_extension_usage_none);
				usage_mask |= (settings_instance.m_shader_model.m_specular_maps ? Flags::_shader_extension_usage_specular_map
																									 : Flags::_shader_extension_usage_none);
				usage_mask |= (settings_instance.m_shader_model.m_specular_lighting ? Flags::_shader_extension_usage_specular_lighting
																										  : Flags::_shader_extension_usage_none);

				Model::g_extension_usage_mask &= usage_mask;
			}

			void GetModelUsage() {
				auto &settings_instance = Get();
				settings_instance.m_shader_model.m_normal_maps        = TEST_FLAG(Model::g_extension_usage_mask,
																										Flags::_shader_extension_usage_bit_normal_map);
				settings_instance.m_shader_model.m_detail_normal_maps = TEST_FLAG(Model::g_extension_usage_mask,
																										Flags::_shader_extension_usage_bit_detail_normal);
				settings_instance.m_shader_model.m_specular_maps      = TEST_FLAG(Model::g_extension_usage_mask,
																										Flags::_shader_extension_usage_bit_specular_map);
				settings_instance.m_shader_model.m_specular_lighting  = TEST_FLAG(Model::g_extension_usage_mask,
																										Flags::_shader_extension_usage_bit_specular_lighting);
			}

			void SetEnvironmentUsage() {
				Environment::g_extension_usage_mask =
					Flags::_shader_extension_usage_directional_lightmaps_diff | Flags::_shader_extension_usage_directional_lightmaps_spec;

				int32 usage_mask = Flags::_shader_extension_usage_none;

				usage_mask |= (Get().m_shader_environment.m_diffuse_directional_lightmaps
									? Flags::_shader_extension_usage_directional_lightmaps_diff : Flags::_shader_extension_usage_none);
				usage_mask |= (Get().m_shader_environment.m_specular_directional_lightmaps
									? Flags::_shader_extension_usage_directional_lightmaps_spec : Flags::_shader_extension_usage_none);

				Environment::g_extension_usage_mask &= usage_mask;
			}

			void GetEnvironmentUsage() {
				Get().m_shader_environment.m_diffuse_directional_lightmaps  = TEST_FLAG(Environment::g_extension_usage_mask,
																												Flags::_shader_extension_usage_bit_directional_lightmaps_diff);
				Get().m_shader_environment.m_specular_directional_lightmaps = TEST_FLAG(Environment::g_extension_usage_mask,
																												Flags::_shader_extension_usage_bit_directional_lightmaps_spec);
			}

			void SetEffectUsage() {
				Effect::g_extension_usage_mask = Flags::_shader_extension_usage_depth_fade;

				int usage_mask = Flags::_shader_extension_usage_none;

				usage_mask |= (Get().m_shader_effect.m_depth_fade ? Yelo::Flags::_shader_extension_usage_depth_fade
																				  : Flags::_shader_extension_usage_none);

				Effect::g_extension_usage_mask &= usage_mask;
			}

			void GetEffectUsage() {
				Get().m_shader_effect.m_depth_fade = TEST_FLAG(Effect::g_extension_usage_mask, Flags::_shader_extension_usage_bit_depth_fade);
			}

		public:
			void PostLoad() override {
				SetModelUsage();
				SetEnvironmentUsage();
				SetEffectUsage();
			}

			void PreSave() override {
				GetModelUsage();
				GetEnvironmentUsage();
				GetEffectUsage();
			}
		};

		void ApplyHooks() {
			int32 i = 0;
			// replace the vertex shader collection paths
			for (i = 0; i < NUMBEROF(K_VSH_COLLECTION_PATH_REFERENCES); i++)
				*K_VSH_COLLECTION_PATH_REFERENCES[i] = g_vs_collection_path;

			// replace the sprintf call to intercept the collection path creation and edit it as necessary
			Memory::WriteRelativeCall(&BuildEffectCollectionPath,
											  GET_FUNC_VPTR(RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL), true);
			// Forces the backface normals of a two sided face to be inverted to fix lighting issues on trees
			Memory::WriteRelativeJmp(&Hook_RenderObject_ForceInvertBackfaceNormals,
											 GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK), true);

			for (i = 0; i < NUMBEROF(K_PS_VERSION_ITERATOR_START); i++)
				*K_PS_VERSION_ITERATOR_START[i] = &g_ps_version_iterator_start;
		}

		void Initialize() {
			c_settings_shaderextension::Register(Settings::Manager());

			g_shader_files_present = true;

			// determine if the custom shader collections are present
			for (int i = 0; (i < NUMBEROF(g_shader_collection_ids)) && g_shader_files_present; i++) {
				FileIO::s_file_info       file_info;
				Enums::file_io_open_error file_open_success = FileIO::OpenFileByID(file_info, g_shader_collection_ids[i]);
				FileIO::CloseFile(file_info);

				g_shader_files_present &= (file_open_success == Enums::_file_io_open_error_none);
			}
		}

		void Dispose() {
			c_settings_shaderextension::Unregister(Settings::Manager());
		}

		void Initialize3D(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *params) {
			g_extensions_enabled = false;

			// user settings override to completely disable the shader extensions
			if (!g_extensions_enabled_user_override)
				return;

			// the shader files are not present, so do not set up the hooks
			if (!g_shader_files_present)
				return;

			// get the profile string id
			const char *ps_profile = D3DXGetPixelShaderProfile(device);
			if (!ps_profile)
				return;

			// parse the profile string to get the major version as an integer and the minor version as a char
			int  profile_major      = 0;
			char profile_minor_char = 0;

			int fields_read = sscanf_s(ps_profile, "ps_%i_%c", &profile_major, &profile_minor_char, 1);
			if (fields_read != 2)
				return;

			// no need to have the minor version as an int as we only need to compare it with a or b when major version is 2
			switch (profile_major) {
				case 3:
					g_ps_support = _ps_support_3_0;
					break;
				case 2: {
					if (profile_minor_char == 'a')
						g_ps_support = _ps_support_2_a;
					else if (profile_minor_char == 'b')
						g_ps_support = _ps_support_2_b;
					else
						g_ps_support = _ps_support_2_0;
					break;
				}
				default:
					g_ps_support = _ps_support_legacy;
					break;
			}

			// if the required shader version is supported, put the hooks in place to
			// add normal goodness
			if (g_ps_support > _ps_support_2_0) {
				// modify the pixel shader version used for finding the correct shader collection and shaders
				// the *ShaderVersion value in D3DCAPS doesn't include 2.a and 2.b, instead it just shows 2.0

				// since this value is used to iterate through the shaders when loading, it would start at ps_2_0 and miss
				// the custom shaders at ps_2_1/ps_2_2 and Halo would close because the new shaders are expected, but not loaded

				// this does not affect shader model 3.0 cards as the iteration starts at ps_3_0 and works down.

				D3DCAPS9 *device_caps = DX9::D3DCaps();
				if (g_ps_support == _ps_support_2_a)
					g_ps_version_iterator_start = 0xFFFF0202;
				else if (g_ps_support == _ps_support_2_b)
					g_ps_version_iterator_start = 0xFFFF0201;
				else
					g_ps_version_iterator_start = device_caps->PixelShaderVersion;

				// determine the maximum pixel shader profile the graphics device supports
				struct {
					byte minor_version;
					byte major_version;
				}        ps_version, vs_version;

				DX9::GetSMVersion(DX9::D3DCaps()->VertexShaderVersion, vs_version.major_version, vs_version.minor_version);
				DX9::GetSMVersion(g_ps_version_iterator_start, ps_version.major_version, ps_version.minor_version);

				if (vs_version.major_version < 2)
					return;
				if (ps_version.major_version < 2)
					return;
				else if ((ps_version.major_version == 2) && (ps_version.minor_version == 0))
					return;

				g_extensions_enabled = true;
				ApplyHooks();
				Model::ApplyHooks();
				Environment::ApplyHooks();
				Effect::ApplyHooks();
			}
		}
		void OnLostDevice() {}
		void OnResetDevice(D3DPRESENT_PARAMETERS *params) {}
		void Release() {}
		void Render() {}
	};
};
