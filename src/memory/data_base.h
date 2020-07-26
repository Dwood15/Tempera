#pragma once
#include <macros_generic.h>
#include "datum_index.h"

namespace Yelo::Memory {
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	for data structures which DO use the proceeding 16 bits after the 'salt' header.
	/// </summary>
	struct s_datum_base {
	private:
		datum_index::salt_t header; // salt for this datum
	public:
		datum_index::salt_t GetHeader() const { return header; }

		bool IsNull() const { return header == 0; }
	};

	STAT_ASSERT(s_datum_base, 2);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	for data structures which DON'T make use of the proceeding 16 bits after the 'salt'
	/// 	header.
	/// </summary>
	struct s_datum_base_aligned : s_datum_base {
	private:
		unsigned short : 16;
	};
	STAT_ASSERT(s_datum_base_aligned, 4);
};
