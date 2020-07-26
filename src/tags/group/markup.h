#pragma once

// [arg2] = documentation

// [arg2] = documentation
#define TAG_FLAG8(name, ...)            byte name##_bit : 1

// [arg2] = documentation
#define TAG_FLAG(name, ...)               unsigned long name##_bit : 1

// [arg2] = enum type
// [arg3] = documentation
#define TAG_ENUM8(name, ...)            byte_enum name

// [arg2] = enum type
// [arg3] = documentation

// [arg2] = enum type
// [arg3] = documentation
#define TAG_ENUM32(name, ...)            long name

#define TAG_BLOCK_INDEX(block_type, name, ...)   block_type::block_index_t name

// Used for blocks which need to allow non-const access
//#d//efine TAG//_TBLOCK(name, block_type)   Yelo::TagBlock<const block_type> name
//#d/efine T/AG_PAD(type, count)            type : 8 * sizeof(type) * count
// [arg2] = units
// [arg3] = documentation

#define TAG_ARRAY(type, name, count)      type name[count]

//#d/efine T/AG_FIELD(type, name, ...) type name
// #define TAG_PAD(name, type, count)            type name[count]

#define TAG_BLOCK_GET_ELEMENT(block_ptr, index, type)  &(block_ptr->Elements<type>()[index])


#define TAG_FIELD(type, name, ...)			type name
#define TAG_FLAG16(name, ...)				unsigned short name##_bit : 1
