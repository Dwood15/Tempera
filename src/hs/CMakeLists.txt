set(S_LIBRARY_H
	${CMAKE_CURRENT_SOURCE_DIR}/library/function_declarations.h
	${CMAKE_CURRENT_SOURCE_DIR}/library/globals_declarations.h
	${CMAKE_CURRENT_SOURCE_DIR}/library/internal_compile.inl
	${CMAKE_CURRENT_SOURCE_DIR}/library/internal_runtime.inl
	${CMAKE_CURRENT_SOURCE_DIR}/library/library.h
	${CMAKE_CURRENT_SOURCE_DIR}/library/yelo_definitions.h
	)

set(S_EXTENSIONS_H
	${CMAKE_CURRENT_SOURCE_DIR}/script_extensions/hs_base
	)


set(S_TYPES_H
	${CMAKE_CURRENT_SOURCE_DIR}/types/compile.inl
	${CMAKE_CURRENT_SOURCE_DIR}/types/traits.h
	${CMAKE_CURRENT_SOURCE_DIR}/types/types.h
	)

set(SCRIPTING_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/hs.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/threads.cpp
	PARENT_SCOPE)

set(SCRIPTING_H
	${S_EXTENSIONS_H}
	${S_LIBRARY_H}
	${S_TYPES_H}
	${CMAKE_CURRENT_SOURCE_DIR}/hs.h
	${CMAKE_CURRENT_SOURCE_DIR}/compile.h
	${CMAKE_CURRENT_SOURCE_DIR}/object_lists.h
	${CMAKE_CURRENT_SOURCE_DIR}/object_lists_structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/runtime.h
	${CMAKE_CURRENT_SOURCE_DIR}/structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/threads.h
	PARENT_SCOPE)
