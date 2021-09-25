#pragma once

#include <addlog.h>
#include "forge.h"
#include "math/colors.h"
#include "RuntimeManager.h"

static short last_respawn_count       = 0x0;
static short last_spawn_count         = 0x0;
static short last_render_window_count = 0x0;

//Live-updating number of players game should spawn.
static short *to_respawn_count = (short *) 0x6B4802;

//Number of players to spawn - set on map initialization/unloading.
static short *spawn_count         = (short *) 0x624A9C;

void UpdateGlobals() {
	if (last_respawn_count != *to_respawn_count) {
		Print("Number of people to respawn from: %d to: %d\n", last_respawn_count, *to_respawn_count);
		last_respawn_count = *to_respawn_count;
	}

	if (last_spawn_count != *spawn_count) {
		Print("Number of people to respawn from: %d to: %d\n", last_spawn_count, *spawn_count);
		last_spawn_count = *spawn_count;
	}

//  render_window_count is not currently provided
//	if (last_render_window_count != CurrentRuntime->) {
//		Print("Updated number players to spawn from: %d to: %d\n", last_render_window_count, *render_window_count);
//		last_render_window_count = *render_window_count;
//	}
}

void HandleMainMenuOptions() {
	if (GetAsyncKeyState(VK_F1) & 1) {
		//Player spawn count set to 2.
		*(short *) 0x624A9C = (short) 0x2;
		CurrentRuntime->ConsoleText(hGreen, "Number players to spawn in next sp map: +1!");
	}
}

void PrintHelp() {
	const char *f1     = "F1    -> (ui only!)Set # players to spawn in next map to 2.\n";
	const char *f2     = "F2    -> Dumps Player Globals data to log. Prints to window when not in ui.map\n";
	const char *shift  = "SHIFT -> Select\\Deselect highlighted object.\n";
	const char *f5     = "F5    -> Log object data that player is holding to file.\n";
	const char *akUP   = "AK_UP -> Moves held object away incrementally.\n";
	const char *akDOWN = "AK_DN -> Moves held object toward, incrementally.\n\t ->Arrow keys are for use with RMBTN\n";
	const char *lmBTN  = "LMBTN -> Hold Object - Hard to describe.\n";
	const char *rmBTN  = "RMBTN -> Hold Object - Same as LMBTN.\n\t ->Use these in map if you have selected an object.\n";
	const char *f11    = "F11   -> This Help Text.\n";

	Print("\n\n/******************************************************\\\n");
	Print("  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n", shift, f1, f2, f5, akUP, akDOWN, lmBTN, rmBTN, f11);
	Print("\\*****************************************************/\n");
}

//TODO: Launch thread later on in the load cycle
[[noreturn]] int forge::MainLoop() {
	if (CurrentEngine->IsHek()) {
		PrintLn("Forge doesn't support anything but Halo PC 1.10 right now. Working on expanding horizons.");
		__asm retn
	}

	Sleep(600);
	//automatically spawn the maximum number of local players at the beginning of the game.

	*(short *) 0x624A9C = (short)MAX_PLAYER_COUNT_LOCAL;
	cd3d.hkD3DHook(nullptr);

	CurrentRuntime->ConsoleText(hGreen, "Number players to spawn in next sp map: 3!");
	PrintHelp();

	while (1) {
		Sleep(20);
		UpdateGlobals();

		auto inMainMenu = CurrentRuntime->AreWeInMainMenu();

		if (inMainMenu) {
			HandleMainMenuOptions();
		}

		if (GetAsyncKeyState(VK_F2) & 1) {
			// if(core) {
			//TODO: Get players_global data dynamically, ie via core
			// 	players_global_data->DumpData(true, core);
			// }
		}

		 if (GetAsyncKeyState(VK_F11) & 1) {
		 	PrintHelp();
		 	continue;
		 }

		 if (inMainMenu) {
		 	Sleep(45);
		 	continue;
		 }

		// if (GetAsyncKeyState(VK_F5) & 1) {
		// 	core->ObjectControl->LogInfo();
		// }

		 if ((CurrentRuntime->IsPlayerSpawned(1)) && GetAsyncKeyState(VK_F7) & 1) {
		 	datum_index plyr_datum = CurrentRuntime->GetPlayerObjectIdent(1);

		 	if (plyr_datum.index > -1) {
		 		object_data *objd = CurrentRuntime->GetGenericObject(plyr_datum.index);

		 		objd->Velocity.z += .25f;
		 		objd->Velocity.y  = 0;

		 		CurrentRuntime->ConsoleText(hBlue, "Incremented The player 2 object's velocity!");
		 	}
		 } else if (CurrentRuntime->IsPlayerSpawned(0) && GetAsyncKeyState(VK_F8) & 1) {
			 datum_index plyr_datum = CurrentRuntime->GetPlayerObjectIdent(0);
		 	if (plyr_datum.index > -1) {
		 		//object_header * objh = core->GetObjectHeader(plyr_datum.index);
		 		object_data *objd = CurrentRuntime->GetGenericObject(plyr_datum.index);

		 		objd->Velocity.x += .25;
		 		objd->Velocity.y  = 0;

				CurrentRuntime->ConsoleText(hBlue, "Increased Player 1 velocity! %.3f", objd->Velocity.x);
		 	}
		 }

		// else if (GetAsyncKeyState(VK_UP)) {
		// 	core->ObjectControl->IncreaseHoldDistance();         // Object MOVE away.
		//
		// } else if (GetAsyncKeyState(VK_DOWN)) {
		// 	core->ObjectControl->DecreaseHoldDistance();         // Object MOVE closer
		//
		// } else if (GetAsyncKeyState(VK_SHIFT) & 1) {
		// 	// SHIFT to lock on object.
		// 	core->ObjectControl->UpdateSelection();
		//
		// } else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { // XY plane
		// 	//left mouse button
		// 	core->ObjectControl->MoveObjXY();
		// } else if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {    // STRAIGHT in front of camera plane.
		// 	//Right mouse button
		// 	core->ObjectControl->UpdateHeldObject();
		// } else if (!(GetAsyncKeyState(VK_RBUTTON))) {
		// 	core->ObjectControl->DropHeldObject();
		// }

		//	TODO: Scrolling to set selected item's distance from player.
		// 	MSG msg;
		//Isn't doing anything?
		// LPMSG msg = NULL;
		// if (PeekMessage(msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE)) {
		// 	if (msg->message == WM_MOUSEWHEEL) {
		// 		short zDelta = GET_WHEEL_DELTA_WPARAM(msg->wParam);
		// 		/*
		// 		The return value is the high-order word representing the wheel-delta value.
		// 		It indicates the distance that the wheel is rotated, expressed in multiples
		// 		or divisions of WHEEL_DELTA, which is 120.
		//
		// 		A positive value == the wheel was rotated away from the user;.
		// 		A negative value indicates that the wheel was rotated toward the user.
		// 		*/
		// 		if (zDelta < 0) {
		// 			core->ObjectControl->DecreaseHoldDistance(zDelta);
		// 		} else {
		// 			core->ObjectControl->IncreaseHoldDistance(zDelta);
		// 		}
		// 		Print(false, "MouseWheel dist: %d", zDelta);
		// 	} else {
		// 		Print(true, "other: %d", msg->message);
		// 	}
		// }
	}
}

