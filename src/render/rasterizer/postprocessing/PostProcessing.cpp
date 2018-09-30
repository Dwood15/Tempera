
#include <d3d9types.h>
#include <enums/postprocessing.h>
#include <d3d9.h>
#include <string>

#include "PostProcessing_RenderStates.inl"
#include "c_post_processing_main.h"
#include "PostProcessing_Systems.inl"
#include "c_quad_manager.h"
#include "../../../cseries/MacrosCpp.h"
#include "PostProcessing.h"
#include "../Rasterizer.hpp"
#ifdef API_DEBUG
#define RENDER_STATE(state) "##state##", state
#else
#define RENDER_STATE(state) state
#endif

// TODO: Finish replacing all for( std::size() ) loops with range for (auto& subsystem : g_postprocess_subsystems)

namespace Yelo::Rasterizer::PostProcessing {


	/////////////////////////////////////////////////////
	// s_render_state_manager
	/*!
	 * \brief
	 * Stores current render state values from the device.
	 *
	 * \param render_device
	 * The current render device.
	 *
	 * Stores current render state values from the device.
	 */
	void s_render_state_manager::StoreAndSet(IDirect3DDevice9 *render_device) {
		s_state *current = &m_states[0];
		while (current && current->m_state != D3DRS_FORCE_DWORD) {
			render_device->GetRenderState(current->m_state, reinterpret_cast<DWORD *>(&current->m_stored_value));
			render_device->SetRenderState(current->m_state, current->m_set_value);
			current++;
		}
	}

	/*!
	 * \brief
	 * Resets a set of render states to a stored value.
	 *
	 * \param render_device
	 * The current render device.
	 *
	 * Resets a set of render states to a stored value.
	 */
	void s_render_state_manager::RestoreStates(IDirect3DDevice9 *render_device) {
		s_state *current = &m_states[0];
		while (current && (current->m_state != D3DRS_FORCE_DWORD)) {
			render_device->SetRenderState(current->m_state, current->m_stored_value);
			current++;
		}
	}

	/*
	 * \brief
	 * Sets the initial values for the post processing components.
	 *
	 * Sets the initial values for the post processing components.
	 */
	void Initialize() {
		// if the main post processing component failed to initialize, return
		c_post_processing_main::Instance().Initialize();

		// initialise all of the subsystems
		for (auto &subsystem : g_postprocess_subsystems)
			subsystem.m_component->Initialize();

		// initialize the quad manager
		c_quad_manager::Instance().Initialize();
	}

	/*!
	 * \brief
	 * Deletes memory and resets the values of all post processing components.
	 *
	 * Deletes memory and resets the values of all post processing components.
	 */
	void Dispose() {
		// dispose of all the subsystems
		for (auto &subsystem : g_postprocess_subsystems)
			subsystem.m_component->Dispose();

		// dispose of the main post processing component and quad manager
		c_post_processing_main::Instance().Dispose();

		c_quad_manager::Instance().Dispose();
	}

