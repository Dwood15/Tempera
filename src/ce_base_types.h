/*
	Project: tempera
   You should have received a copy of the GNU General Public License
   along with tempera. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <macros_generic.h>
#include "memory/datum_index.h"

typedef short                     int16;
typedef unsigned int              uintptr_t;
typedef unsigned long 				 tag;
typedef char                      tag_string[32];
typedef const char                *cstring;
typedef char * tag_reference_name_reference;

/// <summary>	The integral type used to represent game ticks. </summary>
typedef long game_ticks_t;
/// <summary>	The integral type used to represent (relative) game timers. </summary>
/// <remarks>	Steps are in ticks. Generally used for countdowns </remarks>
typedef short game_time_t;


namespace Yelo::Enums
{
	enum {
		k_maximum_number_of_local_players = 4,
		k_multiplayer_maximum_players = 16,

		// for team_data (which has a maximum of 16 datums) and such.
		// This is 1 in Stubbs, I'm positive team_data uses k_multiplayer_maximum_players.
			k_multiplayer_maximum_teams = k_multiplayer_maximum_players,
	};
};



