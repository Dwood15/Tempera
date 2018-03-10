<h1 align="center">Chimera</h1>
<p align="center">
    A feature-rich mod for Halo Custom Edition
</p>

# Introduction

Chimera is a mod for Halo Custom Edition which contributes a wide variety of essential features to enhance or fix Halo.
This documentation should assist in setting up Chimera while hopefully providing more useful information than what a forum post may bring.
For general information on Chimera, go to https://chimera.opencarnage.net

# Table of Contents
* [**Building**](#building)
* [**Requirements**](#requirements)
  * [Reasons of Requirements](#reasons-of-requirements)
* [**Getting Started**](#getting-started)
  * [Chimera Directory Structure](#chimera-directory-structure)
    * [chimerainit.txt and chimerasave.txt](#chimerainittxt-and-chimerasavetxt)
    * [Lua Scripting](#lua-scripting)
* [**Retail Map Compatibility**](#retail-map-compatibility)
* [**Commands**](#commands)
  * [Uncategorized](#uncategorized)
  * [Debug](#debug)
  * [Enhancements](#enhancements)
  * [Fixes](#fixes)
  * [Interpolation](#interpolation)
  * [Visuals](#visuals)
  * [Xbox](#xbox)
* [**Credits**](#credits)
* [**Links**](#links)

# Requirements

Chimera supports Halo Custom Edition version 1.10. If you use an older version of the game or use the retail version, no guarantees can be made on how stable or compatible Chimera will be.
As for hardware, not every PC hardware configuration can be tested, as the number of possible PC configurations which could affect how well Halo may run with Chimera may quite possibly be in the trillions.
Instead, I can only give you a more conservative estimate on what should be able to run the game with at least 900p 60 FPS, maximum settings, with `chimera_interpolate` set to `6`.


| Type             | Part                                                                                               | Release Year                                       |
| ---------------- | -------------------------------------------------------------------------------------------------- | -------------------------------------------------- |
| Operating System | Windows 7 or newer<br>Linux with Wine 3.0 (free)                                                   | 2009<br>2018                                       |
| CPU              | Intel: Intel Core 2 Duo E8400 (3.0 GHz) or better<br>AMD: AMD Athlon II X2 250 (3.0 GHz) or better | 2008<br>2009                                       |
| Graphics         | Nvidia: NVIDIA GeForce 9400 GT<br>AMD: ATI Radeon HD 4550<br>Intel: Intel HD 3000                  | 2008<br>2008<br>2011                               |
| RAM              | 4 GB                                                                                               | 2000 (DDR release date)<br>Circa 2012 (mainstream) |

*Most PCs built within the past five years match or exceed these specifications.*

## Reasons of Requirements

The Intel CPU is a mid-ranged CPU from the year 2008, and the AMD CPU is a mid-ranged processor from the year 2009.
“Mid-ranged” is relative to the year 2009, as these processors are slow by today’s standards.
These CPUs are from an era where Intel and AMD had similar IPC, and at 3.0 GHz, they should perform identically and easily handle a modest level of `chimera_interpolate`.

For graphics, the NVIDIA GeForce 9400 GT was released in 2008. The ATI Radeon HD 4550 was released in around 2008.
These low end GPUs were low end even when they were released, and they were nothing compared to NVIDIA GeForce 9800 GTX or ATI Radeon HD 4850 from their respective times.
These processors are low-end, but Halo, a game from 2001, is not graphics-intensive. `chimera_interpolate` does not directly add GPU load but, rather, it takes better advantage of it.

As for the RAM, while Halo does not consume very much RAM (only a few hundred MB at most), having 4 GB ensures that your operating system will run optimally.
It is also not hard to come by if your PC can be upgraded to 4 GB.

**If you use an unsupported software or hardware configuration, please do not report bugs.**

# Getting Started

Chimera can be obtained from https://chimera.opencarnage.net. Chimera comes in a .7z file which may require [7-Zip](http://www.7-zip.org) if on Windows or [p7zip](http://p7zip.sourceforge.net/) if on Linux.
Some Linux distributions may already include the ability to open `.7z` archives without any additional software installation.

## Chimera Directory Structure

When you first start Chimera, Chimera will create a directory in `Documents\My Games\Halo CE` (or wherever you specified -path) and this directory will contain these files:

```
chimera               - directory - root directory
|   chimerainit.txt   - text file - initialization
|   chimerasave.txt   - text file - initialization (settings)
|
\---lua               - directory - lua scripts root
    +---global        - directory - global lua scripts
    \---temp          - directory - map lua scripts
```

The `chimera` folder is where Chimera’s configuration is stored. This is important so you do not have to re-enter commands on startup.

### `chimerainit.txt` and `chimerasave.txt`

Inside the `chimera` folder are also two files: `chimerainit.txt` and `chimerasave.txt`. These files do the same thing on startup: they feed Chimera commands into Chimera’s command parser.
They also are the same format. However, the difference is that `chimerasave.txt` is overwritten by Chimera as commands are used and is executed after `chimerainit.txt`, effectively overriding it.
`chimerainit.txt` can contain comments (prefixed with `#`) and can also be distributed to other people.

You can also put `chimerainit.txt` in the root directory of Halo (that is, the same folder that contains `haloce.exe`). This will be executed even sooner than the `chimerainit.txt` file in the chimera directory.

### Lua Scripting

Chimera also supports Lua scripting. Scripts in the `global` folder are loaded on startup. They remain permanently loaded unless the user uses `chimera_reload_lua`.
Scripts in the map folder is loaded when a map is loaded and unloaded when a map is unloaded. Scripts may also be contained inside of map files.
A Lua scripting section will be added to this documentation in a future date.

# Retail Map Compatibility

Chimera can enable Halo Custom Edition to load retail Halo maps. This feature requires a small amount of setup in order to be enabled:

1. Create a folder chimera in your `maps` folder. Make sure you have write access to this folder.
2. Copy your retail (Halo PC) `bitmaps.map` and `sounds.map` files into this folder. ***Make sure you *COPY* them; these files *WILL* be deleted.***
3. Copy any maps you wish to run with Halo Custom Edition into your Halo Custom Edition Maps folder.
4. Start up Chimera. Chimera will patch the `bitmaps.map` and `sounds.map` files in the `chimera` folder, replacing them with `ce_bitmaps.map` and `sounds.map`, respectively. Halo Custom Edition will then load these files.

Once finished, you will be able to run maps (such as the original Halo: Combat Evolved campaign) without having to convert them with a program such as Combustion.

**Notes**

If you want to play the Halo: Combat Evolved campaign, there are other ways to do this than this way:

* Moses’s Refined Campaign: http://forum.halomaps.org/index.cfm?page=topic&topicID=50277
* SPV3 (requires installing stuff like a launcher): https://www.reddit.com/r/halospv3/
* Converted maps (scattered around on Halo Maps): http://hce.halomaps.org/?nid=386

Replacing any multiplayer maps may prevent you from joining servers with those maps.
Chimera does NOT allow you to join retail servers regardless of if you are using a retail map or a Custom Edition map.

# Commands

Chimera has a variety of commands at your disposal. For organization, these are grouped into categories. You can also list these commands with the `chimera` command.

## Uncategorized

These commands are primarily for configuring the Chimera client itself rather than Halo. As such, they are not in any sort of category.

* `chimera` - This command is the commands directory for Chimera.
  * `chimera` - Display version and a list of command categories.
  * `chimera <category>` - Display a list of commands in a category.
  * `chimera <command>` - Display help for a command.
* `chimera_reload_lua` - Reload all Lua scripts.
* `chimera_verbose_init` - Get or set whether `chimerainit.txt` or `chimeraname.txt` commands should output messages.

## Debug

These commands are primarily for assisting map developers or modders.

* `chimera_budget [0-2]` - Get or set whether to show or hide various budgets.
  * `0` - Off
  * `1` - On (Modded budgets if a mod is installed such as HAC2)
  * `2` - On (Stock budgets)
* `chimera_devmode [true/false]` - Get or set whether or not to enable Halo’s developer commands.
* `chimera_player_info` - Show player information.
* `chimera_tps [ticks per second]` - Get or set tick rate. This value cannot be set below 0.01.
* `chimera_wireframe [true/false]` - Get or set whether or not to enable or disable wireframe mode. This will not work while in a server.

## Enhancements

These commands exist to enhance your gameplay experience.

* `chimera_auto_center [0-2]` - Get or set how auto centering of vehicles should behave.
  * `0` - Broken stock behavior
  * `1` - Fixed behavior
  * `2` - Disable automatic centering
* `chimera_block_letterbox [true/false]` - Get or set whether or not to block the letterbox effect in cinematics.
* `chimera_block_mo [true/false]` - Get or set whether or not to disable multitexture overlays. This feature is intended to fix the buggy HUD on the stock sniper rifle, but multitexture overlays may be used correctly on some maps.
* `chimera_block_mouse_acceleration [true/false]` - Get or set whether or not to block mouse acceleration. Note that some mice may still exhibit mouse acceleration.
* `chimera_block_server_messages [true/false]` - Get or set whether or not to block inbound server messages.
* `chimera_block_zoom_blur [true/false]` - Get or set whether or not to disable the zoom blur.
* `chimera_disable_buffering [true/false]` - Get or set whether or not to disable buffering. This may improve input latency.
* `chimera_enable_console [true/false]` - Get or set whether or not to automatically enable the console. Unlike most other features, this feature is enabled by default.
* `chimera_gamepad_vertical_scale [value]` - Get or set whether or not to scale gamepad vertical sensitivity.
* `chimera_mouse_sensitivity [<horizontal> <vertical> | <false>]` - Set the horizontal and vertical mouse sensitivities. Values less than 1 do not work properly if mouse acceleration is enabled.
* `chimera_show_spawns [true/false]` - Get or set whether or not to show spawns.
* `chimera_skip_loading [true/false]` - Get or set whether or not to skip the multiplayer loading screen.
* `chimera_uncap_cinematic [true/false]` - Get or set whether or not to remove the 30 FPS framerate cap in cinematics. This may result in objects jittering during cutscenes if chimera_interpolate is not enabled.

## Fixes

These commands fix various problems with Halo. Some fixes, such as the magnetism fix and the descope fix, are enabled by default and have no commands to disable them.

* `chimera_aim_assist [true/false]` - Get or set whether or not fix aim assist for gamepads. This feature is on by default.
* `chimera_sniper_hud_fix [true/false]` - Get or set whether or not to fix the sniper HUD. This may not work on protected maps.
* `chimera_widescreen_fix [0-2]` - Get or set whether or not to (mostly) fix the HUD. **Note:** This will break the HUD if you are using any other widescreen fix.
  * `0` - Off
  * `1` - On
  * `2` - On (center HUD)
* `chimera_widescreen_scope_fix [true/false]` - Enhance an existing widescreen fix by also fixing the scope mask if it's not fixed.

## Interpolation

This is Chimera’s flagship feature. It smoothes out object movement at higher framerates, increasing the game’s fluidity.

* `chimera_widescreen_scope_fix [true/false]` - Enhance an existing widescreen fix by also fixing the scope mask if it's not fixed.
* `chimer_interpolate [off/low/medium/high/ultra]` - Get or set the interpolation level. Interpolation smoothes out object movement between ticks, providing a substantial visual improvement. Higher levels incur greater CPU usage and may impact framerate on slower CPUs.

Here is what each level of interpolation does:

|            | players | weapon | equipment | projectile | scenery | machine | particles | cloth |
| ---------- | ------- | ------ | --------- | ---------- | ------- | ------- | --------- | ----- |
| **Low**    | `B`     | `C`    | `C`       | `~`        | `~`     | `C`     | `~`       | `Yes` |
| **Medium** | `B`     | `B`    | `B`       | `C`        | `C`     | `B`     | `Yes`     | `Yes` |
| **High**   | `B`     | `B`    | `B`       | `B`        | `B`     | `B`     | `Yes`     | `Yes` |
| **Ultra**  | `A`     | `A`    | `A`       | `A`        | `A`     | `A`     | `Yes`     | `Yes` |

* `C` = Interpolation (no rotation) (good)
* `B` = Interpolation (with rotation) (better)
* `A` = Interpolation (with rotation) and no distance optimization (best)

## Startup

These commands modify aspects of Chimera's fast startup feature.

* `chimera_cache [true/false]` - Get or set whether or not to use a cache for fast startup.
* `chimera_cache_clear` - Clear the cache.
* `chimera_modded_stock_maps [true/false]` - Get or set whether or not stock maps will use hardcoded CRC32s. This may be required for some maps to work.

## Visuals

These features offer some modifications for visuals such as HUD.

* `chimera_af [true/false]` - Get or set whether or not to enable anisotropic filtering.
* `chimera_block_firing_particles [true/false]` - Get or set whether or not to block firing particles.
* `chimera_block_gametype_indicator [true/false]` - Get or set whether or not to turn off the gametype indicator.
* `chimera_block_server_ip [true/false]` - Get or set whether or not to hide the server IP. This may be useful for streamers.
* `chimera_block_vsync [true/false]` - Get or set whether or not to turn vSync off startup.
* `chimera_set_resolution <width> <height> [refresh rate] [vsync] [windowed]` - Change Halo's resolution. Width and height can be either resolution in pixels or an aspect ratio.
* `chimera_throttle_fps [max FPS]` - Throttle Halo's framerate.
* `chimera_vfov [VFOV]` - Get or change your FOV by attempting to lock to a specific vertical FOV. This will distort your FOV if HAC2, Open Sauce, etc. are modifying your horizontal FOV. 1 defaults to 55.41 degrees, or Halo’s standard FOV.

## Xbox

These commands add enhancements that emulate or restore functionality from the original console release of the game.

* `chimera_hud_kill_feed [true/false]` - Get or set whether or not to emit kills and deaths messages as HUD text.
* `chimera_safe_zones [true/false]` - Get or set whether or not to emulate Xbox safe zones.
* `chimera_simple_score_screen [true/false]` - Get or set whether or not to use a simplified in-game score screen.
* `chimera_split_screen_hud [true/false]` - Get or set whether or not to use Halo's split screen HUD. **Note:** This may cause potential crashing issues on HAC2.

# Building

To build Chimera, you will need [MinGW32](http://www.mingw.org/) installed and configured properly in your `PATH` environment variable.
Then run the included `build.bat` batch script.

# Credits

These people were invaluable to the development of Chimera:

* **Kavawuvi** - Mod development and creation; documentation
* **Yumiris** - Markdown documentation
* **Btcc22** - [Halo Anticheat 2](http://blog.haloanticheat.com/) development; HAC2 was a source of inspiration for Chimera’s development. HAC2’s signature scanning code was also vital for Chimera’s existence.
* **Oxide** - Phasor development; Phasor had several useful signatures of Halo that proved useful for various things in Chimera. [Phasor source code](https://github.com/urbanyoung/Phasor)
* **Wizard** - His addresses and offsets file saved a lot of time finding things.
* **GoofballMichelle** - Testing; help with interpolation.
* **Tucker933** - Hosting Chimera’s download file and subdomain.
* **StormUndBlackbird** - News coverage of Chimera. [Discord](https://discord.gg/M6YfcE2) and [YouTube channel](https://www.youtube.com/user/StormUndBlackbird).
* **Masters1337** - SPV3; helped get Chimera a lot of attention. [SPV3 Subreddit](https://www.reddit.com/r/halospv3/).
* **Halo 1 Hub** - Testing
* **Altis**
* **Devieth**
* **Dwood** - Reasons coming soon. :)

# Links
* [Chimera on Open Carnage](https://chimera.opencarnage.net)
* [Chimera Discord](https://discord.gg/ZwQeBE2)
* [Chimera source code](https://github.com/Halogen002/Chimera)
