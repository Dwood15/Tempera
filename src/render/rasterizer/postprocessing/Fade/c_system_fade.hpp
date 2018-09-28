/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingCacheComponent.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Fade
	{
		class c_system_fade : public IPostProcessingComponent
		{
			/////////////////////////////////////////////////
			// static members
		private:
			static c_system_fade g_fade_system;

			/////////////////////////////////////////////////
			// static member accessors
		public:
			static c_system_fade& Instance();

			/////////////////////////////////////////////////
			// members
		private:
			struct
			{
				struct
				{
					bool is_ready;
					bool is_unloaded;
					PAD16;
				}m_flags;

				Enums::pp_component_status status;
				PAD16;
			}m_members;

			/////////////////////////////////////////////////
			// member accessors
		public:
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
			// system setup
		private:
			HRESULT CreateShader();
			void DestroyShader();

			void SetDeviceLost();
			HRESULT SetDeviceReset();

			void Validate();

			void UpdateStatus();

			/////////////////////////////////////////////////
			// system application
		public:
			HRESULT FadeCurrentResult(real fade_amount);
		};
	};};};
};
#endif