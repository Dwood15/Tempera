# Tempera

## A GPLv3 Mod for Halo Custom Edition and the Halo Editing Kit

## Setting up Tempera under WINE
   ### TODO: Step-By-Step tutorial for getting HCE and HEK set up and running on WINE
## Setting up Tempera for Windows (Assuming Win 10)
  (This Documentation is extra-detailed so I have some documentation of installing all this stuff more than anyone else)
  ### Step 0: Install Notepad++ and Everything Service
  - [Have Git installed](https://gitforwindows.org/), with your gcc tools on the Path. 
    - If you open a cmd window and do `which which` you should get `/usr/bin/which`
  - [Install Notepad++](https://notepad-plus-plus.org/downloads)
  	- Install the x86 variant, regardless of your CPU type. (If you want to install plugins, they are all 32-bit) 
  - [Install The Everything Service](https://www.voidtools.com) 
    - Go to: `Tools -> Options`
    - Check: `View -> Alternate row color`
    - Check: `History -> Enable search history`
    - Set Hotkey: `Keyboard -> Toggle window Hotkey` and set it to something. Mine is: `Win + Shift + E`
    - Check: `Indexes -> Exclude -> Enable exclude list`, then add the following Exclude filters:
    	-  Add Filter -> `node_modules`
    	-  Add Filter -> `C:\Windows`
    	-  Any other folders you don't want noising up your results. There's methods for only searching allowed places but I would rather exclude folders)

  ### Step 1: Install Prerequisites 
 
  - [Download and install the DirectX June 2010 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
    - While I have attempted to investigate moving off this DXSDK Redist, there's a certain API which is available in the Jun 2010 variant which is not available in the "new" windows 10 variants. (TODO: Update this README with more specific details)
    - When installing you might encounter "error S1023". [Go into Add or Remove Programs and remove the existing Visual C++ 2010 installs (x86 and x64)](https://docs.microsoft.com/en-us/troubleshoot/windows/win32/s1023-error-when-you-install-directx-sdk)
    	- Installing this variant works as of Sep 11, 2021.
  - [MSVC Build tools - (Visual Studio not required)](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16)
    - Windows 10 SDK - (10.0.18362.0 or newer)
    	- Open a new console and ensure that you have cl.exe and msvc.exe are on your path. If they aren't: 
    		- Toggle an `Everything` window and search for `vcvarsall.bat`. 
    		- Right click -> Open Path -> In the new Explorer window type `cmd.exe` will open a cmd prompt in that folder
    		- Type in: `vcvarsall.bat x86` hit enter
    		- Export the PATH and copy paste it into NPP. 
    		- Open the Path window and add in the paths that weren't already there.
  - CMake, v 3.16 or newer (I have personally been exploring using the meson build system but haven't dove into it)
  - Halo Custom Edition (v110) and Halo Editing Kit must both be installed.

  ### Step 2: Build it.
  
  - Generate the build files: 
   
		cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - NMake Makefiles" .

   - Note: Release builds are unsupported. The optimizations can potentially mess with the functions being ran.
  
   Open ADMIN cmd prompt. Find and run VCvarsall.bat (see something like:  `C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build`)
   
   
    cmake --build cmake-build-debug --target all

  ### Step 3: There you go!

   
   You should now find a few files:
    - tempera.init.lua 
    - and dinput8.dll in your halo custom edition install folder.
    - It is safe to delete those newly-created files, assuming you didn't edit them :).
   
   Note: I have only tested this with Halo installed to the program files location. 
   In theory, cmake will read the registry for the location of Halo's install. YMMV.
   Please let me know if you have halo installed to another dir and it doesn't work.

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

This codebase isn't even covered under the Microsoft Content Usage guidelines, since Halo Custom Edition and the HEK licenses predate both.
