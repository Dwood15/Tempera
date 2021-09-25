
add_subdirectory(objects)
add_subdirectory(units)

set(GAMESTATE_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/objects.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/objects/objectcontroller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/controls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/players.cpp
	${UNITS_CXX}
	${G_OBJECTS_CXX}
	PARENT_SCOPE)
