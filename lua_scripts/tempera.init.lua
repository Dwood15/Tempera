--[[
	    Tempera Semi-Private Alpha
		  Lua API Documentation
			Updated July 26th, 2020
		        Dwood15/Zoru (Dason Woodhouse)

Lua scripting engine was added to Tempera by request to allow Sapien-AND-Halo CE platform targeting.

Hopefully you, the mapmaker or mod developer (or end-user) will find it useful.

	A little example for scripters using Tempera.
	Tempera has been going through quite a refactoring, so some of this will change.

	-- July 4th
	- XInput working!
	- Cleaned up, simplifying lua file.
	- Lua errors are accurate and descriptive!

	--Update 2 for June 30th
	- Fixed some bugs and made console output less obnoxious.
	- Added some better debugging messages.
	- Added a little platform detection.
--]]

-- PS: See the bottom of the document for more full information on
--The available LUA API.


DebugPrint(true, "\n\tCALLED A C FUNCTION FROM LUA\n")
--DebugPrint - For logging to file + Console.
--Takes Two parameters (Boolean) and a string.
--Always prints to debug file. Requires a boolean indicating whether or not to write to console.
--This may change slightly in the future, depending on user needs/wants.
--Considering changing this function into three and removing the boolean param.
--"ExternPrint", "DebugPrint", "LogPrint"

currentPlayer = 0
local maxTime = 8
local fifth = 0
local setPlayerVelocity = false
local SetPlayerLook_X = false
local nextX = -1.5
local nextY = 1.5

local foundPad = -1

--XINPUT BUTTONS YO
XINPUT_GAMEPAD_DPAD_UP          = 0x00000001
XINPUT_GAMEPAD_DPAD_DOWN        = 0x00000002
XINPUT_GAMEPAD_DPAD_LEFT        = 0x00000004
XINPUT_GAMEPAD_DPAD_RIGHT       = 0x00000008
XINPUT_GAMEPAD_START            = 0x00000010
XINPUT_GAMEPAD_BACK             = 0x00000020

XINPUT_GAMEPAD_LEFT_THUMB       = 0x00000040
XINPUT_GAMEPAD_RIGHT_THUMB      = 0x00000080
XINPUT_GAMEPAD_LEFT_SHOULDER    = 0x0100
XINPUT_GAMEPAD_RIGHT_SHOULDER   = 0x0200

XINPUT_GAMEPAD_A                = 0x1000
XINPUT_GAMEPAD_B                = 0x2000
XINPUT_GAMEPAD_X                = 0x4000
XINPUT_GAMEPAD_Y                = 0x8000
--All thumbs for XInput are: -32768 and 32767

TRIGGERMIN = 0
TRIGGERMAX = 255

--THUMBSTICK DEADZONES NOT YET IMPLEMENTED
THUMBCENTER = 0
THUMBMIN = -32768
THUMBMAX = 32767

--A little helper to keep things readable.
function IsButtonPressed(buttons, test)
	return (buttons & test) ~= 0
end

--The total number of players this version of tempera can currently support.
-- For the time being, consider not changing this.
MaxPlayers = 1

function NextPlayerIndex()
	if currentPlayer > MaxPlayers then
		currentPlayer = 0
	end
	--Don't bother with currentPlayer shit until later. :)
	--currentPlayer = currentPlayer + 1
end

--player_control_state -> the object that represents the player's controller
--[[
    control_flags -- Will provide more documentation later.
    throttle.x - strafing
    throttle.y - forward back
    desired_weapon_index
    desired_grenade_index
    primary_trigger
--]]
--player_index

local firstPlayerUpdateCalled = false
function PlayerUpdate(player_control_state, player_index)
	if firstPlayerUpdateCalled == false then
		DebugPrint(true, "First Player update, ever!")
	end

	local currentGamePad = 0
	--TODO: Player to Controller mapping...
	while currentGamePad < 4 do
		if firstPlayerUpdateCalled == false then
			DebugPrint(true, string.format("--[%d] player update controller update", currentGamePad))
		end
		--[[
            the controller object:
                "dwPacketNumber" -- compare this with the last run. if this is the same as the last run, then the controller state is the exact same as the last state.
                "buttons"
                "leftTrigger"
                "rightTrigger"
                --Left Thumbstick x, y axis
                "thumbLX"
                "thumbLY"
                --Right Thumbstick x, y axis
                "thumbRX"
                "thumbRY"
        --]]

		if firstPlayerUpdateCalled == false then
			DebugPrint(true, "Retrieving ControllerState")
		end

		--Controlleer is -1 if not found, and result is 0 if successful.
		-- Will be providing helpers for reading this more fully.
		controller, result = GetControllerState(currentGamePad)

		if firstPlayerUpdateCalled == false then
			DebugPrint(true, "ControllerState retrieved")
		end

		if controller ~= -1 and result == 0 then
			if IsButtonPressed(controller.buttons, XINPUT_GAMEPAD_Y) then
				DebugPrint(true, "Controller input pressed, jumping the player")
				MakePlayerJump(player_index)
			end

			--TODO: Check appropriate thumb stick for input, grenade throw, etc etc.
			--SetPlayerXVelocity(0, 0.50)

			--Hard to figure out how to override the biped's desired look points. :X

			--MakePlayerJump             -- Just player index.
		end

		if firstPlayerUpdateCalled == false then
			DebugPrint(true, "ControllerState retrieved")
		end

		currentGamePad = currentGamePad + 1
	end

	firstPlayerUpdateCalled = true

	--player_control_state MUST be returned to have input update.
	return player_control_state
