#pragma once

#include <macros_generic.h>
#include <enums/generic_enums.h>
#include <enums/shaders.h>
#include <enums/materials.h>
#include "../../tags/group/tagreference.h"

#define TAG_FLAG(name, ...)               unsigned long name##_bit : 1
namespace Yelo::TagGroups {
	struct s_shader_animation_function {
		object_function_reference u_animation_source;
		Enums::periodic_function  u_animation_function;
		real                      animation_period;
		real                      animation_phase;
		real                      animation_scale;
	};
	STAT_ASSERT(s_shader_animation_function, 16);

	struct s_shader_animation {
		s_shader_animation_function anim_u, anim_v, anim_rot;
		real_point2d                rotation_animation_center;
	};
	STAT_ASSERT(s_shader_animation, 56);
	struct s_shader_texture_animation_function {
		////////////////////////////////////////////////////////////////
		// texture scrolling animation
		// Scrolls all 2D maps simultaneously.
		Enums::periodic_function animation_function;
		short : 16;
		real animation_period;
		real animation_scale;
	};
	STAT_ASSERT(s_shader_texture_animation_function, 12);

	struct s_shader_scaled_map {
		real          scale;
		tag_reference map;
	};
	STAT_ASSERT(s_shader_scaled_map, 20);

	struct s_shader_color_function {
		real_rgb_color           on_color;
		real_rgb_color           off_color;
		Enums::periodic_function animation_function;
		short : 16;
		real animation_period;
		real animation_phase;
		int  pad_type6[6];
	};
	STAT_ASSERT(s_shader_color_function, 60);

	struct s_shader_shader_framebuffer_function {
		Enums::shader_framebuffer_blend_function shader_framebuffer_blend_function;
		Enums::shader_framebuffer_fade_mode      shader_framebuffer_fade_mode;
		object_function_reference                shader_framebuffer_fade_source;
		short : 16;
	};
	STAT_ASSERT(s_shader_shader_framebuffer_function, 8);

	struct s_shader_radiosity_properties {
		struct __flags {
			byte alpha_tested_bit:1;
			byte decal_bit:1;
			byte two_sided_bit:1;
			byte first_map_is_in_screenspace_bit:1;
			byte draw_before_water_bit:1;
			byte ignore_effect_bit:1;
			byte scale_first_map_with_distance_bit:1;
			byte numeric_bit:1;
		};
		STAT_ASSERT(__flags, sizeof(byte_flags));

		byte                                 numeric_counter_limit;
		__flags                              flags;
		Enums::first_map_type                first_map_type;
		s_shader_shader_framebuffer_function shader_framebuffer_function;
	};
	STAT_ASSERT(s_shader_radiosity_properties, 12);
	//////////////////////////////////////////////////////////////////////////
	struct _shader_definition {
		struct __flags {
			unsigned short simple_parameterization_bit:1;
			unsigned short ignore_normals_bit:1;
			unsigned short transparent_lit_bit:1;
		};

		struct __flags_1 {
			unsigned short unused_bit:1;
		};
		STAT_ASSERT(__flags_1, sizeof(unsigned short));

		struct {
			////////////////////////////////////////////////////////////////
			// radiosity properties
			__flags        flags;
			// Enums::detail_level detail_level;
			short          detail_level;
			real           power;
			real_rgb_color color_of_emitted_light;
			real_rgb_color tint_color;
		} radiosity;

		////////////////////////////////////////////////////////////////
		// physics properties
		__flags_1            flags_1;
		Enums::material_type material_type;

		Enums::shader_type shader_type;
		short              extension_usage;
	};
	STAT_ASSERT(_shader_definition, 0x28);

	struct s_shader_definition {
		enum { k_group_tag = 'shdr' };

		_shader_definition shader;
	};
	STAT_ASSERT(_shader_definition, 0x28);
	//////////////////////////////////////////////////////////////////////////
	struct s_shader_effect_extension {
		byte pad_type4[4];

		// Depth fade
		real depth_fade_distance;
		real camera_fade_distance;

