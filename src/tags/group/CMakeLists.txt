set(GROUP_H
	${CMAKE_CURRENT_SOURCE_DIR}/markup.h
	${CMAKE_CURRENT_SOURCE_DIR}/c_tag_iterator.h
	${CMAKE_CURRENT_SOURCE_DIR}/groups_structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/structure_macros.h
	${CMAKE_CURRENT_SOURCE_DIR}/tag_groups.h
	${CMAKE_CURRENT_SOURCE_DIR}/tagreference.h
	${CMAKE_CURRENT_SOURCE_DIR}/tagdata.h
	${CMAKE_CURRENT_SOURCE_DIR}/tagblock.h
	${CMAKE_CURRENT_SOURCE_DIR}/verification.h

	PARENT_SCOPE)

set(TAGS_GROUP_CXX
	${CMAKE_CURRENT_SOURCE_DIR}/base.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/groups_structures.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tag_groups.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/c_tag_iterator.cpp
	PARENT)
