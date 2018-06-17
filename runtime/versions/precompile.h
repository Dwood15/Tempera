#pragma once

#define _USE_MATH_DEFINES // hurrrrrrrrrr, i like math!

#include <algorithm>
#include <array>
#include <assert.h>
#include <crtdbg.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <Dinput.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <iterator>
#include <math.h>

#define DIRECTINPUT_VERSION 0x0800

//////////////////////////////////////////////////////////////////////////
// STD C includes
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <io.h>
#include <errno.h>
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// STL includes
#include <functional>
#include <memory> // std::unique_ptr
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <utility>
#include <vector>
#include <stdio.h>
#include <string>
#include <thread>
#include <type_traits>
#include <random>
#include <vector>
//////////////////////////////////////////////////////////////////////////

#ifndef WIN32
#define WIN32
#endif

#define NOMINMAX

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _X86_
#define _X86_
#endif

#include <minwinbase.h>
#include <minwindef.h>
#include <rpc.h>
#include <sal.h>
#include <time.h>
#include <winnt.h>
#include <WinSock2.h>

// GameSpy includes & definitions
/* TODO: GameSpy SDK bug fix. see official OS repository */

// Just to make sure we're always using ASCII
#undef GSI_UNICODE

//////////////////////////////////////////////////////////////////////////
//Our includes below:

#include "../../BlamSauce/cseries/MacrosCpp.h"
#include "../../BlamSauce/cseries/base.h"
#include "../../BlamSauce/math/real_math.h"
#include "../../BlamSauce/memory/datum_index.h"
#include "../../BlamSauce/tags/group/base.h"
#include "../../BlamSauce/tags/group/markup.h"