		tag_block pad_type3[3];
	};
#pragma pack(push, 1)
	struct _shader_effect_definition {
		struct _flags {
			unsigned short sort_bias_bit:1;
			unsigned short nonlinear_tint_bit:1;
			unsigned short dont_overdraw_fp_weapon_bit:1;
		};
		STAT_ASSERT(_flags, sizeof(unsigned short));
		struct _map_flags {
			unsigned short unfiltered_bit:1;
		};
		STAT_ASSERT(_map_flags, sizeof(unsigned short));

		_flags                                    flags;
		Enums::shader_framebuffer_blend_function  blend_function;
		Enums::shader_framebuffer_fade_mode       fade_mode;
		_map_flags                                map_flags;
		Yelo::TagBlock<s_shader_effect_extension> shader_extension;
		PAD128;

		struct {
			tag_reference                             bitmap;
			Enums::shader_effect_secondary_map_anchor anchor;
			_map_flags                                flags;
			s_shader_animation                        animation;
		}                                         secondary_map;
		int : 32;
		real z_sprite_radius_scale;
		PAD128;
		int : 32;
	}; STAT_ASSERT(_shader_effect_definition, 0x8C);
#pragma pack(pop)
	// the following tags/blocks have inline instances of this:
	// contrail, lightning_shader_block, particle, particle_system_type_particle_states_block
	// not all of those field sets expose all the available tag fields...
	struct s_shader_effect : s_shader_definition {
		enum { k_group_tag = 'seff' };

		_shader_effect_definition effect;
	}; STAT_ASSERT(s_shader_effect, 0xB4);

	//////////////////////////////////////////////////////////////////////////
	struct s_shader_environment_extension {
		struct _flags {
			unsigned short do_not_use_dlms_bit:1;
		}; STAT_ASSERT(_flags, sizeof(unsigned short));

		struct __specular_color_flags {
			unsigned short alpha_as_exponent_mask_bit:1;
		}; STAT_ASSERT(__specular_color_flags, sizeof(unsigned short));

		_flags flags;
		byte   pad_type20[2];
		real   bump_amount;

		tag_reference          specular_color_map;
		real                   specular_color_coefficient;
		real                   specular_color_exponent;
		__specular_color_flags specular_color_flags;
		byte                   pad_type2A[2];

		real_fraction  perpendicular_brightness;
		real_rgb_color perpendicular_tint_color;
		real_fraction  parallel_brightness;
		real_rgb_color parallel_tint_color;

		real specular_lighting_exponent;
		real specular_lighting_coefficient;

		tag_block pad_type2B[2];
	};

#pragma pack(push, 1)
	struct _shader_environment_definition {
		struct _flags {
			unsigned short alpha_tested_bit:1;
			unsigned short bump_map_is_specular_mask_bit:1;
			unsigned short true_atmospheric_fog_bit:1;
		}; STAT_ASSERT(_flags, sizeof(unsigned short));

		////////////////////////////////////////////////////////////////
		// environment shader
		// Setting <true atmospheric fog> enables per-pixel atmospheric fog (for models) but disables point/spot lights, planar fog, and the ability to control the atmospheric fog density for this shader.
		_flags flags;

		////////////////////////////////////////////////////////////////
		// environment shader type
		// Controls how diffuse maps are combined:
		//
		// NORMAL:
		// Secondary detail map alpha controls blend between primary and secondary detail map. Specular mask is alpha of blended primary and secondary detail map alpha multiplied by alpha of micro detail map.
		//
		// BLENDED:
		// Base map alpha controls blend between primary and secondary detail map. Specular mask is alpha of blended primary and secondary detail map alpha multiplied by alpha of micro detail map.
		//
		// BLENDED BASE SPECULAR:
		// Same as BLENDED, except specular mask is alpha is base map times alpha of micro detail map.
		Enums::environment_shader_type type;

		////////////////////////////////////////////////////////////////
		// lens flares
		struct {
			real          spacing;
			tag_reference reference;
		}                              lens_flare;

		Yelo::TagBlock<s_shader_environment_extension> shader_extension;
		byte                                           pad_type32[0x20];

		////////////////////////////////////////////////////////////////
		// diffuse properties
		struct {
			struct _flags {
				unsigned short rescale_detail_maps_bit:1;
				unsigned short rescale_bump_map_bit:1;
			}; STAT_ASSERT(_flags, sizeof(unsigned short));

