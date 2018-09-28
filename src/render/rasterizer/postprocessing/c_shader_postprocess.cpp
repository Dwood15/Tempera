/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include "../dx9/shaders/shader_postprocess_definitions.hpp"
#include "PostProcessingErrorReporting.hpp"
#include "c_post_processing_main.hpp"
#include "../../../cseries/yelo_base.h"
#include "c_shader_include_manager.hpp"

namespace Yelo {
	namespace Rasterizer {
		namespace PostProcessing {
			/////////////////////////////////////////////////
			// member accessors
			void c_shader_postprocess::SetShaderDefinition(TagGroups::s_shader_postprocess_definition *definition) {
				m_members.definition = definition;
			}

			void c_shader_postprocess::SetSourceData(c_shader_data_base *source) {
				m_members.source_data = source;
			}

			LPD3DXEFFECT c_shader_postprocess::GetEffect() {
				return m_members.definition->runtime.dx_effect;
			}

			bool c_shader_postprocess::IsValid() {
				return m_members.definition->runtime.flags.valid_shader_bit;
			}

			/////////////////////////////////////////////////
			// shader setup
			/*!
			 * \brief
			 * Loads the shader, gets the standard variable handles and sets their values.
			 *
			 * \param render_device
			 * The current render device.
			 *
			 * \returns
			 * Non-zero on error.
			 *
			 * Loads the shader, gets the standard variable handles and sets their values.
			 */
			HRESULT c_shader_postprocess::LoadShader(IDirect3DDevice9 *render_device) {
				// attempt to load the shader
				HRESULT result = LoadShaderImpl(render_device);

				if (FAILED(result))
					return result;

				// it was thought this would load the shaders bitmaps, alas it does not...but we do it anyway
				blam::predicted_resources_precache(m_members.definition->predicted_resources);

				// if the shader loaded successfully get the standard variable handles and initialize their values
				GetHandles();
				SetVariables();

				return result;
			}

			/*!
			 * \brief
			 * Clears the standard variable handles, then releases the effect.
			 *
			 * Clears the standard variable handles, then releases the effect.
			 */
			void c_shader_postprocess::UnloadShader() {
				ClearHandles();
				UnloadShaderImpl();
			}

			/*!
			 * \brief
			 * Clears the standard variable handles, then calls device lost on the effect.
			 *
			 * Clears the standard variable handles, then calls device lost on the effect.
			 */
			void c_shader_postprocess::OnDeviceLost() {
				ClearHandles();
				OnDeviceLostImpl();
			}

			/*!
			 * \brief
			 * Calls device reset on the effect then reinitialises the standard variables.
			 *
			 * \returns
			 * Non-zero on error.
			 *
			 * Calls device reset on the effect then reinitialises the standard variables.
			 */
			HRESULT c_shader_postprocess::OnDeviceReset() {
				HRESULT result = OnDeviceResetImpl();

				if (FAILED(result))
					return result;

				// if the shader reset successfully get the standard variable handles and initialize their values
				GetHandles();
				SetVariables();

				return result;
			}

			/*!
			 * \brief
			 * Sets the valid shader flag.
			 *
			 * Sets the valid shader flag.
			 */
			void c_shader_postprocess::Validate() {
				m_members.definition->runtime.flags.valid_shader_bit = ValidateImpl();
			}

			/*!
			 * \brief
			 * Gets handles for any used standard variables in the effect.
			 *
			 * Gets handles for any used standard variables in the effect.
			 */
			void c_shader_postprocess::GetHandles() {
				m_members.definition->runtime.postprocess_handle = GetTechniqueHandle();

				m_members.definition->ortho_wvp_matrix.Initialize(m_members.definition->runtime.dx_effect, "ORTHOWORLDVIEWPROJECTION", true);
				m_members.definition->scene_size.Initialize(m_members.definition->runtime.dx_effect, "SCENESIZE", true);
				//m_members.definition->screen_fov.Initialize(m_members.definition->runtime.dx_effect,
				//	"SCREENFOV", true); // not yet implemented
				m_members.definition->hud_scale.Initialize(m_members.definition->runtime.dx_effect, "HUDSCALE", true);
				m_members.definition->pixel_size.Initialize(m_members.definition->runtime.dx_effect, "PIXELSIZE", true);
				m_members.definition->near_clip_dist.Initialize(m_members.definition->runtime.dx_effect, "NEARCLIPDISTANCE", true);
				m_members.definition->far_clip_dist.Initialize(m_members.definition->runtime.dx_effect, "FARCLIPDISTANCE", true);
				m_members.definition->tex_source.Initialize(m_members.definition->runtime.dx_effect, "TEXSOURCE", true);
				m_members.definition->tex_scene.Initialize(m_members.definition->runtime.dx_effect, "TEXSCENE", true);
				m_members.definition->tex_buffer.Initialize(m_members.definition->runtime.dx_effect, "TEXBUFFER", true);
			}

