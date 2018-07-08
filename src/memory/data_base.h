#pragma once

#include "datum_index.h"

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_data_signature = 'd@t@',
			k_data_iterator_signature = 'iter',

			k_lruv_cache_signature = 'weee',

			k_memory_pool_signature = 'pool',
			k_block_header_signature = 'head',
			k_block_trailer_signature = 'tail',
		};
	};

	namespace Memory
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	for data structures which DO use the proceeding 16 bits after the 'salt' header.
		/// </summary>
		struct s_datum_base
		{
		private:
			datum_index::salt_t header; // salt for this datum
		public:
			datum_index::salt_t GetHeader() const	{ return header; }
			bool IsNull() const						{ return header == 0; }
		}; static_assert( sizeof(s_datum_base) == 2 );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	for data structures which DON'T make use of the proceeding 16 bits after the 'salt'
		/// 	header.
		/// </summary>
		struct s_datum_base_aligned : s_datum_base
		{
		private:
			unsigned short : 16;
		}; static_assert( sizeof(s_datum_base_aligned) == 4 );
	};
};
