set(C_SHARED_H
	#	${CMAKE_CURRENT_SOURCE_DIR}/shared/pc_sound_cache.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/shared/pc_texture_cache.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/shared/texture_cache.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/shared/sound_cache.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/shared/shared_cache_file_definitions.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/shared/shared_cache_files.hpp
	)

set(CACHE_H
	${C_SHARED_H}
	#	${CMAKE_CURRENT_SOURCE_DIR}/builder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/cache_base.h
	${CMAKE_CURRENT_SOURCE_DIR}/cache_files.h
	${CMAKE_CURRENT_SOURCE_DIR}/cache_files_globals.h
	#	${CMAKE_CURRENT_SOURCE_DIR}/cache_files_yelo.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/cache_files_yelo_base.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/cache_files_structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/cache_files_structures_yelo.h
	${CMAKE_CURRENT_SOURCE_DIR}/data_file_structures.h
	#	${CMAKE_CURRENT_SOURCE_DIR}/predicted_resources.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/physical_memory_map_structures.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/physical_memory_map.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/shared/pc_texture_cache.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/cache_file_memory_layout_table_definitions.hpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/cache_file_memory_gestalt_definitions.hpp
	PARENT_SCOPE)
