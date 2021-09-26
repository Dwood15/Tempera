/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include "macros_generic.h"
#include "tag_groups.h"
#include "enums/tag_enums.h"
#include "../../memory/datum_index.h"

namespace Yelo {
	struct s_tag_field {
		Yelo::Enums::field_type type;
		unsigned short pad0;
		const char *name;
		void *definition;
	};
	STAT_ASSERT(s_tag_field, 0xC);

	struct tag_field : s_tag_field {
	public:
		// cast the [definition] pointer to a T*
		template <typename T>
		T *Definition() const { return reinterpret_cast<T *>(definition); }

		// cast the data of [definition] to T
		template <typename T>
		T DefinitionCast() const { return reinterpret_cast<T>(definition); }

		bool IsReadOnly() const {
			return name && strchr(name, Enums::k_tag_field_markup_character_read_only); // NOTE: engine uses strrchr
		}

		bool IsAdvanced() const {
			return name && strchr(name, Enums::k_tag_field_markup_character_advanced); // NOTE: engine uses strrchr
		}

		bool IsBlockName() const {
			return name && strchr(name, Enums::k_tag_field_markup_character_block_name); // NOTE: engine uses strrchr
		}

		bool IsInvisible() const {
			return name && *name == '\0'; // yes, a field with no name wouldn't be considered 'invisible', according to engine code
		}
	};
	STAT_ASSERT(tag_field, 0xC);


	// Called as each element is read from the tag stream
	// NOTE: tag_index is non-standard, and will only be valid when invoked by OS code.
	// We can add additional parameters so long as __cdecl is __cdecl (where the caller cleans up the stack).
	// NOTE: tag_index is not guaranteed to be not NONE! Eg: tag_block_add_element
	typedef bool    (__cdecl *proc_tag_block_postprocess_element)(void *element, Enums::tag_postprocess_mode mode, datum_index tag_index);

	// if [formatted_buffer] returns empty, the default block formatting is done
	typedef const char *(__cdecl *proc_tag_block_format)(datum_index tag_index, void *block, long element_index, char formatted_buffer[Yelo::Enums::k_tag_block_format_buffer_size]);

	// Procedure called during tag_block_delete_element, but before all the child data is freed
	typedef void    (__cdecl *proc_tag_block_delete_element)(void *block, long element_index);

	struct s_tag_block_definition {
	public:
		const char *name;
		long_flags                         flags;
		long                               maximum_element_count;
		size_t                             element_size;
		void                               *unused0;
		tag_field                          *fields;
		void                               *unused1;
		proc_tag_block_postprocess_element postprocess_proc;
		proc_tag_block_format              format_proc;
		proc_tag_block_delete_element      delete_proc;
		byte_swap_code_t                   *byte_swap_codes;
	}; STAT_ASSERT(s_tag_block_definition, 0x2C);

	struct tag_block_definition : s_tag_block_definition{
	public:
		// Yelo::TagGroups::s_tag_field_set_runtime_data *GetRuntimeInfo() const;

		// void SetRuntimeInfo(TagGroups::s_tag_field_set_runtime_data *info);

		struct s_field_iterator {
		private:
			tag_field *m_fields;

			bool IsEndHack() const { return m_fields == nullptr; }

		public:
			s_field_iterator() : m_fields(nullptr) {} // for EndHack
			s_field_iterator(const tag_block_definition *definition) : m_fields(definition->fields) {}

			bool operator !=(const s_field_iterator &other) const {
				if (other.IsEndHack()) {
					return m_fields->type != Enums::_field_terminator;
				}

				if (this->IsEndHack()) {
					return other.m_fields->type != Enums::_field_terminator;
				}

				return m_fields != other.m_fields;
			}

			s_field_iterator &operator ++() {
				++m_fields;
				return *this;
			}

			tag_field &operator *() const {
				return *m_fields;
			}
		};

		s_field_iterator begin() /*const*/
		{
			auto iter = s_field_iterator(this);
			return iter;
		}

		static const s_field_iterator end() /*const*/
		{
			return s_field_iterator();
		}
	};

	STAT_ASSERT(tag_block_definition, 0x2C);
};

