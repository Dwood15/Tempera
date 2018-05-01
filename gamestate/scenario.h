#pragma once

#include "../headers/macros_generic.h"

/*
	Project: haloforge
	File: objectcontroller.cpp
	Copyright � 20018 Dwood15

	This file is part of haloforge.

    haloforge is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haloforge is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.
*/

struct s_scenario_location {
	long             leaf_index;
	__int16          cluster_index;
	unsigned __int16 __unk_type0;
}; static_assert(sizeof(s_scenario_location) == 0x8, STATIC_ASSERT_FAIL);