			_flags flags;
			short : 16;
			int                        pad_type6[6];
			tag_reference              base_map;
			int                        pad_type60[6];
			Enums::detail_map_function detail_map_function;
			short : 16;
			s_shader_scaled_map        primary_detail_map;
			s_shader_scaled_map        secondary_detail_map;
			int                        pad_type61[6];
			Enums::detail_map_function micro_detail_map_function;
			short : 16;
			s_shader_scaled_map micro_detail_map;
			real_rgb_color      material_color;
			int                 pad_type3[3];
		}                                              diffuse;

		////////////////////////////////////////////////////////////////
		// bump properties
		// Perforated (alpha-tested) shaders use alpha in bump map.
		struct {
			s_shader_scaled_map bump_map;
			UNKNOWN_TYPE(real); // postprocessed
			UNKNOWN_TYPE(real); // postprocessed
			int                 pad_type4[4];
		}                                              bump;

		////////////////////////////////////////////////////////////////
		// texture scrolling animation
		// Scrolls all 2D maps simultaneously.
		s_shader_texture_animation_function texture_scrolling[2];
		int                                 pad_type6[6];

		////////////////////////////////////////////////////////////////
		// self-illumination properties
		// There are three self-illumination effects which are added together. Each effect has an <on color>, used when the shader is active, and an <off color>, used when the shader is not active. The self-illumination map is used as follows:
		// * RED: primary mask
		// * GREEN: secondary mask
		// * BLUE: plasma mask
		// * ALPHA: plasma animation reference
		//
		// Each effect also has an animation <function>, <period> and <phase>, used when the shader is active. The primary and secondary effects simply modulate the <on color> by the animation value to produce an animation color, and then blend between the animation color and the <off color> based on the shader's activation level, and finally modulate by the mask.
		//
		// The plasma shader compares the animation value with the alpha channel of the map (the plasma animation reference) and produces a high value when they are similar and a dark value when they are different. This value modulates the <plasma on color> to produce a plasma animation color, and the rest proceeds just like the primary and secondary effects.
		struct {
			struct _flags {
				unsigned short unfiltered_bit:1;
			}; STAT_ASSERT(_flags, sizeof(unsigned short));

			_flags flags;
			short : 16;
			int                     pad_type6[6];
			s_shader_color_function color_functions[3];
			s_shader_scaled_map     map;
			int                     pad_type60[6];
		}                                   self_illumination;

		////////////////////////////////////////////////////////////////
		// specular properties
		// Controls dynamic specular highlights. The highlight is modulated by <brightness> as well as a blend between <perpendicular color> and <parallel color>.
		//
		// Set <brightness> to zero to disable.
		struct {
			struct _flags {
				unsigned short overbright_bit:1;
				unsigned short extra_shiny_bit:1;
				unsigned short lightmap_is_specular_bit:1;
			};
			STAT_ASSERT(_flags, sizeof(unsigned short));

			_flags flags;
			short : 16;
			int            pad_type4[4];
			real_fraction  brightness;
			int            pad_type5[5];
			real_rgb_color perpendicular_color;
			real_rgb_color parallel_color;
			int            pad_type40[4];
		}                                   specular;

		////////////////////////////////////////////////////////////////
		// reflection properties
		// Controls environment cube map reflections. The color of the cube map is "tinted" by a blend between <perpendicular color> and <parallel color> from the SPECULAR PROPERTIES above, and then modulated by a blend between <perpendicular brightness> and <parallel brightness>.
		//
		// BUMPED CUBE MAP:
		// This type of reflection uses the shader's bump map (if it exists) to affect the reflection, as well as the perpendicular and parallel brightness (i.e. the "fresnel" effect).
		//
		// FLAT CUBE MAP:
		// This is the fastest type of reflection. The bump map is used to attenuate the fresnel effect, but the reflection image itself is not bumped.
		//
		// Clear <reflection cube map> or set both <perpendicular brightness> and <parallel brightness> to zero to disable.
		struct {
			struct _flags {
				unsigned short dynamic_mirror_bit:1;
			}; STAT_ASSERT(_flags, sizeof(unsigned short));

