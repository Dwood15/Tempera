#pragma once
namespace Yelo {
	namespace Enums {
		enum {
			k_uuid_buffer_size = 16,

			k_kilo = 1024,
			k_mega = k_kilo * k_kilo,
		};
	};

	namespace Flags {
		enum {
			k_alignment_8bit,
			k_alignment_16bit,
			k_alignment_32bit,
			k_alignment_64bit,
			k_alignment_128bit,
			k_alignment_256bit,
			k_alignment_512bit,
			k_alignment_1024bit,
			k_alignment_2048bit,
			k_alignment_4096bit,
		};
	};
};
