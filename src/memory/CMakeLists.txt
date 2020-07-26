#Yeah, I know, I got lazy. But holey moley is it a pain to do so many of these different CMakeLists files.
set(M_UPGRADES_H
	${CMAKE_CURRENT_SOURCE_DIR}/upgrades/game_state_yelo.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/upgrades/gs_upgrades.inl
	)

set(MEMORY_H
	${CMAKE_CURRENT_SOURCE_DIR}/array.h
	${CMAKE_CURRENT_SOURCE_DIR}/array_yelo.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/bitstream.h
	${CMAKE_CURRENT_SOURCE_DIR}/circular_queue.h
	${CMAKE_CURRENT_SOURCE_DIR}/data_array.h
	${CMAKE_CURRENT_SOURCE_DIR}/data_base.h
	${CMAKE_CURRENT_SOURCE_DIR}/data_iterator.h
	${CMAKE_CURRENT_SOURCE_DIR}/datum_index.h
	${CMAKE_CURRENT_SOURCE_DIR}/files.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/linked_list.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/lruv_cache.h
	${CMAKE_CURRENT_SOURCE_DIR}/memory_pool.h

	${CMAKE_CURRENT_SOURCE_DIR}/memory_yelo.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/memory_interface_base.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/naked_func_writer.h
	${CMAKE_CURRENT_SOURCE_DIR}/physical_memory_allocation.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/packed_file.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/position_queue.h
	${CMAKE_CURRENT_SOURCE_DIR}/profiling.h
	${CMAKE_CURRENT_SOURCE_DIR}/queued_sigscanner.h
	${CMAKE_CURRENT_SOURCE_DIR}/simple_circular_queue.h
	${M_UPGRADES_H}
	PARENT_SCOPE)