			/*!
			 * \brief
			 * Resets all of the handles in the shader to NULL.
			 *
			 * Resets all of the handles in the shader to NULL.
			 */
			void c_shader_postprocess::ClearHandles() {
				m_members.definition->runtime.postprocess_handle = NULL;
				m_members.definition->ortho_wvp_matrix.ClearHandles();
				m_members.definition->scene_size.ClearHandles();
				m_members.definition->screen_fov.ClearHandles();
				m_members.definition->hud_scale.ClearHandles();
				m_members.definition->pixel_size.ClearHandles();
				m_members.definition->near_clip_dist.ClearHandles();
				m_members.definition->far_clip_dist.ClearHandles();
				m_members.definition->tex_source.ClearHandles();
				m_members.definition->tex_scene.ClearHandles();
				m_members.definition->tex_buffer.ClearHandles();
			}

			/*!
			 * \brief
			 * Does checks to make sure the shader is valid.
			 *
			 * \returns
			 * True if the shader is valid.
			 *
			 * Does checks to make sure the shader is valid.
			 *
			 * \remarks
			 * Classes that derive from c_shader_postprocess should override this function to add their own
			 * validation checks, but then also call this base function and boolean AND the results
			 * so that the entire shader is checked to be valid.
			 */
			bool c_shader_postprocess::ValidateImpl() {
				bool valid = false;
				do {
					if (!m_members.definition->runtime.dx_effect) break;
					if (!m_members.definition->runtime.active_technique) break;
					if (!m_members.definition->runtime.postprocess_handle) break;
					if (!m_members.definition->ortho_wvp_matrix.IsUsed()) break;
					if (!m_members.source_data) break;
					if (!ValidateShaderVersions()) break;

					valid = true;
				} while (false);

				return valid;
			}

			/*!
			 * \brief
			 * Loads the shaders effect from a shader data source.
			 *
			 * \param render_device
			 * The current render device.
			 *
			 * \returns
			 * Non-zero if an error occurred.
			 *
			 * Loads the shaders effect from a shader data source. The shader must have a shader definition for storing
			 * the effect pointer and variable handles. It must also have a source data object, which removes data handling
			 * from the shader loading code so that subsystems can handle it in their own way.
			 */
			HRESULT c_shader_postprocess::LoadShaderImpl(IDirect3DDevice9 *render_device) {
				HRESULT      hr           = S_OK;
				LPD3DXBUFFER error_buffer = nullptr;

				// if the shader does not have the required data something is horrifically wrong
				if (!m_members.definition || !m_members.source_data)
					return E_FAIL;

				// if the effect has already been created (derived shaders) do not recreate it
				if (m_members.definition->runtime.dx_effect == nullptr) {
					// get the shader data and its size
					DWORD data_size = 0;
					const void *data = m_members.source_data->GetData(data_size);

					// if the data is not available, return E_FAIL

					// for ascii files loaded from external sources, their includes are handled manually
					c_shader_include_manager include_manager(m_members.source_data->GetIncludePath());

					// create the effect
					// macros can be used, and are defined in the source shader data struct
					hr = D3DXCreateEffect(render_device,
												 data,
												 data_size,
												 m_members.source_data->GetMacros(),
												 &include_manager,
												 D3DXSHADER_OPTIMIZATION_LEVEL3,
												 nullptr,
												 &m_members.definition->runtime.dx_effect,
												 &error_buffer);

					// if effect creation failed, report the error
					if (FAILED(hr)) {
						ErrorReporting::Write("Failed to load shader:");
						ErrorReporting::Write(m_members.source_data->DataSourceID());
						if (error_buffer)
							ErrorReporting::WriteD3DXErrors(error_buffer);
					}
					Yelo::safe_release(error_buffer);
				}

				m_members.definition->runtime.active_technique = GetActiveTechnique();;

				return hr;
			}

			/*!
			 * \brief
			 * Releases the effect.
			 *
			 * Releases the effect.
			 */
			void c_shader_postprocess::UnloadShaderImpl() {
				if (m_members.source_data)
					m_members.source_data->Dispose();
				Yelo::safe_release(m_members.definition->runtime.dx_effect);
			}

