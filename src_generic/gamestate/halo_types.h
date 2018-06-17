/*
	Project: tempera
	File: core.cpp
	Copyright � 2015 Kornman00
 	Copyright 	 2018 Dwood

	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
////////////////////////////////////////
// Generic Halo Types
////////////////////////////////////////
#include <d3dx9math.h>
#include "../headers/colors.h"

struct bone {
	float unknown[10];
	vect3 World;
};

struct ident {
	short index;
	short id;
};
