/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include <Common/Precompile.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include <blamlib/Halo1/cache/texture_cache.hpp>

namespace Yelo
{
	namespace TagGroups
	{
#if !PLATFORM_IS_EDITOR && !PLATFORM_IS_DEDI
		/////////////////////////////////////////////////////////////////////
		// s_shader_postprocess_parameter
		void s_shader_postprocess_parameter::GetBoundingValues(void*& lower_ref, void*& upper_ref)
		{
			switch(value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				value.boolean.inverse = !value.boolean.enabled;

				lower_ref = &value.boolean.enabled;
				upper_ref = &value.boolean.inverse;
				break;

			case Enums::_shader_variable_base_type_integer:
				lower_ref = &value.integer32.lower_bound;
				upper_ref = &value.integer32.upper_bound;
				break;

			case Enums::_shader_variable_base_type_float:
				switch(value_type.count)
				{
				case 1:
					lower_ref = &value.real32.lower_bound;
					upper_ref = &value.real32.upper_bound;
					break;
				case 2:
					lower_ref = &value.vector2d.lower_bound;
					upper_ref = &value.vector2d.upper_bound;
					break;
				case 3:
					lower_ref = &value.vector3d.lower_bound;
					upper_ref = &value.vector3d.upper_bound;
					break;
				case 4:
					lower_ref = &value.vector4d.lower_bound;
					upper_ref = &value.vector4d.upper_bound;
					break;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				lower_ref = &value.color4d.lower_bound;
				upper_ref = &value.color4d.upper_bound;
				break;
			}
		}

		void s_shader_postprocess_parameter::SetVariableInterp(LPD3DXEFFECT effect, const void* lower_data, const void* upper_data, const real* interp_values)
		{
			switch(value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				value.boolean.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
				break;

			case Enums::_shader_variable_base_type_integer:
				value.integer32.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
				break;

			case Enums::_shader_variable_base_type_float:
				switch(value_type.count)
				{
				case 1: value.real32.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
					break;
				case 2: value.vector2d.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
					break;
				case 3: value.vector3d.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
					break;
				case 4: value.vector4d.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
					break;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				value.color4d.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
				break;
			}
		}

		void s_shader_postprocess_parameter::SetVariable(LPD3DXEFFECT effect, void* data, const bool fixup_argb_color_hack)
		{
			switch(value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				value.boolean.handle.SetVariable(effect, data, fixup_argb_color_hack);
				break;

			case Enums::_shader_variable_base_type_integer:
				value.integer32.handle.SetVariable(effect, data, fixup_argb_color_hack);
				break;

			case Enums::_shader_variable_base_type_float:
				switch(value_type.count)
				{
				case 1: value.real32.handle.SetVariable(effect, data, fixup_argb_color_hack);
					break;
				case 2: value.vector2d.handle.SetVariable(effect, data, fixup_argb_color_hack);
					break;
				case 3: value.vector3d.handle.SetVariable(effect, data, fixup_argb_color_hack);
					break;
				case 4: value.vector4d.handle.SetVariable(effect, data, fixup_argb_color_hack);
					break;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				value.color4d.handle.SetVariable(effect, data, fixup_argb_color_hack);
				break;
			}
		}


		HRESULT s_shader_postprocess_parameter::LoadBitmap(IDirect3DDevice9* pDevice)
		{
			HRESULT hr = E_FAIL;
			if(value_type.type != Enums::_shader_variable_base_type_texture)
				return hr;

			if(!bitmap_value.flags.is_external_bit)
			{
				if(bitmap_value.runtime._internal.bitmap != nullptr)
				{
					// Load the bitmap into the pc texture cache, and block until the he direct3d texture is created
					blam::texture_cache_bitmap_get_hardware_format(bitmap_value.runtime._internal.bitmap, true, true);

					hr = (bitmap_value.runtime._internal.bitmap->hardware_format == nullptr ? E_FAIL : S_OK);
				}
			}
			else
			{
				if(bitmap_value.runtime.external.source != nullptr)
					hr = D3DXCreateTextureFromFile(pDevice, bitmap_value.runtime.external.source, &bitmap_value.runtime.external.texture_2d);
			}
			bitmap_value.flags.is_loaded_bit = SUCCEEDED(hr);
			return hr;
		}

		void s_shader_postprocess_parameter::ReleaseBitmap()
		{
			if(value_type.type != Enums::_shader_variable_base_type_texture)
				return;

			// No need to release the bitmap if it is internal, Halo handles that itself
			if(bitmap_value.flags.is_external_bit)
				Yelo::safe_release(bitmap_value.runtime.external.texture_2d);
		}

		IDirect3DTexture9* s_shader_postprocess_parameter::GetTexture()
		{
			if(value_type.type != Enums::_shader_variable_base_type_texture)
				return nullptr;

			if(!bitmap_value.flags.is_loaded_bit)
				return nullptr;
			return bitmap_value.flags.is_external_bit ? bitmap_value.runtime.external.texture_2d : CAST_PTR(IDirect3DTexture9*,bitmap_value.runtime._internal.bitmap->hardware_format);
		}

#elif PLATFORM_IS_EDITOR
		void s_shader_postprocess_parameter::SetParameter(const s_shader_postprocess_value_base* value_source)
		{
			std::memcpy( value_name,		 value_source->value_name,			sizeof(tag_string));
			std::memcpy(&value_type,		&value_source->value_type,			sizeof(value_type));
			std::memcpy(&value,				&value_source->value,				sizeof(value));
			std::memcpy(&runtime_value,		&value_source->runtime_value,		sizeof(runtime_value));
			std::memcpy(&animation_function,&value_source->animation_function,	sizeof(animation_function));
		}
		void s_shader_postprocess_parameter::SetParameter(const s_shader_postprocess_bitmap* value_source)
		{
			SetParameter(CAST_PTR(const s_shader_postprocess_value_base*, value_source));

			bitmap_value.bitmap.tag_index = value_source->bitmap.tag_index;

			blam::tag_reference_set(bitmap_value.bitmap, value_source->bitmap.group_tag, value_source->bitmap.name);
		}
#endif
	};
};