#include "enums/generic_enums.h"
namespace byteswaps {
	using namespace Yelo;
	using namespace Yelo::Enums;

	static byte_swap_code_t k_string_byte_swap_codes[] = {sizeof(tag_string), 0};

	static byte_swap_code_t k_char_integer_byte_swap_codes[]  = {_bs_code_1byte, 0};
	static byte_swap_code_t k_short_integer_byte_swap_codes[] = {_bs_code_2byte, 0};
	static byte_swap_code_t k_long_integer_byte_swap_codes[]  = {_bs_code_4byte, 0};

	static byte_swap_code_t k_point2d_byte_swap_codes[] = {_bs_code_2byte, _bs_code_2byte, 0};

	static byte_swap_code_t k_rectangle2d_byte_swap_codes[] = {_bs_code_2byte, _bs_code_2byte, _bs_code_2byte, _bs_code_2byte, 0};

	static byte_swap_code_t k_real_point2d_byte_swap_codes[] = {_bs_code_4byte, _bs_code_4byte, 0};

	static byte_swap_code_t k_real_point3d_byte_swap_codes[] = {_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	static byte_swap_code_t k_real_rgb_color_byte_swap_codes[] = {_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	static byte_swap_code_t k_real_argb_color_byte_swap_codes[] = {_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	static byte_swap_code_t k_real_quaternion_byte_swap_codes[] = {_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	static byte_swap_code_t k_real_plane3d_byte_swap_codes[] = {_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	static byte_swap_code_t k_tag_reference_byte_swap_codes[] = {_bs_code_4byte, sizeof(char *), _bs_code_4byte, sizeof(datum_index), 0};

	static byte_swap_code_t k_tag_block_byte_swap_codes[] = {_bs_code_4byte, _bs_code_4byte, sizeof(Yelo::tag_block_definition *), 0};

	static byte_swap_code_t k_tag_data_byte_swap_codes[] = {_bs_code_4byte, sizeof(long) * 2, sizeof(void *) * 2, 0};

	static byte_swap_code_t k_start_array_byte_swap_codes[] = {_bs_code_array_start, 0};

	static byte_swap_code_t k_end_array_byte_swap_codes[] = {_bs_code_array_end, 0};

	static byte_swap_code_t k_pad_byte_swap_codes[] = {0};

	static byte_swap_code_t k_explanation_byte_swap_codes[] = {0};

	static byte_swap_code_t k_custom_byte_swap_codes[] = {0};

	static byte_swap_code_t k_terminator_byte_swap_codes[] = {_bs_code_array_end, 0};
};

#include "../../math/colors.h"
#include "../../math/int_math.h"
#include "../../math/real_math.h"
#include <iterator>
namespace Yelo::TagGroups {
	using namespace Yelo;
	using namespace byteswaps;

	struct s_tag_field_definition {
		size_t size;                  /// <summary>	The size of a single instance of this field. </summary>
		const char *name;                  /// <summary>	The user-friendly name of this field. </summary>
		byte_swap_code_t *byte_swap_codes;   /// <summary>	The needed for byte swapping an instance of this field. </summary>

		/// <summary>	The C name of this field. Null if it can't be defined in code (eg, _field_custom) </summary>
		const char *code_name;
	};

	const Yelo::TagGroups::s_tag_field_definition k_tag_field_definitions[] = {
		{sizeof(tag_string),           "string",               k_string_byte_swap_codes,          "tag_string"},
		{sizeof(sbyte),                "char integer",         k_char_integer_byte_swap_codes,    "byte"},
		{sizeof(short),                "short integer",        k_short_integer_byte_swap_codes,   "short"},
		{sizeof(long),                 "long integer",         k_long_integer_byte_swap_codes,    "long"},
		{sizeof(real),                 "angle",                k_long_integer_byte_swap_codes,    "angle"},
		{sizeof(tag),                  "tag",                  k_long_integer_byte_swap_codes,    "tag"},
		{sizeof(short),                "enum",                 k_short_integer_byte_swap_codes,   "short"},
		{sizeof(unsigned long),        "long flags",           k_long_integer_byte_swap_codes,    "long_flags"},
		{sizeof(unsigned short),       "word flags",           k_short_integer_byte_swap_codes,   "unsigned short"},
		{sizeof(byte_flags),           "byte flags",           k_char_integer_byte_swap_codes,    "byte_flags"},
		{sizeof(point2d),              "point 2d",             k_point2d_byte_swap_codes,         "point2d"},
		{sizeof(rectangle2d),          "rectangle 2d",         k_rectangle2d_byte_swap_codes,     "rectangle2d"},
		{sizeof(rgb_color),            "rgb color",            k_long_integer_byte_swap_codes,    "rgb_color"},
		{sizeof(argb_color),           "argb color",           k_long_integer_byte_swap_codes,    "argb_color"},

		{sizeof(real),                 "real",                 k_long_integer_byte_swap_codes,    "real"},
		{sizeof(real_fraction),        "real fraction",        k_long_integer_byte_swap_codes,    "real_fraction"},
		{sizeof(real_point2d),         "real point 2d",        k_real_point2d_byte_swap_codes,    "real_point2d"},
		{sizeof(real_point3d),         "real point 3d",        k_real_point3d_byte_swap_codes,    "real_point3d"},
		{sizeof(real_vector2d),        "real vector 2d",       k_real_point2d_byte_swap_codes,    "real_vector2d"},
		{sizeof(real_vector3d),        "real vector 3d",       k_real_point3d_byte_swap_codes,    "real_vector3d"},
		{sizeof(real_quaternion),      "real quaternion",      k_real_quaternion_byte_swap_codes, "real_quaternion"},
		{sizeof(real_euler_angles2d),  "real euler angles 2d", k_real_point2d_byte_swap_codes,    "real_euler_angles2d"},
		{sizeof(real_euler_angles3d),  "real euler angles 3d", k_real_point3d_byte_swap_codes,    "real_euler_angles3d"},
		{sizeof(real_plane2d),         "real plane 2d",        k_real_point3d_byte_swap_codes,    "real_plane2d"},
		{sizeof(real_plane3d),         "real plane 3d",        k_real_plane3d_byte_swap_codes,    "real_plane3d"},
		{sizeof(real_rgb_color),       "real rgb color",       k_real_rgb_color_byte_swap_codes,  "real_rgb_color"},
		{sizeof(real_argb_color),      "real argb color",      k_real_argb_color_byte_swap_codes, "real_argb_color"},

		{sizeof(real_rgb_color),       "real hsv color",       k_real_rgb_color_byte_swap_codes,         /*"real_hsv_color"*/ },
		{sizeof(real_argb_color),      "real ahsv color",      k_real_argb_color_byte_swap_codes,         /*"real_ahsv_color"*/ },

		{sizeof(short_bounds),         "short integer bounds", k_point2d_byte_swap_codes,         "short_bounds"},
		{sizeof(angle_bounds),         "angle bounds",         k_real_point2d_byte_swap_codes,    "angle_bounds"},
		{sizeof(real_bounds),          "real bounds",          k_real_point2d_byte_swap_codes,    "real_bounds"},
		{sizeof(real_fraction_bounds), "fraction bounds",      k_real_point2d_byte_swap_codes,    "real_fraction_bounds"},

		{0x10,        "tag reference",        k_tag_reference_byte_swap_codes,   "tag_reference"}, //sizeof(tag_reference)
		{0xC,            "block",                k_tag_block_byte_swap_codes,       "tag_block"},
		{sizeof(short),                "short block index",    k_short_integer_byte_swap_codes,   "short"},
		{sizeof(long),                 "long block index",     k_long_integer_byte_swap_codes,    "long"},
		//sizeof(tag_data) but it's being a bitch so fuck it.
		{0xC,             "data",                 k_tag_data_byte_swap_codes,        "tag_data"},
		{0,                            "array start",          k_start_array_byte_swap_codes},
		{0,                            "array end",            k_end_array_byte_swap_codes},
		{0,                            "pad",                  k_pad_byte_swap_codes},
		{0,                            "skip",                 k_pad_byte_swap_codes},
		{0,                            "explanation",          k_explanation_byte_swap_codes},
		{0,                            "custom",               k_custom_byte_swap_codes},
		{0,                            "terminator X",         k_terminator_byte_swap_codes}
	};
	static_assert(::std::size(k_tag_field_definitions) == field_type::k_number_of_tag_field_types);
};

namespace Yelo {
	typedef void (__cdecl *proc_tag_data_byte_swap)(void *block_element, void *address, long size);

	struct tag_data_definition {
		const char *name;
		long_flags              flags;
		long                    maximum_size;
		proc_tag_data_byte_swap byte_swap_proc;
	};

	static_assert(sizeof(tag_data_definition) == 0x10);

	struct tag_reference_definition {
		unsigned long flags;
		tag           group_tag;
		tag           *group_tags;

		struct s_group_tag_iterator {
		private:
			tag *m_group_tags;

			bool IsEndHack() const { return m_group_tags == nullptr; }

		public:
			s_group_tag_iterator() : m_group_tags(nullptr) {} // for EndHack
			s_group_tag_iterator(const tag_reference_definition *definition) : m_group_tags(definition->group_tags) {}

			bool operator !=(const s_group_tag_iterator &other) const {
				if (other.IsEndHack()) {
					return (*m_group_tags) != NONE;
				}

				if (this->IsEndHack()) {
					return *other.m_group_tags != NONE;
				}

				return m_group_tags != other.m_group_tags;
			}

			s_group_tag_iterator &operator ++() {
				++m_group_tags;
				return *this;
			}

			tag &operator *() const {
				return *m_group_tags;
			}
		};

		s_group_tag_iterator begin() /*const*/
		{
			auto iter = s_group_tag_iterator(this);
			return iter;
		}

		static const s_group_tag_iterator end() /*const*/
		{
			return s_group_tag_iterator();
		}
	};

	STAT_ASSERT(tag_reference_definition, 0xC);

	// Postprocess a tag definition (eg, automate the creation of fields, etc)
	// Called once the tag has been fully loaded (header_block_definition's postprocess is called before this)
	typedef bool (__cdecl *proc_tag_group_postprocess)(datum_index tag_index, Enums::tag_postprocess_mode mode);

	struct tag_group {
		const char *name;
		unsigned long flags;
		tag           group_tag;
		tag           parent_group_tag;
		short         version;
		unsigned short : 16;
		proc_tag_group_postprocess postprocess_proc;
		tag_block_definition       *header_block_definition;
		tag                        child_group_tags[Yelo::Enums::k_maximum_children_per_tag];
		short                      child_count;
		unsigned short : 16;

		// TagGroups::s_tag_field_set_runtime_data *GetHeaderRuntimeInfo() const { return header_block_definition->GetRuntimeInfo(); }

		// tag_group* [] (ie, tag_group**) qsort procs
		static int __cdecl CompareByNameProc(void *, const tag_group *const *lhs, const tag_group *const *rhs) {
			return strcmp((*lhs)->name, (*rhs)->name);
		}

		static int __cdecl SearchByNameProc(void *, const char *key, const tag_group *const *group) {
			return strcmp(key, (*group)->name);
		}
	};

	STAT_ASSERT(tag_group, 0x60);


	// struct s_tag_instance : Memory::s_datum_base_aligned {
	// 	char        filename[Yelo::Enums::k_max_tag_name_length + 1];         // 0x4
	// 	tag         group_tag;            // 0x104
	// 	tag         parent_group_tags[2];   // 0x108
	// 	bool        is_verified;         // 0x110 was loaded with Flags::_tag_load_for_editor_bit
	// 	bool        is_read_only;         // 0x111
	// 	bool        is_orphan;            // 0x112
	// 	bool        is_reload;            // 0x113 true if this instance is the one used for another tag during tag_reload
	// 	datum_index reload_index;   // 0x114 index of the instance used to reload -this- tag's definition
	// 	uint        file_checksum;      // 0x118
	// 	tag_block<void>   root_block;      // 0x11C
	// }; static_assert(sizeof(s_tag_instance) == 0x128);

};

