
if  ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(FATAL_ERROR "GCC/CMAKE USUPPORTED AT THIS TIME.")
	return()
endif()
#	set(CMAKE_CXX_COMPILER_ID "Clang")
	#	set(CMAKE_C_COMPILER C:\\Program\ Files\\LLVM\\bin\\clang.exe)
	#	set(CMAKE_CXX_COMPILER C:\\Program\ Files\\LLVM\\bin\\clang++.exe)

	set(CMAKE_C_FLAGS "-Wall -std=c17 -m32 ")
	set(CMAKE_C_FLAGS_DEBUG "-g")
	set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
	set(CMAKE_C_FLAGS_RELEASE "-O4 -DNDEBUG")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

	#-fdelayed-template-parsing

	#-wno-switch b/c of: warning: 19 enumeration values not handled in switch:
	set(CLANG_DISABLED_WARNINGS " -Wno-unused-variable -Wno-deprecated -Wno-unused-function -Wno-switch -Wno-overloaded-virtual")
	set(CLANG_ENABLED_WARNINGS "-Wall -Wpadded  -Wmissing-braces ")

	set(CLANG_ONLY_OPTIONS "-fms-extensions -fms-compatibility -ferror-limit=1 -fmodules -fmodules-cache-path=${CMAKE_SOURCE_DIR}/cache -Warray-bounds-pointer-arithmetic -Wmissing-method-return-type -Wfor-loop-analysis -Wmismatched-tags -Wcomma -Watomic-properties -Wassign-enum")

	set(CLANG_MS_WARNINGS " -Wmicrosoft-charize -Wextra-semi -Wmicrosoft-cpp-macro -Wmicrosoft-enum-value ")
	set(CLANG_EXTRAS "-Wc++2a-extensions -Wc++2a-compat ${CLANG_MS_WARNINGS}")

	set(CLANG_MACROS "-DWIN32 -D_USRDLL -D_WINDLL -DCMAKE_LINK_WHAT_YOU_USE=TRUE")
	set(CLANG_EXTRA_SETTINGS "-fms-extensions ")
	#-stdlib=libstdc++  -nostdlib -isystem C:/msys32/mingw64/include/c++/8.2.0  -Wno-microsoft-include

	set(QUIET_GCC "-Wno-comment -Wno-multichar -Wno-unknown-pragmas -Wno-unused-local-typedefs")

	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

	set(CMAKE_CXX_FLAGS "-Wfatal-errors -m32 -masm=intel -fpermissive ${CMAKE_CXX_FLAGS_DEBUG} ${QUIET_GCC} ${CLANG_EXTRA_SETTINGS} ${CLANG_ENABLED_WARNINGS} ${CLANG_DISABLED_WARNINGS} ${CLANG_MACROS}")

	message(FATAL_ERROR "Clang and gcc unsupported ATM, sorry")
	message(STATUS "Forcing GNU to Clang compiler because CLion is dumb.")
	#TODO: SET CLANG STUFFS.
	set(MSVC 0)
