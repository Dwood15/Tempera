#add_subdirectory(units)

set(G_OBJECTS_H
	${CMAKE_CURRENT_SOURCE_DIR}/damage.h
	${CMAKE_CURRENT_SOURCE_DIR}/objectcontroller.h
	${CMAKE_CURRENT_SOURCE_DIR}/units/unit_camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/weapons.h
	${CMAKE_CURRENT_SOURCE_DIR}/yelo_objects.h
	PARENT_SCOPE)

set(G_OBJECTS_CXX
	${CMAKE_CURRENT_SOURCE_DIR}/damage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/objectcontroller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/yelo_objects.cpp
	${UNITS_SRC}
	PARENT_SCOPE)
