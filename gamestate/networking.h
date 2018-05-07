/*
	Project: tempera
	File: networking.h
	Copyright � 2015 Kornman00
 	Copyright 	 2018 Dwood

	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "../headers/enums_generic.h"
#include "../ce_base_types.h"
#include "../gamestate/objects.h"

struct s_halo_pc_network {
	networked_datum Role;                                //0x0, or 0x4 in context of the object_data structure
	bool : 8;                                            //0x4 or 0x8
	bool should_force_baseline_update;                   //0x5  or 0x9
	unsigned __int16 : 16;                              //0x6	or 0xA //TODO: verify if this is padding or not
	signed long network_time;                           // 0x8 or 0xC
}; static_assert(sizeof(s_halo_pc_network) == 0xC, STATIC_ASSERT_FAIL);

