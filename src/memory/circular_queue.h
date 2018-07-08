#pragma once

#include "../cseries/base.h"

namespace Yelo {
	namespace Enums {
		enum {
			k_circular_queue_signature = 'circ',
		};
	};

	namespace Memory {
		struct s_circular_queue {
			const char * name;
			tag     signature;
			long   read_offset;
			long   write_offset;
			long   buffer_size;
			void *buffer;
		};
	};
};