//Probably the wrong version
//TODO: update from PC 1.07/08(?) to CE 1.10
// Local .blam storage ( if you are host )
constexpr unsigned long SERVER_BLAM_ADDRESS = 0x0087A620;
// .blam storage ( if you are NOT host )
constexpr unsigned long LOCAL_BLAM_ADDRESS  = 0x0087A6C0;

CBlam::CBlam() {
	server_blam = (_blam_ *) SERVER_BLAM_ADDRESS;
	local_blam  = (_blam_ *) LOCAL_BLAM_ADDRESS;
}
void CBlam::SetGameTypeName(_blam_ *_blam, wchar_t *wName) { memcpy(_blam->GameTypeName, wName, 24); }

wchar_t *CBlam::GetGameTypeName(_blam_ *_blam) { return _blam->GameTypeName; }

void CBlam::SetGameType(_blam_ *_blam, long dwType) { _blam->GameType = dwType; }

void CBlam::SetTeamPlay(_blam_ *_blam, long dwNum) { _blam->TeamPlay = dwNum; }

long CBlam::GetGameType(_blam_ *_blam) { return _blam->GameType; }

long CBlam::GetTeamPlay(_blam_ *_blam) { return _blam->TeamPlay; }

void CBlam::SetBallIndicator(_blam_ *_blam, bool bNum) { _blam->BallIndicator = bNum; }

void CBlam::SetFriendsOnRadar(_blam_ *_blam, bool bNum) { _blam->FriendsOnRadar = bNum; }

void CBlam::SetStartEquipment(_blam_ *_blam, bool bNum) { _blam->StartEquipment = bNum; }

void CBlam::SetInvisible(_blam_ *_blam, bool bNum) { _blam->Invisible = bNum; }

void CBlam::SetShields(_blam_ *_blam, bool bNum) { _blam->Shields = bNum; }

void CBlam::SetInfiniteGrenades(_blam_ *_blam, bool bNum) { _blam->InfiniteGrenades = bNum; }

void CBlam::SetFriendIndicators(_blam_ *_blam, bool bNum) { _blam->FriendIndicators = bNum; }

void CBlam::SetPlayersOnRadar(_blam_ *_blam, bool bNum) { _blam->PlayersOnRadar = bNum; }

bool CBlam::GetBallIndicator(_blam_ *_blam) { return _blam->BallIndicator; }

bool CBlam::GetFriendsOnRadar(_blam_ *_blam) { return _blam->FriendsOnRadar; }

bool CBlam::GetStartEquipment(_blam_ *_blam) { return _blam->StartEquipment; }

bool CBlam::GetInvisible(_blam_ *_blam) { return _blam->Invisible; }

bool CBlam::GetShields(_blam_ *_blam) { return _blam->Shields; }

bool CBlam::GetInfiniteGrenades(_blam_ *_blam) { return _blam->InfiniteGrenades; }

bool CBlam::GetFriendIndicators(_blam_ *_blam) { return _blam->FriendIndicators; }

bool CBlam::GetPlayersOnRadar(_blam_ *_blam) { return _blam->PlayersOnRadar; }
