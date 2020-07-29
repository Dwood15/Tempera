# Tempera

## A GPLv3 Mod for Halo Custom Edition and the Halo Editing Kit

## Building Tempera
  ### Step 1: Install Prerequisites 
 
  - DirectX June 2010 SDK - https://www.microsoft.com/en-us/download/details.aspx?id=6812
  - MSVC Build tools - (Visual Studio not required) - https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16 
    - Windows 10 SDK - (10.0.18362.0 or newer) 
    - Ensure that cl, msvc tool is on the path 
  - CMake, v 3.16 or newer
  - Halo Custom Edition (v110), Halo Editing Kit must both be installed.

  ### Step 2: Build it.
  
  Generate the build files: 
  
    cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - NMake Makefiles" .

   Note: Release mode builds are currently untested.
   
    cmake --build cmake-build-debug --target all

  ### Step 3: There you go!

   
   You should now find a few files:
    - tempera.init.lua 
    - and dinput8.dll in your halo custom edition install folder.
    - It is safe to delete those newly-created files, assuming you didn't edit them :).
   
   Note: I have only tested this with Halo installed to the program files location. 
   In theory, cmake will read the registry for the location of Halo's install. YMMV.
   Please let me know if you continue to have issues.

  ### Disclaimer 

   This project may cause your game to die, files to corrupt, or, if luck does not shine down upon you, your machine to crash.

### What is Tempera? It aims to:
  - Add Local Splitscreen Multiplayer to Halo Custom Edition for PC
  - Add a Forge-like mode for Halo Custom Edition
  - Be equivalent to an Open Sauce v2

### Tempera is: 
  - A series proofs of concepts - Features are there, showcasing they're possible. 
  - A mishmash of code from Kornman00 an TheFieryScythe's Open Sauce project, as well as SilentK and abyll's haloforge project.
  - Between major refactorings and feature updates.
  - In Alpha - Breaking changes happen probably every single commit. 

### Tempera is in need of:
  -  Feedback and help. No need for halo-specific modding experience. Just C++ and C knowledge. 
  
### Tempera is not: 
  - Stable. (I'm barely even willing to consider this an alpha)
  - Good for your health. The codebase will make you use a number of swears, probably cursing my name.
  
### Open Source Compilers?
  - Long-term-ish, I will be looking to move the project to be compatible w/ Clang/Gcc. 
  This is what's been the easier setup for me..

## Attributions
  
Various files and items in this codebase can be attributed as follows:

 *	SilentK & Abyll - For the initial implementaiton of haloforge. Most of haloforge's unique features are built on the work of these two. AFAIK, haloforge is gone with Google Code.
 *	Kornman00 (Sean Cooper) - For Open Sauce. See: https://bitbucket.org/KornnerStudios/opensauce-release/wiki/Home
 *  ZBE - Helping me get the initial prototypes running and helping to answer my RE-related questions.
 *	Dwood15 (Dason Woodhouse - Me) - Primary developer and current maintainer of Tempera. 
  
At one point, I was using Chimera as a base for Tempera. All of Kavawuvi's code has been removed, and Tempera is now a completely separate codebase.

See Chimera here: https://github.com/Kavawuvi/Chimera

Tempera has grown and evolved to the point that maintaining attribution in every file is impractical and (quite frankly)silly. Attributions in individual files are being removed in favor of a readme notice.

SilentK and Abyll's haloforge project was mostly kept private. Intended for halo PC v1.08 in 2008-2009, I have updated it to Custom Edition, v1.10. Expect major changes between pushes.

Tempera's repository may be the only publicly released code from the original live_projekt.

## License Notice

All code within this repository is under the GNU General Public License, Version 3, and comes with no warranty, express or implied.

This codebase isn't even covered under the Microsoft Content Usage guidelines.
