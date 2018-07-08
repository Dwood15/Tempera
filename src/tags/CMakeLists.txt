add_subdirectory(group)

set(TAGS_H
	${GROUP_H}
	${CMAKE_CURRENT_SOURCE_DIR}/map.h
	${CMAKE_CURRENT_SOURCE_DIR}/tags.h
	PARENT_SCOPE
	)