			/*!
			 * \brief
			 * Calls lost device on the effect.
			 *
			 * Calls lost device on the effect.
			 */
			void c_shader_postprocess::OnDeviceLostImpl() {
				if (m_members.definition->runtime.dx_effect)
					m_members.definition->runtime.dx_effect->OnLostDevice();
			}

			/*!
			 * \brief
			 * Calls device reset on the effect.
			 *
			 * \returns
			 * Non-zero on error.
			 *
			 * Calls device reset on the effect.
			 */
			HRESULT c_shader_postprocess::OnDeviceResetImpl() {
				HRESULT hr = E_FAIL;
				if (m_members.definition->runtime.dx_effect)
					hr = m_members.definition->runtime.dx_effect->OnResetDevice();
				return hr;
			}

			/*!
			 * \brief
			 * Checks that the vertex and pixel shader version are compatible.
			 *
			 * \returns
			 * True if the versions are compatible.
			 *
			 * Checks that the vertex and pixel shader versions are compatible. If either the vertex shader or pixel shader
			 * are version 3.0 or above then the two shader types must have the same version. A shader cannot combine shader
			 * model 3.0 with a shader model less than 3.0.
			 */
			bool c_shader_postprocess::ValidateShaderVersions() {
				IDirect3DDevice9 *render_device = c_post_processing_main::Instance().Globals().render_device;
				D3DCAPS9         &device_caps   = c_post_processing_main::Instance().Globals().device_caps;

				// iterate through each effect
				LPD3DXEFFECT effect = m_members.definition->runtime.dx_effect;

				// iterate through each technique
				D3DXHANDLE technique = effect->GetTechniqueByName(m_members.definition->runtime.active_technique->name);

				if (!technique)
					return false;

				D3DXTECHNIQUE_DESC technique_description;
				effect->GetTechniqueDesc(technique, &technique_description);
				for (UINT j = 0; j < technique_description.Passes; j++) {
					// iterate through each pass
					D3DXHANDLE    pass = effect->GetPass(technique, j);
					D3DXPASS_DESC pass_description;
					effect->GetPassDesc(pass, &pass_description);
					// get the pixel and vertex shader versions
					union {
						DWORD version;
						struct {
							union {
								WORD major_minor_version;
								struct {
									char minor_version;
									char major_version;
								};
							};
							WORD type;
						};
					}             pass_vs_version, pass_ps_version;
					pass_vs_version.version = D3DXGetShaderVersion(pass_description.pVertexShaderFunction);
					pass_ps_version.version = D3DXGetShaderVersion(pass_description.pPixelShaderFunction);

					// check the shaders can be used on the current graphics device
					bool ps_version_ok = !(pass_vs_version.version > device_caps.VertexShaderVersion);
					bool vs_version_ok = !(pass_ps_version.version > device_caps.PixelShaderVersion);
					if (!ps_version_ok || !vs_version_ok) {
						// ErrorReporting::Write("Failed to load shader:");
						// ErrorReporting::Write(m_members.source_data->DataSourceID());
						// ErrorReporting::WriteLine("The device does not support the required shader model [VS:%i.%i, PS:%i.%i]",
						// 								  pass_vs_version.major_version, pass_vs_version.minor_version, pass_ps_version.major_version,
						// 								  pass_ps_version.minor_version);
						return false;
					}

					// if either the pixel or vertex shader version is 3.0 or above, then ensure that
					// they are both of the same version.
					if ((pass_ps_version.version >= D3DPS_VERSION(3, 0) || pass_vs_version.version >= D3DPS_VERSION(3, 0))
						 && (pass_ps_version.major_minor_version != pass_vs_version.major_minor_version)) {
						// ErrorReporting::Write("Failed to load shader:");
						// ErrorReporting::Write(m_members.source_data->DataSourceID());
						// ErrorReporting::WriteLine("Incompatible shader models [VS:%i.%i, PS:%i.%i]",
						// 								  pass_vs_version.major_version, pass_vs_version.minor_version, pass_ps_version.major_version,
						// 								  pass_ps_version.minor_version);
						return false;
					}
				}
				return true;
			}

