set(S_LIBRARY_H
	${CMAKE_CURRENT_SOURCE_DIR}/library/enums.h
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
	${CMAKE_CURRENT_SOURCE_DIR}/types/runtime.inl
	${CMAKE_CURRENT_SOURCE_DIR}/types/traits.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/types/types.h
	)

set(SCRIPTING_SRC
	PARENT_SCOPE)

set(SCRIPTING_H
	${S_EXTENSIONS_H}
	${S_LIBRARY_H}
	${S_TYPES_H}
	${CMAKE_CURRENT_SOURCE_DIR}/hs.h
	${CMAKE_CURRENT_SOURCE_DIR}/compile.h
	${CMAKE_CURRENT_SOURCE_DIR}/object_lists.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/object_lists_structures.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/runtime.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/runtime_structures.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/scenario.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/structures.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/threads.h
	PARENT_SCOPE)
