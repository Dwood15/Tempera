#pragma once

namespace Yelo::Rasterizer::PostProcessing {
	class c_shader_data_postprocess_definition : public c_shader_data_base {
		const char                                 *m_shader_name;
		TagGroups::s_shader_postprocess_definition *m_shader_definition;

		void ClearMembers() {
			// TODO: null m_shader_name?

			m_shader_definition = nullptr;
		}

	public:
		void SetShaderName(const char *shader_name) { m_shader_name = shader_name; }

		void SetShaderDefinition(TagGroups::s_shader_postprocess_definition *definition) { m_shader_definition = definition; }

		void Ctor() {
			ClearMembers();
		}

		void Dtor() {
			ClearMembers();
		}

		const char *DataSourceID();

		const void *GetData(uint32 &data_size);

		void Dispose() {}

		const bool IsBinary() { return true; }

		const char *GetIncludePath() { return nullptr; }

		const D3DXMACRO *GetMacros() { return nullptr; }
	};
};
