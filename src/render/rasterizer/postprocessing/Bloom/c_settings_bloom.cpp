/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_settings_bloom.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/Bloom/c_system_bloom.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Bloom
	{
		c_settings_container::c_settings_container()
			: Configuration::c_configuration_container("Rasterizer.PostProcessing.Bloom")
			, m_enabled("Enabled", true)
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_settings_container::GetMembers()
		{
			return std::vector<i_configuration_value* const> { &m_enabled };
		}

		void c_settings_bloom::PostLoad()
		{
			c_system_bloom::Instance().Enabled() = Get().m_enabled;
		}

		void c_settings_bloom::PreSave()
		{
			Get().m_enabled = c_system_bloom::Instance().Enabled();
		}
	};};};
};
#endif