/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingCacheComponent.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingUpdatable.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingRenderable.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Bloom
	{
		class c_system_bloom
			: public IPostProcessingCacheComponent
			, public IPostProcessingUpdatable
			, public IPostProcessingRenderable
		{
			/////////////////////////////////////////////////
			// static members
		private:
			static c_system_bloom g_bloom_system;

			/////////////////////////////////////////////////
			// static member accessors
		public:
			static c_system_bloom& Instance();

			/////////////////////////////////////////////////
			// members
		private:
			struct
			{
				struct
				{
					bool is_ready;
					bool is_unloaded;
					bool is_enabled;
					PAD8;
				}m_flags;

				Enums::pp_component_status status;
				PAD16;
			}m_members;

			/////////////////////////////////////////////////
			// member accessors
		public:
			bool& Enabled();
			bool IsReady();
			bool IsUnloaded();

			/////////////////////////////////////////////////
			// IPostProcessingComponent
		public:
			void Initialize();
			void Dispose();

			void InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters);
			void OnLostDevice_Base();
			void OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters);
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
			// IPostProcessingCacheComponent
		public:
			void Initialize_Cache();
			void Dispose_Cache();

			/////////////////////////////////////////////////
			// system setup
		private:
			HRESULT CreateShader();
			void DestroyShader();

			HRESULT CreateEffect();
			void DestroyEffect();

			void SetDeviceLost();
			HRESULT SetDeviceReset();

			void SetBloomShaderVariables();

			void Validate();

			void UpdateStatus();

			/////////////////////////////////////////////////
			// system application
		private:
			bool RenderBloom(IDirect3DDevice9* render_device);

			/////////////////////////////////////////////////
			// scripting
		public:
			void SetBloomSize(real size, real change_time);
			void SetBloomExposure(real exposure, real change_time);
			void SetBloomMixAmount(real mix_amount, real change_time);
			void SetBloomMinimumColor(const real_rgb_color& minimum_color, real change_time);
			void SetBloomMaximumColor(const real_rgb_color& maximum_color, real change_time);
		};
	};};};
};
#endif