/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/rasterizer/rasterizer.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/open_sauce/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace Rasterizer
	{
#pragma region Settings
		class c_settings_container
			: public Configuration::c_configuration_container
		{
			class c_upgrades_container
				: public Configuration::c_configuration_container
			{
			public:
				Configuration::c_configuration_value<bool> m_maximum_rendered_triangles;
#if PLATFORM_VERSION <= 0x1090
				Configuration::c_configuration_value<bool> m_model_node_stretching_fix;
#endif
	
				c_upgrades_container();

			protected:
				const std::vector<Configuration::i_configuration_value* const> GetMembers() final override;
			};

		public:
#if PLATFORM_VERSION <= 0x1090
			Configuration::c_configuration_value<bool> m_use_nvidia_camo;
#endif
			c_upgrades_container m_upgrades;

			c_settings_container();
			
		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override;
		};

		class c_settings_rasterizer
			: public Settings::c_settings_singleton<c_settings_container, c_settings_rasterizer>
		{
		public:
			void PostLoad() final override;
		};
#pragma endregion

		struct s_rasterizer_resolution
		{
			DWORD			width;
			DWORD			height;
			wchar_t			resolution_string[16];
			DWORD			refresh_rate_count;
			DWORD			refresh_rates[8];
		}; BOOST_STATIC_ASSERT(sizeof(s_rasterizer_resolution) == 0x4C);

		void Initialize();
		void Dispose();

		void Update();
	};

	namespace Render
	{
		bool IsRenderingReflection();

		void Initialize();
		void Dispose();

		void SetRenderStates();
	};
};
#endif