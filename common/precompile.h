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

#include <algorithm>
#include <array>
#include <assert.h>
#include <crtdbg.h>
#include <cstddef>

#include <cstdint>

#include <d3dx9math.h>
#include <functional>

#include <handleapi.h>

#include <minwindef.h>
#include <minwinbase.h>

#include <rpc.h>

#include <sal.h>
#include <stdexcept>
#include <string>

#include <stdio.h>
#include <time.h>
#include <type_traits>
#include <vector>

#include <winnt.h>
//Our defined includes below:
typedef unsigned int uintptr_t;
#define ADDR static constexpr uintptr_t

#include "target_version.h"
#include "../src_generic/headers/macros_generic.h"
