#pragma once

#ifndef WIN32
#define WIN32
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _X86_
#define _X86_
#endif

#define DIRECTINPUT_VERSION 0x0800

#include <algorithm>
#include <array>
#include <assert.h>
#include <crtdbg.h>
#include <cstddef>

#include <cstdint>

#include <d3dx9math.h>

#include <fstream>
#include <functional>

#include <handleapi.h>

#include <minwindef.h>
#include <minwinbase.h>

#include <rpc.h>

#include <sal.h>
#include <stdexcept>
#include <stdlib.h>

#include <string>

#include <stdio.h>
#include <time.h>
#include <type_traits>
#include <vector>

#include <Windows.h>
#include <winnt.h>
//Our defined includes below:
typedef unsigned int uintptr_t;

// GameSpy includes & definitions
/* TODO: GameSpy SDK bug fix. see official OS repository */

// Always use ASCII
#undef GSI_UNICODE

#include "macros_generic.h"
#include <dbghelp.h>
#include <mysql.h>
