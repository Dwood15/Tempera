
set(SRC_CXX
	${CMAKE_CURRENT_SOURCE_DIR}/tags/group/base.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/tags/group/groups_structures.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tags/group/tag_groups.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/tags/group/c_tag_iterator.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/units/units.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/objects.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/objects/objectcontroller.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/controls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/players.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/lua/script_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Direct3D/d3d9hook.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/cseries/random.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Dinput/dinput.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/engine_functions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CurrentEngine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/forge.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/math/math.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/exceptions/exception_handler.cpp
	PARENT_SCOPE)