			/*!
			 * \brief
			 * Retrieves a pointer to the technique block for the current shader model.
			 *
			 * \returns
			 * A s_technique_definition pointer if a valid technique block is present, otherwise NULL.
			 *
			 * Retrieves a pointer to the technique block for the current shader model.
			 */
			TagGroups::s_technique_definition *c_shader_postprocess::GetActiveTechnique() {
				D3DCAPS9 &device_caps = c_post_processing_main::Instance().Globals().device_caps;

				byte vertex_shader_model = D3DSHADER_VERSION_MAJOR(device_caps.VertexShaderVersion);
				byte pixel_shader_model  = D3DSHADER_VERSION_MAJOR(device_caps.VertexShaderVersion);

				// use the lowest shader model to be safe
				byte shader_model = (vertex_shader_model > pixel_shader_model ? pixel_shader_model : vertex_shader_model);

				TagGroups::s_technique_definition *technique = NULL;
				// look for a shader model 3 technique
				for (int i = 0; (i < m_members.definition->techniques.Count) && (technique == NULL); i++) {
					TagGroups::s_technique_definition &element = m_members.definition->techniques[i];

					if ((shader_model > 2) && element.shader_model.sm_3_0_bit)
						technique = &m_members.definition->techniques[i];
				}
				// look for a shader model 2 technique
				for (int i = 0; (i < m_members.definition->techniques.Count) && (technique == NULL); i++) {
					TagGroups::s_technique_definition &element = m_members.definition->techniques[i];

					if ((shader_model > 1) && element.shader_model.sm_2_0_bit)
						technique = &m_members.definition->techniques[i];
				}
				// look for a shader model 1 technique
				for (int i = 0; (i < m_members.definition->techniques.Count) && (technique == NULL); i++) {
					TagGroups::s_technique_definition &element = m_members.definition->techniques[i];

					if ((shader_model > 0) && element.shader_model.sm_1_0_bit)
						technique = &m_members.definition->techniques[i];
				}
				return technique;
			}

			/*!
			 * \brief
			 * Gets the D3DXHANDLE of the active technique from the D3DXEFFECT.
			 *
			 * \returns
			 * The active techniques D3DXHANDLE.
			 *
			 * Gets the D3DXHANDLE of the active technique from the D3DXEFFECT.
			 */
			D3DXHANDLE c_shader_postprocess::GetTechniqueHandle() {
				return m_members.definition->runtime.dx_effect->GetTechniqueByName(m_members.definition->runtime.active_technique->name);
			}

			/////////////////////////////////////////////////
			// shader application
			/*!
			 * \brief
			 * Sets the values of all the standard variables.
			 *
			 * Sets the values of all the standard variables.
			 */
			void c_shader_postprocess::SetVariables() {
				c_post_processing_main::s_post_processing_globals &globals = c_post_processing_main::Instance().Globals();

				LPD3DXEFFECT effect = m_members.definition->runtime.dx_effect;

				effect->SetTechnique(m_members.definition->runtime.postprocess_handle);

				m_members.definition->ortho_wvp_matrix.SetVariable(effect, &globals.ortho_proj_matrix);
				m_members.definition->scene_size.SetVariable(effect, &globals.screen_dimensions);
				m_members.definition->screen_fov.SetVariable(effect, &globals.screen_fov);
				m_members.definition->hud_scale.SetVariable(effect, &globals.m_hud.value);

				m_members.definition->pixel_size.SetVariable(effect, &globals.pixel_size);
				m_members.definition->near_clip_dist.SetVariable(effect, &globals.m_clipping.near_clip);
				m_members.definition->far_clip_dist.SetVariable(effect, &globals.m_clipping.far_clip);
				m_members.definition->tex_scene.SetVariable(effect, NULL);
				m_members.definition->tex_source.SetVariable(effect, NULL);
				m_members.definition->tex_buffer.SetVariable(effect, NULL);

				bool variable_used = false;
				globals.gbuffer->SetDepth(effect, variable_used);
				m_members.definition->runtime.flags.uses_gbuffer_bit |= variable_used;

				globals.gbuffer->SetVelocity(effect, variable_used);
				m_members.definition->runtime.flags.uses_gbuffer_bit |= variable_used;

				globals.gbuffer->SetNormals(effect, variable_used);
				m_members.definition->runtime.flags.uses_gbuffer_bit |= variable_used;

				globals.gbuffer->SetIndex(effect, variable_used);
				m_members.definition->runtime.flags.uses_gbuffer_bit |= variable_used;
			}

			/*!
			 * \brief
			 * Updates the standard variables that can change on each game tick.
			 *
			 * Updates the standard variables that can change on each game tick.
			 */
			void c_shader_postprocess::UpdateVariables() {
				c_post_processing_main::s_post_processing_globals &globals = c_post_processing_main::Instance().Globals();

				// set any standard variables that change per tick
				m_members.definition->screen_fov.SetVariable(m_members.definition->runtime.dx_effect, &globals.screen_fov);
				m_members.definition->hud_scale.SetVariable(m_members.definition->runtime.dx_effect, &globals.m_hud.value);
				m_members.definition->near_clip_dist.SetVariable(m_members.definition->runtime.dx_effect, &globals.m_clipping.near_clip);
				m_members.definition->far_clip_dist.SetVariable(m_members.definition->runtime.dx_effect, &globals.m_clipping.far_clip);
			}

