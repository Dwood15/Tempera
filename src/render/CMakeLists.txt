add_subdirectory(rasterizer)

set(RENDER_H
	${RASTERIZER_H}
#	${CMAKE_CURRENT_SOURCE_DIR}/cameras.hpp
#	${CMAKE_CURRENT_SOURCE_DIR}/c_sky_manager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/constants.h
#	${CMAKE_CURRENT_SOURCE_DIR}/objects.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/render.h
	PARENT_SCOPE)

set(RENDER_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/render.cpp
	PARENT_SCOPE)
