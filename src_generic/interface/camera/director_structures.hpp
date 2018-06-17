#pragma once
#include <precompile.h>

namespace Yelo
{
	namespace Enums
	{
		enum game_perspective : short
		{
			// Incomplete
			_game_perspective_first_person,
			_game_perspective_following_camera,

			_game_perspective
		};
	};

	namespace Camera
	{
		struct s_director_data
		{
			unsigned short : 16; // unknown field						0x0
			unsigned short : 16;									//	0x2
			unsigned long : 32; // unknown field						0x4
			void* camera_update_proc;				//	0x8
			PAD(1, 64);								//	0xC
			unsigned long : 32; // unknown field						0x4C
			unsigned char : 8; // unknown field						0x50
			bool inhibit_facing;					//	0x51
			bool inhibit_input;						//	0x52
			unsigned char : 8;									//	0x53
			short game_perspective;					//	0x54
			short  perspective;						//	0x56
			s_camera_command command;				//	0x58
			unsigned char : 8; // unknown field						0xC0
			unsigned char : 8; unsigned short : 16; 									//	0xC1
			unsigned long : 32; // unknown field						0xC4
			PAD(2, sizeof(real_vector3d) * 4);		//	0xC8
		}; static_assert(sizeof(s_director_data) == 0xF8);

		s_director_data*				GlobalDirector();
	};
};