			_flags                 flags;
			Enums::reflection_type type;
			real_fraction          lightmap_brightness_scale;
			int                    pad_type7[7];
			real_fraction          perpendicular_brightness;
			real_fraction          parallel_brightness;
			int                    pad_type4[4];
			int                    pad_type2[2];
			int                    pad_type40[4];
			tag_reference          cube_map;
			int                    pad_type41[4];
		}                                   reflection;
	};
#pragma pack(pop)
	STAT_ASSERT(_shader_environment_definition, 0x31C);

	struct s_shader_environment_definition : s_shader_definition {
		enum { k_group_tag = 'senv' };

		_shader_environment_definition environment;
	}; STAT_ASSERT(s_shader_environment_definition, 0x344); // max count: 1
	//////////////////////////////////////////////////////////////////////////
	struct s_shader_model_extension {
		struct __specular_color_flags {
			unsigned short alpha_as_exponent_mask_bit:1;
		}; STAT_ASSERT(__specular_color_flags, sizeof(unsigned short));

		struct __diffuse_lighting_flags {
			unsigned short do_not_use_dlms_bsp_bit:1;
		}; STAT_ASSERT(__diffuse_lighting_flags, sizeof(unsigned short));

		struct s_map {
			tag_reference map;
			union {
				byte pad_type16[16];
				struct {
					real coefficient; //used by all
					union {
						struct {
							int : 32;
							real                   exponent;  //only used by specular color
							__specular_color_flags flags;
						};
						struct {
							real scale;  //only used by detail normals
							real v_scale;  //only used by detail normals
						};
					};
				};
			}             modifiers;
		};

		s_map specular_color;   //32
		s_map base_normal;   //32
		s_map detail_normals[2];  //64

		real_fraction  perpendicular_brightness;
		real_rgb_color perpendicular_tint_color;
		real_fraction  parallel_brightness;
		real_rgb_color parallel_tint_color;

		__diffuse_lighting_flags diffuse_lighting_flags;
		short : 16;
		int   : 32;

		real specular_lighting_exponent;
		real specular_lighting_coefficient;

		byte pad_type16[16];
	};
	struct _shader_model_definition {
		struct __flags {
			unsigned short detail_after_reflection_bit:1;
			unsigned short two_sided_bit:1;
			unsigned short not_alpha_tested_bit:1;
			unsigned short alpha_blended_decal_bit:1;
			unsigned short true_atmospheric_fog_bit:1;
			unsigned short disable_two_sided_culling_bit:1;
		}; STAT_ASSERT(__flags, sizeof(unsigned short));

		struct __flags_1 {
			unsigned short no_random_phase_bit:1;
		}; STAT_ASSERT(__flags_1, sizeof(unsigned short));

		////////////////////////////////////////////////////////////////
		// model shader
		// Setting <true atmospheric fog> enables per-pixel atmospheric fog but disables point/spot lights, planar fog, and the ability to control the atmospheric fog density for this shader.
		__flags flags;
		short : 16;
		int           pad_type3[3];
		real_fraction translucency;
		int           pad_type4[4];

		////////////////////////////////////////////////////////////////
		// change color
		// Change color is used to recolor the diffuse map, it affects pixels based on the BLUE channel of the multipurpose map.
		object_change_color_reference change_color_source;
		short : 16;
		int pad_type7[7];

		struct {
			////////////////////////////////////////////////////////////////
			// self-illumination
			// Self-illumination adds diffuse light to pixels based on the GREEN channel of the multipurpose map. The external self-illumination color referenced by <color source> is modulated by the self-illumination animation.
			__flags_1 flags_1;
			short : 16;
			object_change_color_reference color_source;
			Enums::periodic_function      animation_function;
			real                          animation_period;
			real_rgb_color                animation_color_lower_bound;
			real_rgb_color                animation_color_upper_bound;
			int                           pad_type3[3];
		}   self_illumination;

