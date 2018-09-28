/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/External/c_shader_external.hpp"

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic { namespace External
	{
		void c_shader_external::SetShaderPath(const char* path)
		{
			m_members_external.shader_path[0] = '\0';
			m_members_external.include_path[0] = '\0';

			// copy the absolute path
			strcpy_s(m_members_external.shader_path, sizeof(m_members_external.shader_path), path);

			// copy the directory path for relative files in the shader
			const char* filename_offset = strrchr(path, '\\');
			if(!filename_offset)
				return;

			uint32 index = CAST(uint32, filename_offset - path);
			// include the path divider
			index++;
			strncpy_s(m_members_external.include_path, sizeof(m_members_external.include_path), path, index);
		}
		void c_shader_external::SetShaderID(const char* id)
		{
			m_members_external.shader_id[0] = '\0';
			strncpy_s(m_members_external.shader_id, sizeof(m_members_external.shader_id), id,
				(strlen(id) > sizeof(m_members_external.shader_id) ? sizeof(m_members_external.shader_id) : strlen(id)));
		}
		const char* c_shader_external::GetShaderID()
		{
			return m_members_external.shader_id;
		}
		void c_shader_external::SetupShader()
		{
			// if the source data is not null this shader has not been Dtor'd, which is bad
			YELO_ASSERT_DISPLAY(m_members_external.shader_source_data == nullptr, "source data pointer being set before it has been deleted and nulled");

			// create a source data class for the base class to compile the shader from
			m_members_external.shader_source_data = new c_shader_data_external();
			m_members_external.shader_source_data->Ctor();
			m_members_external.shader_source_data->SetFilePath(m_members_external.shader_path, m_members_external.include_path);
			SetSourceData(m_members_external.shader_source_data);

			c_shader_generic::SetupShader();
		}
	};};};};
};
#endif