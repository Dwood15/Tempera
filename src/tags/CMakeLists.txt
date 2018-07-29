add_subdirectory(group)
add_subdirectory(string_ids)

set(TAGS_H
	${STRING_ID_H}
	${GROUP_H}
	${CMAKE_CURRENT_SOURCE_DIR}/map.h
	${CMAKE_CURRENT_SOURCE_DIR}/tags.h
	PARENT_SCOPE)