			/*!
			 * \brief
			 * Renders the shader.
			 *
			 * \param render_device
			 * The current render device.
			 *
			 * \param render_quad
			 * The quad instance to draw with.
			 *
			 * \returns
			 * Non-zero if an error occurred.
			 *
			 * Renders the shader.
			 */
			HRESULT c_shader_postprocess::Render(IDirect3DDevice9 *render_device, c_quad_instance *render_quad) {
				LPD3DXEFFECT effect = m_members.definition->runtime.dx_effect;

				;

				// start the effect
				UINT pass_count;
				HRESULT_ERETURN(effect->Begin(&pass_count, 0));

				// if there is no active technique, or the techniques pass count does not match that from the effect
				;;

				DX9::s_render_target_chain_scene &main_chain      = c_post_processing_main::Instance().Globals().scene_buffer_chain;
				DX9::s_render_target_chain       &secondary_chain = c_post_processing_main::Instance().Globals().secondary_buffer_chain;

				// render each pass of the effect
				HRESULT    hr   = E_FAIL;
				for (int32 pass = 0; pass < m_members.definition->runtime.active_technique->passes.Count; ++pass) {
					TagGroups::s_pass_definition &pass_element = m_members.definition->runtime.active_technique->passes[pass];

					// shaders can choose to write to either the main chain or a secondary buffer chain
					DX9::s_render_target_chain *render_chain;
					switch (pass_element.render_chain) {
						case 1:
							render_chain = &secondary_chain;
							break;
						default:
							render_chain = &main_chain;
					}

					// if the shader uses the scene texture, set it
					if (m_members.definition->tex_scene.IsUsed())
						m_members.definition->tex_scene.SetVariable(effect,
																				  main_chain.GetSceneTexture());

					// if the shader uses the source texture, set it
					if (m_members.definition->tex_source.IsUsed())
						m_members.definition->tex_source.SetVariable(effect,
																					main_chain.m_first_render ?
																					main_chain.GetSceneTexture() :
																					main_chain.GetNextTexture());

					// if the shader uses the buffer texture, set it
					// if the buffer has not been written to yet and the buffer texture is not cleared set to the scene texture
					if (m_members.definition->tex_buffer.IsUsed()) {
						if (secondary_chain.m_first_render && !pass_element.flags.clear_buffer_texture_bit)
							m_members.definition->tex_buffer.SetVariable(effect, main_chain.GetSceneTexture());
						else
							m_members.definition->tex_buffer.SetVariable(effect, secondary_chain.GetNextTexture());
					}

					// when copy_scene_to_target is true the scene texture is stretchrect'd to the target texture
					// this is so that the shader can draw onto, and sample, the current scene texture
					if (pass_element.flags.copy_scene_to_target_bit)
						render_device->StretchRect(main_chain.GetSceneSurface(), nullptr, render_chain->GetCurrentSurface(), nullptr,
															D3DTEXF_POINT);

					// clear the buffer texture if necessary
					// this should be avoided where possible as it is very expensive
					if (pass_element.flags.clear_buffer_texture_bit) {
						render_device->SetRenderTarget(0, secondary_chain.GetNextSurface());
						render_device->Clear(0L, nullptr, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L);
					}

					// set the render target to the render chains target surface
					render_device->SetRenderTarget(0, render_chain->GetCurrentSurface());

					// if the pass needs a clear target, clear it
					// this should be avoided where possible as it is very expensive
					if (pass_element.flags.clear_target_bit)
						render_device->Clear(0L, nullptr, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L);

					// render the pass
					effect->BeginPass(pass);
					{
						// the vertex and index sources have already been set by the quad manager, the quad instance struct details
						// the location of the quad in the buffers
						hr = render_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
																			  render_quad->Quad().start_vertex,
																			  0,
																			  render_quad->Quad().vertex_count,
																			  render_quad->Quad().start_index,
																			  render_quad->Quad().primitive_count);
					};
					effect->EndPass();

					// swap the render chain textures so this passes result becomes the next passes source texture
					render_chain->m_first_render = false;
					render_chain->Flip();
				}
				effect->End();

				return hr;
			}
		};
	};
};