		struct { // 0x9C
			////////////////////////////////////////////////////////////////
			// maps
			// Base map alpha is used for alpha-testing.
			//
			// Multipurpose map is used for the following:
			// * RED: specular reflection mask (modulates reflections)
			// * GREEN: self-illumination mask (adds to diffuse light)
			// * BLUE: primary change color mask (recolors diffuse map)
			// * ALPHA: auxiliary mask
			//
			// Note that when DXT1 compressed color-key textures are used for the multipurpose map (as they should be normally), the alpha channel is 1-bit and any non-zero alpha pixels must have zero-color, therefore the secondary change color mask cannot affect pixels already affected by any of the other channels.
			//
			// Detail map affects diffuse map, and optionally affects reflection if <detail after reflection> flag is set.
			real                                     map_u_scale;
			real                                     map_v_scale;
			tag_reference                            base_map;
			int                                      pad_type2[2];
			tag_reference                            multipurpose_map;
			int                                      pad_type02[2];
			short                                    detail_function; //Enums::detail_function
			Enums::detail_mask                       detail_mask;
			s_shader_scaled_map                      detail_map;
			real                                     detail_map_v_scale;
			Yelo::TagBlock<s_shader_model_extension> shader_extension;
			s_shader_animation                       animation;
			int                                      pad_type20[2];
		}   maps;

		struct {
			////////////////////////////////////////////////////////////////
			// reflection properties
			real           reflection_falloff_distance;
			real           reflection_cutoff_distance;
			real_fraction  perpendicular_brightness;
			real_rgb_color perpendicular_tint_color;
			real_fraction  parallel_brightness;
			real_rgb_color parallel_tint_color;
			tag_reference  reflection_cube_map;
		}   reflection_properties;

		int pad_type4_0[4];
		int : 32;
		int pad_type4_1[4];
		int pad_type8[8];
	};
	STAT_ASSERT(_shader_model_definition, 0x190);

	struct s_shader_model_definition : s_shader_definition {
		enum { k_group_tag = 'soso' };

		_shader_model_definition model;
	};
	STAT_ASSERT(s_shader_model_definition, 0x1B8); // max count: 1
	//////////////////////////////////////////////////////////////////////////
	struct s_shader_transparent_layer {
		tag_reference shader;
	};
	STAT_ASSERT(s_shader_transparent_layer, 0x10); // max count: 4
	struct s_shader_transparent_chicago_map {
		struct __flags {
			unsigned short unfiltered_bit:1;
			unsigned short alpha_replicate_bit:1;
			unsigned short u_clamped_bit:1;
			unsigned short v_clamped_bit:1;
		};
		STAT_ASSERT(__flags, sizeof(unsigned short));

		__flags flags;
		short : 16;
		int                   pad_type10[10];
		Enums::color_function color_function;
		Enums::color_function alpha_function;
		int                   pad_type9[9];
		real                  map_u_scale;
		real                  map_v_scale;
		real                  map_u_offset;
		real                  map_v_offset;
		real                  map_rotation;
		real_fraction         mipmap_bias;
		tag_reference         map;
		int                   pad_type10_0[10];

		////////////////////////////////////////////////////////////////
		// 2D texture animation
		s_shader_animation map_animation;
	};
	STAT_ASSERT(s_shader_transparent_chicago_map, 0xDC); // max count: 4
	struct s_shader_transparent_chicago_base {
		struct __extra_flags {
			TAG_FLAG(don_t_fade_active_camouflage);
			TAG_FLAG(numeric_countdown_timer);
		};
		STAT_ASSERT(__extra_flags, sizeof(long_flags));

		////////////////////////////////////////////////////////////////
		// chicago shader
		s_shader_radiosity_properties radiosity_properties;

		////////////////////////////////////////////////////////////////
		// lens flares
		real          lens_flare_spacing;
		tag_reference lens_flare;
	};
	//////////////////////////////////////////////////////////////////////////
	struct _shader_transparent_chicago_definition : s_shader_transparent_chicago_base {
		Yelo::TagBlock<s_shader_transparent_layer>       extra_layers;
		Yelo::TagBlock<s_shader_transparent_chicago_map> maps;
		__extra_flags                                    extra_flags;
		int                                              pad_type2[2];
	};

	struct s_shader_transparent_chicago_definition : s_shader_definition {
		enum { k_group_tag = 'schi' };

		_shader_transparent_chicago_definition transparent_chicago;
	};
	STAT_ASSERT(s_shader_transparent_chicago_definition, 0x6C); // max count: 1
	//////////////////////////////////////////////////////////////////////////
	struct _shader_transparent_chicago_extended_definition : s_shader_transparent_chicago_base {
		Yelo::TagBlock<s_shader_transparent_layer>       extra_layers;
		Yelo::TagBlock<s_shader_transparent_chicago_map> _4_stage_maps;
		Yelo::TagBlock<s_shader_transparent_chicago_map> _2_stage_maps;
		__extra_flags                                    extra_flags;
		int                                              pad_type2[2];
	};

