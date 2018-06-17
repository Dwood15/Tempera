//No addresses here!

#include <precompile.h>
#include "headers/blam.h"

CBlam::CBlam()
{
	server_blam = (blam*)SERVER_BLAM_ADDRESS;
	local_blam = (blam*)LOCAL_BLAM_ADDRESS;
}
CBlam::~CBlam()
{
}

void CBlam::SetGameTypeName(blam* _blam, wchar_t *wName	){memcpy(	_blam->GameTypeName, wName, 24);}
wchar_t *CBlam::GetGameTypeName(blam* _blam){return _blam->GameTypeName;}

void CBlam::SetGameType(blam* _blam, long dwType){_blam->GameType = dwType;}
void CBlam::SetTeamPlay(blam* _blam, long dwNum){_blam->TeamPlay = dwNum;}
long CBlam::GetGameType(blam* _blam){return _blam->GameType;}
long CBlam::GetTeamPlay(blam* _blam){return _blam->TeamPlay;}

void CBlam::SetBallIndicator(blam* _blam, bool bNum){_blam->BallIndicator = bNum;}
void CBlam::SetFriendsOnRadar(blam* _blam, bool bNum){ _blam->FriendsOnRadar = bNum;}
void CBlam::SetStartEquipment(blam* _blam, bool bNum){ _blam->StartEquipment = bNum;}
void CBlam::SetInvisible(blam* _blam, bool bNum){ _blam->Invisible = bNum;}
void CBlam::SetShields(blam* _blam, bool bNum){ _blam->Shields = bNum;}
void CBlam::SetInfiniteGrenades(blam* _blam, bool bNum){ _blam->InfiniteGrenades = bNum;}
void CBlam::SetFriendIndicators(blam* _blam, bool bNum){ _blam->FriendIndicators = bNum;}
void CBlam::SetPlayersOnRadar(blam* _blam, bool bNum){ _blam->PlayersOnRadar = bNum;}

bool CBlam::GetBallIndicator(blam* _blam){ return _blam->BallIndicator;}
bool CBlam::GetFriendsOnRadar(blam* _blam){ return _blam->FriendsOnRadar;}
bool CBlam::GetStartEquipment(blam* _blam){ return _blam->StartEquipment;}
bool CBlam::GetInvisible(blam* _blam){ return _blam->Invisible;}
bool CBlam::GetShields(blam* _blam){ return _blam->Shields;}
bool CBlam::GetInfiniteGrenades(blam* _blam){ return _blam->InfiniteGrenades;}
bool CBlam::GetFriendIndicators(blam* _blam){ return _blam->FriendIndicators;}
bool CBlam::GetPlayersOnRadar(blam* _blam){return _blam->PlayersOnRadar;}
