#pragma once
#include "../cseries/base.h"

namespace Yelo::Cache {
	struct s_cache_header_base {
		enum {
			k_header_signature = 'head',
			k_footer_signature = 'foot',
		};
	};
};

namespace Yelo::Cache {
	struct s_cache_header_yelo_base {
		enum {
			k_signature = 'yelo',
		};

		tag   signature;
		short version;
	};
};
