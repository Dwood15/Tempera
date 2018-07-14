#pragma once

#include "../../cseries/base.h"

namespace Yelo
{
	namespace AI
	{
		struct s_action_flee_state : TStructImpl(0x30)
		{
			//TStructGetPtrImpl(unsigned short?,			, 0x0);
			//TStructGetPtrImpl(short,				flee_stationary_ticks, 0x2);
			//TStructGetPtrImpl(bool,				, 0x4);
			//TStructGetPtrImpl(bool,				, 0x5);
			//TStructGetPtrImpl(bool,				find_new_flee_position, 0x6);
			// unsigned char : 8
			//TStructGetPtrImpl(short,				flee_firing_position_index, 0x8);

			//TStructGetPtrImpl(bool,				, 0xA);

			//TStructGetPtrImpl(short,				, 0xC);
			//TStructGetPtrImpl(bool,				unable_to_flee, 0xE);
			//TStructGetPtrImpl(bool,				done_fleeing, 0xF);
			//TStructGetPtrImpl(bool,				, 0x10); // true if they're blindly fleeing and speaking
			//unsigned char : 8; unsigned short : 16;?
			//TStructGetPtrImpl(long,				, 0x14); // game time, last-time-of-speaking? will only update every 60 ticks

			//TStructGetPtrImpl(datum_index,				, 0x1C); // prop
		};
	};
};