end


local onTickNextPlayerIndexPrinted = false
function OnTick(ticks_til_frame, time_since_map_started)
	-- too much log spam
	--if onTickNextPlayerIndexPrinted == false then
	--	DebugPrint(true, "Checking if Core Initialized!")
	--end

	if IsCoreInitialized() == false then
		return
	elseif onTickNextPlayerIndexPrinted == false then
		DebugPrint(true, "Core is initialized!\n")
	end

	if onTickNextPlayerIndexPrinted == false then
		DebugPrint(true, "\nChecking if in main menu first time\n")
	end

	--At some point, Is* functions will be a variable list passed into the .
	--While only relevant in HCE, this value should still be valid as (false) in sapien.
	if AreWeInMainMenu() == false then
		DebugPrint(true, "Not in main menu!\n")
	end

	if onTickNextPlayerIndexPrinted == false then
		DebugPrint(true, "Calling first NextPlayerIndex")
	end

	--The Current setup for NextPlayerIndex doesn't really work so great
	NextPlayerIndex()

	if onTickNextPlayerIndexPrinted == false then
		DebugPrint(true, "NextPlayerIndex called\n")
	end

	onTickNextPlayerIndexPrinted = true
end

DebugPrint(true, "\tOntick callback registered")
RegisterCallBack(5, "OnTick")

if IsCustomEd() then
	DebugPrint(true, "\tCustom Edition Detected in Lua!\n")
	--TODO: Change when GetMaxLocalPlayers() actually works.
	MaxPlayers = 1
	DebugPrint(true, "\tSet MaxPlayers to 1\n")
elseif IsHek() then
	DebugPrint(true, "\tHEK Detected!")
	--Lua support within it is 100% experimental.
	if IsSapien() then
		DebugPrint(true, " Sapien detected!\n")
		--Sapien will likely only ever have 1 MaxPlayer.
		MaxPlayers = 1
	end
end


--[[
	--"RegisterCallBack"
	Requires two parameters
		string of the name of the function you wish to have called on the specified hook.

	As of right now, there's no way to detect what hooks are available for the current platform.

	The following hooks are _guaranteed_ to work on all platforms Tempera officially supports Lua:

	before_game_tick   = 4
	after_game_tick    = 5

	post_initialize    = 6
	-- All memory and game structures have been allocated, this is right after the game's network connections have
	been initialized, but before main loop. Before #4 or #5 are called.

	post_dll_init      = 7
	-- called immediately after Tempera has been initialized. Before #4,#5, or #6 get called.

	This whole init.txt file is the first to run. Do not call Read/Write in the main file, it will quite possibly change!

	(0, 1, 2, 3) - No hooks in any viable running target for Tempera (yet).
	--]]

--First param is always the player index
--Second param (if there is one) is generally a float.

--"GetMaxLocalPlayers"
--Takes no parameters.
--Returns a number indicating the value of Tempera's MAX_PLAYER_COUNT_LOCAL

--"AreWeInMainMenu"
-- Returns a boolean indicating whether or not the game is in ui.map.

--"GetEngineContext" - Returns two variables:

--[[
CallVoidEngineFunctionByFunctionMapName
	-- NO params
	-- Returns to lua a boolean of whether or not the function was found & called.
	-- Assumes function does not return anything.
	--- May crash the game... Use with caution. Will document functions modder can potentially call.

IsPlayerSpawned
	-- 1 Param - player index (0 - 15) if player is spawned or not.
	-- returns true/false

GetPlayerAddress
	-- 1 param, player index (0-15)
	-- Returns unsigned 32 bit integer indicating address of player location. returns -1 if invalid index.
--]]

--[[
	IsCoreInitialized - Returns true if Core (the interface used to access gamestate) has been initialized.
	- Check this function _before_ attempting to access the Core variable within CurrentEngine.

	IsCustomEd - True if current context is CustomEdition
	IsSapien - True if current context is Sapien.
	IsHek - True if current context is HEK
	--Coming soon (ish): Actual Version numbering

	GetEngineContext()
	- No parameters.
	- could (theoretically) return any of the following:
	"DEDI";
	"SAPI"; - (SAPIEN)
	"HEK"; - (GENERIC HEK, unrecognized)
	"PC";
	"MAC";
	"STBZ";
	"TRIL";
	"CE";
	"";
		- Tempera cannot tell the difference between versions of halo ce (yet).
		- if it's not sapien or haloce it will return ""
--]]

-- Low-Level functions:
--[[
	"ReadByte"
	"Read32f"
	"Read32i"
	"Read16i"
--]]
--Read* functions take 1 parameter - the address you would like to read.
-- `i` stands for `integer` - Whole numbers. think of haloscript's short (for 16i) and long (for 32i) types.
-- integers may be negative.
-- `f` for float - a single-precision floating point number. Think of Haloscripts "real" type.

--Note: Lua only has 3 base types - booleans, floats, and strings. It has more complex types like maps.
-- I suggest reading more on lua @ www.lua.org

--[[
	"WriteByte"
	"Write32f"
	"Write32i"
	"Write16i"
--]]
--2 parameters (Address, Val)
-- Address is the location (in memory) to write the value.
-- Since lua does not have integers as a base type, Tempera only does simple bounds checking on the value passed in.
-- Other than that, Tempera assumes the value is correct, and dumbly writes the value to memory.
