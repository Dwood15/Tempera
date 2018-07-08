/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_IS_EDITOR

// This is mostly just for easing the hard coding of definitions in TagFieldSetReplacements.cpp

#define TAG_FIELD_MARKUP_IS_ADVANCED	"!"
#define TAG_FIELD_MARKUP_IS_READONLY	"*"
#define TAG_FIELD_MARKUP_IS_BLOCK_NAME	"^"
#define TAG_FIELD_MARKUP_PREFIX_UNITS	":"
#define TAG_FIELD_MARKUP_PREFIX_HELP	"#"

#define TAG_FIELD_ENTRY(type, name, ...)						\
	{Yelo::Enums::type, name, __VA_ARGS__}
#define TAG_FIELD_ENTRY_PAD(size)								\
	TAG_FIELD_ENTRY(_field_pad, nullptr, CAST_PTR(void*, size))
#define TAG_FIELD_ENTRY_SKIP(size)								\
	TAG_FIELD_ENTRY(_field_pad, nullptr, CAST_PTR(void*, size))
#define TAG_FIELD_ENTRY_END()									\
	TAG_FIELD_ENTRY(_field_terminator, nullptr)

#define TAG_GROUP_STRING_TABLE_DEFINE(name, count, ...)			\
	static Yelo::const char * name##_strings[] = { __VA_ARGS__ };	\
	static_assert(std::size(name##_strings)==count,				\
		"too few strings defined for " #name);					\
	static Yelo::string_list name = {							\
		std::size(name##_strings),								\
		name##_strings											\
	}

#define TAG_GROUP_REFERENCE_GET(name)										\
	name##_reference
#define TAG_GROUP_REFERENCE_DEFINE(name, flags, group_tag)					\
	static Yelo::tag_reference_definition TAG_GROUP_REFERENCE_GET(name) = {	\
		flags,																\
		group_tag,															\
		nullptr																\
	}
#define TAG_GROUP_REFERENCES_DEFINE(name, flags, ...)						\
	static Yelo::const char * TAG_GROUP_REFERENCE_GET(name)##_tags[] = {			\
		__VA_ARGS__,														\
		NONE,																\
	};																		\
	static_assert(std::size(TAG_GROUP_REFERENCE_GET(name)) > 1,				\
		"no tags defined for reference " #name);							\
	static Yelo::tag_reference_definition TAG_GROUP_REFERENCE_GET(name) = {	\
		flags,																\
		NONE,																\
		TAG_GROUP_REFERENCE_GET(name)##_tags								\
	}

#define TAG_GROUP_BLOCK_FIELDS_GET(name)								\
	name##_fields
#define TAG_GROUP_BLOCK_FIELDS_DEFINE(name)								\
	static Yelo::tag_field TAG_GROUP_BLOCK_FIELDS_GET(name)[]

#define TAG_GROUP_BLOCK_GET(name)										\
	name##_block
#define TAG_GROUP_BLOCK_DEFINE(name, flags, max_count, size, ...)		\
	static Yelo::tag_block_definition TAG_GROUP_BLOCK_GET(name) = {		\
		BOOST_PP_STRINGIZE(TAG_GROUP_BLOCK_GET(name)),					\
		flags, max_count, size,											\
		nullptr,														\
		TAG_GROUP_BLOCK_FIELDS_GET(name),								\
		nullptr,														\
		__VA_ARGS__,													\
	}

#endif
