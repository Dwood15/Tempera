#pragma once

#include "macros_generic.h"

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

	char *TryAndGetGroupName(tag group_tag, _Out_ long_string name);

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
};

