Tempera
==

License Disclaimer
=========

All code within this repository is Licensed under the GNU General Public License, Version 3 or newer license and comes with no warranty, express or implied.

MSFT Usage Guidelines
=========

The Tempera project, being targeted at Halo Custom Edition and the HEK are **not** covered under the Microsoft Content Usage guidelines. Halo Custom Edition and the HEK licenses predate both.

General Disclaimer
=========

Tempera may cause your files to corrupt or game or pc to crash. None of which I am liable. Backup your files regularly.

Table of contents
=================

   * [Setup](#setup)
      * [For WINE](#for-wine)
      * [Setting up Tempera for Windows 10](#for-windows-10)
      	* [Install NPP and Everything Service](#install-npp-and-everything)
      	* [Install Build Tools](#install-the-build-tools)
      	* [Build Tempera](#build-tempera)
      	* [Quick Dev Intro](#quick-dev-intro)
   * [FAQ](#faq)
   * [Attributions](#attributions)

---
Setup
==
---

## For WINE
    TODO: Step-By-Step tutorial for getting HCE and HEK set up and running on WINE

---  

## For Windows 10
  (This Documentation is extra-detailed so I have some documentation of installing all this stuff more than anyone else)
  ### Install NPP and Everything
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

  ### Install The Build Tools 
 
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

  ### Build Tempera
  
  - Generate the build files: 
   
		cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - NMake Makefiles" .

   - Note: Release builds are unsupported. The optimizations can potentially mess with the functions being ran.
  
   Open ADMIN cmd prompt. Find and run VCvarsall.bat (see something like:  `C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build`)
   
   
    cmake --build cmake-build-debug --target all

  ### Quick Dev Intro
   
   You should now find a few files:
    - tempera.init.lua 
    - and dinput8.dll in your halo custom edition install folder.
    - It is safe to delete those newly-created files, assuming you didn't edit them :).
   
   Note: I have only tested this with Halo installed to the program files location. 
   In theory, cmake will read the registry for the location of Halo's install. YMMV.
   Please let me know if you have halo installed to another dir and it doesn't work.

---

FAQ
=================

Q: What is Tempera? 

A: Tempera is a love letter for Halo, an omnibus reverse-engineering mod/project for Halo Custom Edition, based on a heavily stripped-down Open Sauce project (thanks, Kornman!).

---

Q: As a Developer, What Can I do with Tempera Right Now?

A: Not much. You can spit some logs to file and hook into certain events via lua or send certain logs to console 

---

Q: As a Modder, What Can I do with Tempera Right Now?

A: Print some Lua text to console 

---

Q: Why is the project activity so low?

A: I'm the only one working on it, and Tempera is a novelty project that I work on when I have extra energy between being a software engineer.

---

Q: What do you plan to do with Tempera?

A: There's a number of long-term goals for the project:
  1. Control bipeds inside Sapien via controller
  2. Allow the Keyboard + mouse and controller to update separate bipeds  
  3. Render First Person View for separate bipeds
  4. Add forge-like mode for Halo Custom Edition

---

Q: Current State?

A:
  1. A series proofs of concepts - Certain features are in the code which showcase much of this is possible. 
  2. A mishmash of code from Kornman00 an TheFieryScythe's Open Sauce project, as well as SilentK and Abyll's haloforge project.
  3. Between major refactorings and feature updates.
  4. Breaking changes happen probably nearly every single commit. Haven't bothered to begin versioning

A demo showing that we can render 4 cameras at once:

https://user-images.githubusercontent.com/8879021/132976632-ac583fe8-d4e5-41bc-845d-1d8e62456d6b.mp4

A demo showing the biped control working: 

https://www.youtube.com/watch?v=uyb1S1r83Vw

---

Q: Can I help?

A: Yes, but for the most part I need people who are comfortable reverse-engineering and turning x86 into readable C.

---

Q: How?

A: There's a number of things that I want to be done, which aren't reverse-engineering-focused. Check the Repository Issues.

---

# Attributions

Various files and items in this codebase can be attributed as follows:


All code within this repository is Licensed under the GNU General Public License, Version 3 or newer license and comes with no warranty, express or implied.
MSFT Usage Guidelines

The Tempera project, being targeted at Halo Custom Edition and the HEK are not covered under the Microsoft Content Usage guidelines. Halo Custom Edition and the HEK licenses predate both.
 * Kornman00 (Sean Cooper) - [For Open Sauce](https://bitbucket.org/KornnerStudios/opensauce-release/wiki/Home)
 * Kavawuvi//Snowy for their endless knowledge, support, and the [Chimera project](https://github.com/Kavawuvi/Chimera)
  * Endless font of knowledge, support, and shitposts
 * SilentK & Abyll - For initial implementaiton of haloforge. Most of our forge features are built on the work of these two.
   * AFAIK, haloforge's source is gone with the demise of Google Code and is largely lost to time. Even then, it was mostly kept private and only intended for halo PC v1.08 in 2008-2009
   * Because of this, Tempera may be the only remaining repository containing code from the original live_projekt.
 * ZBE - Helped me get the initial prototypes running and answering my Reverse-Engineering-related questions.
 * Dwood15 - Primary developer and maintainer of Tempera. 

Attributions in individual files are being removed in favor of this readme notice. Tempera has grown and evolved to the point that maintaining attribution in every file is impractical and (quite frankly)silly.
