/**
 *	Project: Tempera
 *	File: main.cpp
 *	Copyright � 2018 Dwood
 *
 *	This file is part of Tempera.
 *
 * Tempera is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tempera is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tempera. If not, see <http://www.gnu.org/licenses/>.
 **/

#pragma once

#include "../headers/enums_generic.h"
#include "../ce_base_types.h"
#include "../gamestate/objects.h"

struct s_halo_pc_network {
	networked_datum Role;                                                //0x0
	bool : 8;                                            //0x4
	bool should_force_baseline_update;                            //0x5
	unsigned __int16 : 16;                              //0x6	//TODO: verify if this is padding or not
	signed long network_time;                           // 0x8
}; static_assert(sizeof(s_halo_pc_network) == 0xC, STATIC_ASSERT_FAIL);