	struct s_shader_transparent_chicago_extended_definition : s_shader_definition {
		enum { k_group_tag = 'scex' };

		_shader_transparent_chicago_extended_definition transparent_chicago_extended;
	};
	STAT_ASSERT(s_shader_transparent_chicago_extended_definition, 0x78); // max count: 1
	//////////////////////////////////////////////////////////////////////////
	struct s_shader_transparent_generic_map {
		struct __flags {
			unsigned short unfiltered_bit:1;
			unsigned short u_clamped_bit:1;
			unsigned short v_clamped_bit:1;
		}; STAT_ASSERT(__flags, sizeof(unsigned short));

		__flags flags;
		short : 16;
		real          map_u_scale;
		real          map_v_scale;
		real          map_u_offset;
		real          map_v_offset;
		real          map_rotation;
		real_fraction mipmap_bias;
		tag_reference map;

		////////////////////////////////////////////////////////////////
		// 2D texture animation
		s_shader_animation map_animation;
	};
	STAT_ASSERT(s_shader_transparent_generic_map, 0x64); // max count: 4
	struct s_shader_transparent_generic_stage {
		struct __flags {
			unsigned short color_mux_bit:1;
			unsigned short alpha_mux_bit:1;
			unsigned short a_out_controls_color0_animation_bit:1;
		};
		STAT_ASSERT(__flags, sizeof(unsigned short));

		__flags flags;
		short : 16;

		////////////////////////////////////////////////////////////////
		// constants and animation
		// Constant color 0 is animated in exactly the same way as the self-illumination color of the model shader, except that it has an alpha component in addition to the RGB components. Constant color 1 is just a constant.
		object_change_color_reference color0_source;
		Enums::periodic_function      color0_animation_function;
		real                          color0_animation_period;
		real_argb_color               color0_animation_lower_bound;
		real_argb_color               color0_animation_upper_bound;
		real_argb_color               color1;

		////////////////////////////////////////////////////////////////
		// color inputs
		struct {
			Enums::color_input         input;
			Enums::color_input_mapping input_mapping;
		}                             color_inputs[4];

		////////////////////////////////////////////////////////////////
		// color outputs
		struct {
			Enums::color_output          output;
			Enums::color_output_function output_function;
		}                             color_outputs[2];
		Enums::color_output           output_ab_cd_mux_sum;
		Enums::output_mapping         output_mapping;

		////////////////////////////////////////////////////////////////
		// alpha inputs
		struct {
			Enums::alpha_input         input_;
			Enums::color_input_mapping input_mapping;
		}                             alpha_inputs[4];

		////////////////////////////////////////////////////////////////
		// alpha outputs
		struct {
			Enums::alpha_output output;
		}                             alpha_outputs[2];
		Enums::alpha_output           output_ab_cd_mux_sum_1;
		Enums::output_mapping         output_mapping_1;
	};
	STAT_ASSERT(s_shader_transparent_generic_stage, 0x70); // max count: 7
	struct _shader_transparent_generic_definition {
		////////////////////////////////////////////////////////////////
		// generic transparent shader
		s_shader_radiosity_properties radiosity_properties;

		////////////////////////////////////////////////////////////////
		// lens flares
		real                                               lens_flare_spacing;
		tag_reference                                      lens_flare;
		Yelo::TagBlock<s_shader_transparent_layer>         extra_layers;
		Yelo::TagBlock<s_shader_transparent_generic_map>   maps;
		Yelo::TagBlock<s_shader_transparent_generic_stage> stages;
	};

	struct s_shader_transparent_generic_definition : s_shader_definition {
		enum { k_group_tag = 'sotr' };

		_shader_transparent_generic_definition transparent_generic;
	};
	STAT_ASSERT(s_shader_transparent_generic_definition, 0x6C); // max count: 1
	//////////////////////////////////////////////////////////////////////////
	struct _shader_transparent_glass_definition {
		struct __flags {
			unsigned short alpha_tested_bit:1;
			unsigned short decal_bit:1;
			unsigned short two_sided_bit:1;
			unsigned short bump_map_is_specular_mask_bit:1;
		};
		STAT_ASSERT(__flags, sizeof(unsigned short));

