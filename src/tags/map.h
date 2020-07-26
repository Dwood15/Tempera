/*
	Project: haloforge
	File: map.h
	Copyright � 2009 SilentK, Abyll

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

#pragma once

#include "tags.h"

//// 2 Map Headers are loaded into static memory. 2 differences -
// // few bytes before and after the header ( Index Header pointer and bps pointer? )
// // Either of these should work though
// #define MAP_HEADER_ADDRESS 0x643044
// //0x006A807C 108 - PC
// #define MapHeaderAltAddy	0x006AAB80
// #define TAG_INDEX_HEADER_ADDRESS 0x40440000


// **** MAP HEADER STUFF ****

//class CMapHeader // HALO TRIAL type.
//{
//public:
//	WORD Unknown1;
//	WORD Version; // 5 = Xbox, 6 = Trial, 7 = PC, 609 = CE
//	BYTE Unknown2[700];
//	char Header[4]; // 'head''Ehed'
//	DWORD TagIndexMetaLength;
//	char BuildDate[32]; // Year.Month.Day.Build - I guess they use this to make sure that a certain build will only open that build's map files, because this string is in the app too
//	BYTE Unknown3[672];
//	long MapType; // 0 = singleplayer, 1 = multiplayer, 2 = ui - this also determines the size of the cache file. UI = 35MB, multiplayer = 47MB, and singleplayer = 270MB
//	char MapName[32];
//	BYTE Unknown4[60];
//	unsigned long DecompLen; // Actual len of decompressed data. Halo sticks garbage on the end so that the file is one of several fixed sizes (35, etc).
//	void *TagIndexOffset;
//	char Footer[4]; // 'foot' 'Gfot'
//};

struct map_header // HALO PC type.
{
	char head[4];			// 'deah' translates as head
	int Version;			// 5 = Xbox, 6 = Trial, 7 = PC, 609 = CE
	int DecompLen;			// Actual len of decompressed data. Halo sticks garbage on the end so that the file is one of several fixed sizes (35, etc).
	int Unknown1;
	int TagIndexOffset;
	int TagIndexMetaLength;
	int Zeros0[2];			// both always 0x0
	char MapName[32];
	char BuildDate[32];		// Year.Month.Day.Build - I guess they use this to make sure that a certain build will only open that build's map files, because this string is in the app too
	int MapType;			// 0 = singleplayer, 1 = multiplayer, 2 = ui - this also determines the size of the cache file. UI = 35MB, multiplayer = 47MB, and singleplayer = 270MB
	int Unknown4;
	int Zeros1[485];
	char foot[4];			// 'toof' translates to foot
};
