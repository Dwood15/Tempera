#pragma once

#include "render.h"
#include <engine_interface.h>
#include <cstring>
#include <locale>
#include "../CurrentEngine.h"


namespace render {
	/*
	 * 0x4CCF76
	 * - E8 75 EF FF FF
	 *
	 * 0x53BF45
	 * - E8 A6 FF F8 FF
	 *
	 * 0x53D018
	 * - E8 D3 EE F8 FF
	 *
	 * 0x53D0B8
	 * - E8 33 EE F8 FF
	 */
	int main_get_current_solo_level(char * level_string) {
		char v5[124];
		char test[127];


		memset(&v5, 0, 124u);
		strncpy(test, level_string, 127u);

		if (test) {
			char *v1 = test;
			char v2;
			do
			{
				*v1 = tolower((unsigned __int8)*v1);
				v2 = (v1++)[1];
			}
			while ( v2 );
		}
		if ( strstr(test, "a10") )
			return 0;
		if ( strstr(test, "a30") )
			return 1;
		if ( strstr(test, "a50") )
			return 2;
		if ( strstr(test, "b30") )
			return 3;
		if ( strstr(test, "b40") )
			return 4;
		if ( strstr(test, "c10") )
			return 5;
		if ( strstr(test, "c20") )
			return 6;
		if ( strstr(test, "c40") )
			return 7;
		if ( strstr(test, "d20") )
			return 8;
		return strstr(test, "d40") != 0 ? 9 : -1;
	}

};
