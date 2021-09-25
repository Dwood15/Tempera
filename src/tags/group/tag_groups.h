#pragma once

#include "macros_generic.h"
#include "../../memory/datum_index.h"
#include "../../memory/data_iterator.h"

namespace Yelo::TagGroups {
	// Patches stock tag_groups with new fields where they once had useless padding
	// Called before group definitions have been verified (but after group tags have) and group parents are built
	// We do this to ensure we don't fuck anything up, but also because a parent group may get modified
	// Defined in CheApe's TagFieldSetReplacements.cpp
	// TODO: perhaps instead we should run this -after- initial verification, then in true debug builds
	// run group verification again after this executes. Or we could perform verification in the fieldset replacement code
	void InitializeFieldSetReplacements();

	// Patches stock tag groups for various *fixes* (eg, to correct behavior), not additions or the like
	// Called after the group definitions have been verified
	// Defined in CheApe's TagGroups.cpp
	void InitializeFixes();

	char *TryAndGetGroupName(tag group_tag, long_string &name);

	// Convenience function to handle deleting all of the data in tag_data field.
	// Use [terminator_size] for tag_data which HAS to have a specific amount of
	// bytes no matter what. IE, text data requires 1 or 2 bytes (ascii or unicode)
	// for the null terminator.
	// void tag_data_delete(Yelo::tag_data *data, size_t terminator_size = 0) {
	// 	blam::tag_data_resize(data, terminator_size);
	// }

	// just an endian swap
	static void TagSwap(tag &x) {
		x = (x >> 24) |
			 ((x << 8) & 0x00FF0000) |
			 ((x >> 8) & 0x0000FF00) |
			 (x << 24);
	}

	static tag string_to_group_tag(const char *name) {
		const uint _group = *((const uint *) name);
		return (tag) ((_group >> 24) | ((_group >> 8) & 0xFF00) | (((_group << 8) & 0xFF0000) | ((_group << 24) & 0xFF000000)));
	}

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpadded"
#endif

#pragma pack(push, 1)

	// Union hack to use a group tag as a string
	union u_group_tag_to_string {
		struct {
			tag group;
			unsigned char : 8; // null terminator
		};

		char str[sizeof(tag) + 1];

		u_group_tag_to_string &Terminate() {
			str[4] = '\0';
			return *this;
		}

		u_group_tag_to_string &TagSwap() {
			TagGroups::TagSwap(group);
			return *this;
		}

		const char *ToString() {
			return Terminate().TagSwap().str;
		}
	};
	STAT_ASSERT(u_group_tag_to_string, 0x5);
	STAT_ASSERT(u_group_tag_to_string, sizeof(tag) + 0x1);

#pragma pack(pop)

	static const char* group_tag_to_string(tag rhs) {
		u_group_tag_to_string gtts;
		gtts.group = rhs;
		return gtts.ToString();
	}

#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wpadded"
#endif

};

namespace Yelo::blam {
	void tag_groups_initialize();
	void tag_groups_dispose();
	void tag_groups_initialize_for_new_map();
	void tag_groups_dispose_from_old_map();
	void tag_groups_dump_memory();
	uint  tag_groups_checksum();


	datum_index find_tag_instance(tag group_tag, cstring name);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialize a tag instance iterator for the given group tag </summary>
	///
	/// <param name="iter">			   	[out] The iterator to initialize </param>
	/// <param name="group_tag_filter">
	/// 	(Optional) the group tag to filter results by. Use [NONE] for [group_tag_filter] to
	/// 	iterate all tag groups.
	/// </param>
	void tag_iterator_new(TagGroups::s_tag_iterator& iter, const tag group_tag_filter = NONE);
	template<typename T> inline
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialize a tag instance iterator for the given group tag </summary>
	///
	/// <param name="iter">	[out] The iterator to initialize </param>
	void tag_iterator_new(TagGroups::s_tag_iterator& iter)
	{
		tag_iterator_new(iter, T::k_group_tag);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Increment a tags instance iterator to the next instance </summary>
	///
	/// <param name="iter">	[in,out] The iterator to increment </param>
	///
	/// <returns>	Returns the next datum's index or [datum_index::null] when finished iterating </returns>
	datum_index tag_iterator_next(TagGroups::s_tag_iterator& iter);
}
