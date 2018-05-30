#pragma once

#include "macros_generic.h"

//S_animation-state won't get synched.
struct s_animation_state {
	//TODO: Double check if these are signed..
	// (0 = Idle, 1 = Gesture, Turn Left = 2, Turn Right = 3, Move Front = 4, Move Back = 5, Move Left = 6, Move Right = 7, Stunned Front = 8,
	// Stunned Back = 9, Stunned Left = 10, Stunned Right = 11, Slide Front = 12, Slide Back = 13, Slide Left = 14, Slide Right = 15, Ready = 16,
	// Put Away = 17, Aim Still = 18, Aim Move = 19, Airborne = 20, Land Soft = 21, Land Hard = 22, ??? = 23, Airborne Dead = 24, Landing Dead = 25,
	// Seat Enter = 26, Seat Exit = 27, Custom Animation = 28, Impulse = 29, Melee = 30, Melee Airborne = 31, Melee Continuous = 32,
	// Grenade Toss = 33, Resurrect Front = 34, Resurrect Back = 35, Feeding = 36, Surprise Front = 37, Surprise Back = 38, Leap Start = 39,
	// Leap Airborne = 40, Leap Melee = 41, Unused AFAICT = 42, Berserk = 43)
	__int16 animation_index;
	__int16 frame_index;
}; static_assert(sizeof(s_animation_state) == 0x4);