		////////////////////////////////////////////////////////////////
		// glass shader
		__flags flags;
		short               : 16;

		////////////////////////////////////////////////////////////////
		// background tint properties
		// Background pixels are multiplied by the tint map and constant tint color.
		int                 pad_type10[10];
		real_rgb_color      background_tint_color;
		s_shader_scaled_map background_tint_map;
		int                 pad_type5[5];

		////////////////////////////////////////////////////////////////
		// reflection properties
		// Reflection maps are multiplied by fresnel terms (glancing angles cause reflections to disappear) and then added to the background. The primary reflection map is textured normally, while the secondary reflection map is magnified.
		short : 16;
		Enums::reflection_type reflection_type;
		real_fraction          perpendicular_brightness;
		real_rgb_color         perpendicular_tint_color;
		real_fraction          parallel_brightness;
		real_rgb_color         parallel_tint_color;
		tag_reference          reflection_map;
		s_shader_scaled_map    bump_map;
		int                    pad_type32[32];

		////////////////////////////////////////////////////////////////
		// diffuse properties
		// Diffuse lights are accumulated in monochrome and then alpha-blended with diffuse map and diffuse detail map. The color is determined by double-multiplying both maps and multiplying with the accumulated light, the result being alpha-blended into the framebuffer. The opacity is determined by multiplying both map's alpha channels. Since this effect is alpha-blended, it covers up tinting and reflection on pixels with high opacity.
		int : 32;
		s_shader_scaled_map diffuse_map;
		s_shader_scaled_map diffuse_detail_map;
		int                 pad_type7[7];

		////////////////////////////////////////////////////////////////
		// specular properties
		// Specular lights are accumulated in monochrome and then alpha-blended with diffuse map and diffuse detail map. The color is determined by double-multiplying both maps and multiplying with the accumulated light, the result being alpha-blended into the framebuffer. The opacity is determined by multiplying both map's alpha channels. Since this effect is alpha-blended, it covers up tinting, reflection and diffuse texture on pixels with high opacity.
		int : 32;
		s_shader_scaled_map specular_map;
		s_shader_scaled_map specular_detail_map;
		int                 pad_type7_0[7];
	};

	struct s_shader_transparent_glass_definition : s_shader_definition {
		enum { k_group_tag = 'sgla' };

		_shader_transparent_glass_definition transparent_glass;
	}; STAT_ASSERT(s_shader_transparent_glass_definition, 0x1E0); // max count: 1
	//////////////////////////////////////////////////////////////////////////
	struct _shader_transparent_meter_definition {
		struct __flags {
			unsigned short decal_bit:1;
			unsigned short two_sided_bit:1;
			unsigned short flash_color_is_negative_bit:1;
			unsigned short tint_mode_2_bit:1;
			unsigned short unfiltered_bit:1;
		}; STAT_ASSERT(__flags, sizeof(unsigned short));

		////////////////////////////////////////////////////////////////
		// meter shader
		__flags flags;
		short : 16;
		int           pad_type8[8];
		tag_reference map;
		int           pad_type8_0[8];

		////////////////////////////////////////////////////////////////
		// colors
		real_rgb_color gradient_min_color;
		real_rgb_color gradient_max_color;
		real_rgb_color background_color;
		real_rgb_color flash_color;
		real_rgb_color tint_color;
		real_fraction  meter_transparency;
		real_fraction  background_transparency;
		int            pad_type6[6];

		////////////////////////////////////////////////////////////////
		// external function sources
		object_function_reference meter_brightness_source;
		object_function_reference flash_brightness_source;
		object_function_reference value_source;
		object_function_reference gradient_source;
		object_function_reference flash_extension_source;
		short : 16;
		int pad_type8_1[8];
	};

	struct s_shader_transparent_meter_definition : s_shader_definition {
		enum { k_group_tag = 'smet' };

		_shader_transparent_meter_definition transparent_meter;
	}; STAT_ASSERT(s_shader_transparent_meter_definition, 0x104); // max count: 1
	//////////////////////////////////////////////////////////////////////////
	struct _shader_transparent_plasma_definition {
		////////////////////////////////////////////////////////////////
		// plasma shader
		short : 16;
		short : 16;

