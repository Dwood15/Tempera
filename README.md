# Tempera

### A project seeking to:
Tempera aims to:
  - Add Local Splitscreen Multiplayer to Halo Custom Edition for PC
  - Add a Forge-like mode for Halo Custom Edition
  - Be equivalent to an Open Sauce v2

## Disclaimer 

This project may cause your game to crash, files to corrupt, or, if luck does not shine down upon you, your machine to crash.

### Tempera is: 
  - A mishmash of code from Kornman00 an TheFieryScythe's Open Sauce project, as well as SilentK and abyll's haloforge project.
  - Between major refactorings and feature updates.
  - In Pre-Alpha

### Tempera is in need of:
  -  Feedback and help. No need for halo-specific modding experience. Just C++ and C knowledge. 
  
### Tempera is not: 
  - Stable. (I'm not even willing to consider this alpha phase yet)
  - Good for your health. The codebase will make you use a number of swears, probably cursing my name.
  
### Building Tempera Requires:
  - capstone_static.lib, - On-the-fly decompilation and debugging for Exception handling (not yet implemented) (bundled in repository)
  - detours.lib - Hooking directx functions. Looking to remove this dependency. (bundled in repository)
  - DirectX June 2010 SDK - https://www.microsoft.com/en-us/download/details.aspx?id=6812
  - Windows 10 SDK v. (10.0.16299.0) 
  - C++ 17 compiler. Tempera is untested on mingw/gcc. See: VS 2018 build tools: http://landinghub.visualstudio.com/visual-cpp-build-tools
  - Cmake - The Visual Studio ecosystem is a blight upon humanity.
  
## Attributions  
  
Various files and items in this codebase can be attributed as follows:

 *	SilentK & Abyll - For the initial implementaiton of haloforge. Most of haloforge's unique features are built on the work of these two. AFAIK, haloforge is gone with Google Code.
 *	Kornman00 (Sean Cooper) - For Open Sauce. See: https://bitbucket.org/KornnerStudios/opensauce-release/wiki/Home
 *  ZBE - Helping me get the initial prototypes running and helping to answer my RE-related questions.
 *	Dwood15 (Dason Woodhouse - Me) - Creator, primary developer and current maintainer of Tempera. 
  
At one point, I was using Chimera as a base for Tempera. All of Halogen002's code has been removed, and Tempera is now a completely separate codebase.

See Chimera here: https://github.com/Halogen002/Chimera

Tempera has grown and evolved to the point that maintaining attribution is impractical. All attributions in individual files will be removed in the near future in favor of a more generic notice.

SilentK and Abyll's haloforge project was mostly kept private. Intended for halo PC v1.08 in 2008-2009, I have updated it to Custom Edition, v1.10. Expect major changes between pushes.

Tempera's repository may be the only publicly released code from the original live_projekt.

Theoretically, Tempera should compile with MinGW thanks to CMake. In either case, the Windows 10 SDK and a separate DirectX sdk is required. Any DirectX sdk from 2007 to 2010 should be compatible. 

## License Notice

All code within this repository is under the GNU General Public License, Version 3, and comes with no warranty, express or implied.

This codebase isn't even covered under the Microsoft Content Usage guidelines.