	/*!
	 * \brief
	 * Performs initialization that only happens when the game starts.
	 *
	 * \param pDevice
	 * The current render device.
	 *
	 * \param pParameters
	 * The presentation parameters of the current device.
	 *
	 * Performs initialization that only happens when the game starts.
	 *
	 * \remarks
	 * Components that implement IPostProcessingComponent will be initialized here.
	 *
	 * \see
	 * IPostProcessingComponent
	 */
	void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pParameters) {
		// initialize the global resources in the main post processing component
		// if this fails, return
		c_post_processing_main::Instance().InitializeResources_Base(pDevice, pParameters);

		YELO_ASSERT_DISPLAY(c_post_processing_main::Instance().IsReady(),
								  "The main post processing component failed to initialise it's resources");

		if (!c_post_processing_main::Instance().IsReady())
			return;

		// initialise the subsystems
		for (auto &subsystem : g_postprocess_subsystems) {
			subsystem.m_component->InitializeResources_Base(pDevice, pParameters);
			subsystem.is_ready = subsystem.m_component->IsReady();
		}

		// create the quad buffers
		c_quad_manager::Instance().InitializeResources_Base(pDevice, pParameters);
	}

	/*!
	 * \brief
	 * Performs device lost logic on all subsystems.
	 *
	 * Performs device lost logic on all subsystems.
	 */
	void OnLostDevice() {
		// run device lost logic for all components
		for (auto &subsystem : g_postprocess_subsystems) {
			subsystem.m_component->OnLostDevice_Base();
			subsystem.is_ready = subsystem.m_component->IsReady();
		}

		// update the ready states of cache components
		for (auto &cache_subsystem : g_postprocess_cache_subsystems)
			g_postprocess_subsystems[cache_subsystem.component_index].is_ready = cache_subsystem.m_component->IsReady();

		// run device lost logic for the main system resources
		c_post_processing_main::Instance().OnLostDevice_Base();

		// release the quad buffers
		c_quad_manager::Instance().OnLostDevice_Base();
	}

	/*!
	 * \brief
	 * Performs device reset logic on all subsystems.
	 *
	 * Performs device reset logic on all subsystems.
	 */
	void OnResetDevice(D3DPRESENT_PARAMETERS *pParameters) {
		// run device reset logic for the main system resources
		c_post_processing_main::Instance().OnResetDevice_Base(pParameters);

		if (c_post_processing_main::Instance().IsUnloaded())
			return;

		YELO_ASSERT_DISPLAY(c_post_processing_main::Instance().IsReady(), "The main post processing component failed to be reset");

		if (!c_post_processing_main::Instance().IsReady())
			return;

		// run device reset logic for normal components
		for (auto &subsystem : g_postprocess_subsystems) {
			subsystem.m_component->OnResetDevice_Base(pParameters);
			subsystem.is_ready = subsystem.m_component->IsReady();
		}

		// update the ready states of cache components
		for (auto &cache_subsystem : g_postprocess_cache_subsystems)
			g_postprocess_subsystems[cache_subsystem.component_index].is_ready = cache_subsystem.m_component->IsReady();

		// create the quad buffers
		c_quad_manager::Instance().OnResetDevice_Base(pParameters);
	}

	void Render() {}

	/*!
	 * \brief
	 * Releases all resources allocated by the subsystems.
	 *
	 * Releases all resources allocated by the subsystems. This is called when the game is closed.
	 */
	void Release() {
		// release normal component resources
		for (auto &subsystem : g_postprocess_subsystems)
			subsystem.m_component->ReleaseResources_Base();

		// release the main system resources
		c_post_processing_main::Instance().ReleaseResources_Base();

		// release the quad buffers
		c_quad_manager::Instance().ReleaseResources_Base();
	}

	/*!
	 * \brief
	 * Runs initialization code for the cache components.
	 *
	 * Runs initialization code for the cache components.
	 *
	 * \remarks
	 * Components that implement IPostProcessingCacheComponent are initialized here.
	 *
	 * \see
	 * IPostProcessingCacheComponent
	 */
	void InitializeForNewMap() {
		// initialize cache dependencies in the main post processing component
		c_post_processing_main::Instance().Initialize_Cache();

		// initialize cache based components
		for (int i = 0; i < std::size(g_postprocess_cache_subsystems); i++)
			g_postprocess_cache_subsystems[i].m_component->Initialize_Cache();

		YELO_ASSERT_DISPLAY(c_post_processing_main::Instance().IsReady(),
								  "The main post processing component failed to be initialised for a new map");
		if (!c_post_processing_main::Instance().IsReady())
			return;

		// runs resource initialization for the cache components
		for (int i = 0; i < std::size(g_postprocess_cache_subsystems); i++) {
			g_postprocess_cache_subsystems[i].m_component->InitializeResources_Cache();
			g_postprocess_subsystems[g_postprocess_cache_subsystems[i].component_index].is_ready = g_postprocess_cache_subsystems[i].m_component->IsReady();
		}

		// create the quad buffers
		c_quad_manager::Instance().InitializeResources_Cache();
	}

	/*!
	 * \brief
	 * Runs disposal code for the cache components.
	 *
	 * Runs disposal code for the cache components.
	 *
	 * \remarks
	 * Components that implement IPostProcessingCacheComponents are disposed here.
	 *
	 * \see
	 * IPostProcessingCacheComponents
	 */
	void DisposeFromOldMap() {
		// release all cache component resources
		for (int i = 0; i < std::size(g_postprocess_cache_subsystems); i++)
			g_postprocess_cache_subsystems[i].m_component->ReleaseResources_Cache();

		// release cache dependent resources of the main system
		c_post_processing_main::Instance().ReleaseResources_Cache();

		// dispose of cache components
		for (int i = 0; i < std::size(g_postprocess_cache_subsystems); i++)
			g_postprocess_cache_subsystems[i].m_component->Dispose_Cache();

		c_post_processing_main::Instance().Dispose_Cache();

		// release the quad buffers
		c_quad_manager::Instance().ReleaseResources_Cache();
	}

	/*!
	 * \brief
	 * Performs an update on all subsystems.
	 *
	 * \param delta_time
	 * The time that has passed in seconds, since the last update.
	 *
	 * Performs an update on all subsystems.
	 */
	void Update(real delta_time) {
		// update global values
		c_post_processing_main::Instance().Update(delta_time);

		// update subsystems
		for (int i = 0; i < std::size(g_postprocess_updatable_subsystems); i++)
			g_postprocess_updatable_subsystems[i].m_component->Update(delta_time);
	}

	void RenderPreAlphaBlended() {
		c_post_processing_main::Instance().PollUpdate();

		RenderSystems(Enums::_postprocess_render_stage_pre_blur);
		RenderSystems(Enums::_postprocess_render_stage_blur);
		RenderSystems(Enums::_postprocess_render_stage_pre_alpha_blended);
	}

	void RenderPreHUD() {
		RenderSystems(Enums::_postprocess_render_stage_pre_hud);
	}

	void RenderPreMenu() {
		RenderSystems(Enums::_postprocess_render_stage_pre_ui);
	}

	void RenderPostMenu() {
		RenderSystems(Enums::_postprocess_render_stage_post_ui);
	}

	/*!
	 * \brief
	 * Performs a full load of the post processing system.
	 *
	 * Performs a full load of the post processing system. This is called when the pp_load function is entered into the console.
	 */
	void LoadSystem() {
		// the system has been loaded already
		if (!c_post_processing_main::Instance().IsUnloaded())
			return;

		// load the main post processing component
		c_post_processing_main::Instance().Load();

		if (!c_post_processing_main::Instance().IsReady())
			return;

		// load all of the subsystems
		for (int i = 0; i < std::size(g_postprocess_subsystems); i++) {
			if (g_postprocess_subsystems[i].m_component->IsUnloaded()) {
				g_postprocess_subsystems[i].m_component->Load();
				g_postprocess_subsystems[i].is_ready = g_postprocess_subsystems[i].m_component->IsReady();
			}
		}

		// recreate the quad buffers
		c_quad_manager::Instance().CreateBuffers();
	}

	/*!
	 * \brief
	 * Unloads the entire post processing system.
	 *
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 *
	 * Unloads the entire post processing system. This only affects the d3d resoures of the components. Any post processing
	 * logic such as variable updates will still be run.
	 */
	void UnloadSystem() {
		// return if the system is already unloaded
		if (c_post_processing_main::Instance().IsUnloaded())
			return;

		// unload all of the subsystems
		for (int i = 0; i < std::size(g_postprocess_subsystems); i++)
			if (!g_postprocess_subsystems[i].m_component->IsUnloaded()) {
				g_postprocess_subsystems[i].m_component->Unload();
				g_postprocess_subsystems[i].is_ready = g_postprocess_subsystems[i].m_component->IsReady();
			}

		// unload the main post process components resources
		c_post_processing_main::Instance().Unload();

		// release the quad buffers
		c_quad_manager::Instance().DestroyBuffers();
	}

	/*!
	 * \brief
	 * Applies all of the subsystems at a specified render stage.
	 *
	 * \param render_stage
	 * The current render stage the function is being called at.
	 *
	 * Applies all of the subsystems at a specified render stage.
	 */
	void RenderSystems(Enums::postprocess_render_stage render_stage) {
		// return if the system is not ready
		if (!c_post_processing_main::Instance().IsReady()) return;
		// return if the engine is rendering a reflection buffer
		if (Render::IsRenderingReflection()) return;

		IDirect3DDevice9 *render_device = c_post_processing_main::Instance().Globals().render_device;

		// reset the render targets so that halos primary buffer is the scene texture
		c_post_processing_main::Instance().Globals().scene_buffer_chain.ResetTargets();

		// set the devices vertex and index sources to the quad buffers
		HRESULT hr = c_quad_manager::Instance().SetBuffers();
		if (SUCCEEDED(hr)) {
			// store and set the necessary render states for this render stage
			g_render_state_managers[render_stage].StoreAndSet(render_device);
			{
				// render the subsystems that are ready
				bool     effects_applied = false;
				for (int i               = 0; i < std::size(g_postprocess_renderable_subsystems); i++)
					if (g_postprocess_subsystems[g_postprocess_renderable_subsystems[i].component_index].is_ready)
						effects_applied |= g_postprocess_renderable_subsystems[i].m_component->Render(render_stage);

				if (effects_applied) {
					// it's probable that the last surface rendered to isn't halos primary buffer
					// in that case, stretchrect the result to the primary buffer
					if ((Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface !=
						  c_post_processing_main::Instance().Globals().scene_buffer_chain.GetSceneSurface()))
						render_device->StretchRect(
							c_post_processing_main::Instance().Globals().scene_buffer_chain.GetSceneSurface(),
							nullptr,
							Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface,
							nullptr,
							D3DTEXF_NONE);
				}
			}
			// restore the render states
			g_render_state_managers[render_stage].RestoreStates(render_device);

			// reset the devices render target to halos primary buffer
			render_device->SetRenderTarget(0, Render::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface);
		}
	}
};