		////////////////////////////////////////////////////////////////
		// intensity
		// Controls how bright the plasma is.
		object_function_reference intensity_source;
		short : 16;
		real intensity_exponent;

		////////////////////////////////////////////////////////////////
		// offset
		// Controls how far the plasma energy extends from the model geometry.
		object_function_reference offset_source;
		short : 16;
		real offset_amount;
		real offset_exponent;
		int  pad_type8[8];

		////////////////////////////////////////////////////////////////
		// color
		// Controls the tint color and Fresnel brightness effects.
		real_fraction                 perpendicular_brightness;
		real_rgb_color                perpendicular_tint_color;
		real_fraction                 parallel_brightness;
		real_rgb_color                parallel_tint_color;
		object_change_color_reference tint_color_source;
		short : 16;
		int pad_type8_1[8];
		short               : 16;
		short               : 16;
		int pad_type4[4];

		////////////////////////////////////////////////////////////////
		// primary noise map
		int : 32;
		int : 32;
		real                primary_animation_period;
		real_vector3d       primary_animation_direction;
		s_shader_scaled_map primary_noise_map;
		int                 pad_type8_0[8];

		////////////////////////////////////////////////////////////////
		// secondary noise map
		int : 32;
		real                secondary_animation_period;
		real_vector3d       secondary_animation_direction;
		s_shader_scaled_map secondary_noise_map;
		int                 pad_type8_2[8];
	};

	struct s_shader_transparent_plasma_definition : s_shader_definition {
		enum { k_group_tag = 'spla' };
		_shader_transparent_plasma_definition transparent_plasma;
	};
	STAT_ASSERT(s_shader_transparent_plasma_definition, 0x14C); // max count: 1
	//////////////////////////////////////////////////////////////////////////
	struct s_shader_transparent_water_ripple {
		short : 16;
		short : 16;
		real_fraction contribution_factor;
		int           pad_type8[8];
		angle         animation_angle;
		real          animation_velocity;
		real_vector2d map_offset;
		int16         map_repeats;
		int16         map_index;
		int           pad_type4[4];
	};
	STAT_ASSERT(s_shader_transparent_water_ripple, 0x4C); // max count: 4

	struct _shader_transparent_water_definition {
		struct __flags {
			unsigned short base_map_alpha_modulates_reflection_bit:1;
			unsigned short base_map_color_modulates_background_bit:1;
			unsigned short atmospheric_fog_bit:1;
			unsigned short draw_before_fog_bit:1;
		};
		STAT_ASSERT(__flags, sizeof(unsigned short));

		////////////////////////////////////////////////////////////////
		// water shader
		// Base map color modulates the background, while base map alpha modulates reflection brightness. Both of these effects can be independently enables and disabled. Note that if the <base map alpha modulates reflection> flag is not set, then the perpendicular/parallel brightness has no effect (but the perpendicular/parallel tint color DOES has an effect).
		__flags flags;
		short : 16;
		int            pad_type8[8];
		tag_reference  base_map;
		int            pad_type4[4];
		real_fraction  view_perpendicular_brightness;
		real_rgb_color view_perpendicular_tint_color;
		real_fraction  view_parallel_brightness;
		real_rgb_color view_parallel_tint_color;
		int            pad_type4_0[4];
		tag_reference  reflection_map;
		int            pad_type4_1[4];
		angle          ripple_animation_angle;
		real           ripple_animation_velocity;
		real           ripple_scale;
		tag_reference  ripple_maps;
		int16          ripple_mipmap_levels;
		short : 16;
		real_fraction                                     ripple_mipmap_fade_factor;
		real                                              ripple_mipmap_detail_bias;
		int                                               pad_type16[16];
		Yelo::TagBlock<s_shader_transparent_water_ripple> ripples;
		int                                               pad_type4_2[4];
	};

	struct s_shader_transparent_water_definition : s_shader_definition {
		enum { k_group_tag = 'swat' };

		_shader_transparent_water_definition transparent_water;
	};
	STAT_ASSERT(s_shader_transparent_water_definition, 0x140); // max count: 1
	//////////////////////////////////////////////////////////////////////////
};
