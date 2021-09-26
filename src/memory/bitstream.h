/* See GPLv3 Or Later GPL licenses for license information */
#pragma once

namespace Yelo {
	namespace Enums {
		enum bitstream_mode : signed long {
			_bitstream_mode_write,
			_bitstream_mode_read,
		};
	};

	namespace Memory {
		struct s_bitstream {
			Enums::bitstream_mode mode;
			void *buffer;      // 0x4
			void *first_bit;   // 0x8 first bit where we can write to
			void *byte_number;   // 0xC byte index in the buffer
			void *bit_in_byte;   // 0x10 bitNum in torque
			void *last_bit;      // 0x14 last bit where we can write to
			long size;         // 0x18 size of buffer
		}; static_assert(sizeof(s_bitstream) == 0x1C);
	};
};
