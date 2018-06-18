set(GAMEMAP_H
	${CMAKE_CURRENT_SOURCE_DIR}/dinput/dinput.h
	${CMAKE_CURRENT_SOURCE_DIR}/enums/ControlTypes.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/colors.h
	${CMAKE_CURRENT_SOURCE_DIR}/players_loop_related
	${CMAKE_CURRENT_SOURCE_DIR}/headers/hce_addresses.h
	)

set(GAMESTATE_H
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/objects.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/halo_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/scenario.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/game_input.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/controls.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/player_controller_to_index_array.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/render_window.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/core.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/forge.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/objectcontroller.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/players.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/sbsp.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/networking.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/game_globals.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/render.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/player_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/tags_generic.h
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/hud_related.h
	)

set(GAMESTATE_CPP
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/core.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/math.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/controls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gamestate/objectcontroller.cpp
	)

set(TAGS_H
	${CMAKE_CURRENT_SOURCE_DIR}/tags/bsp.h
	${CMAKE_CURRENT_SOURCE_DIR}/tags/map.h
	${CMAKE_CURRENT_SOURCE_DIR}/tags/scenario.h
	${CMAKE_CURRENT_SOURCE_DIR}/tags/tags.h
	)

set(HALOFORGE_H
	${CMAKE_CURRENT_SOURCE_DIR}/extended/addlog.h

	${CMAKE_CURRENT_SOURCE_DIR}/Direct3D/textures.h
	${CMAKE_CURRENT_SOURCE_DIR}/Direct3D/d3d9hook.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/weapons.h)

set(CEINTERNAL_H
	${TAGS_H}
	${GAMEMAP_H}
	${HALOFORGE_H}
	${CMAKE_CURRENT_SOURCE_DIR}/headers/real_math.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/int_math.h
	${CMAKE_CURRENT_SOURCE_DIR}/ce_base_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/enums_generic.h
	${CMAKE_CURRENT_SOURCE_DIR}/exceptions/exception_handler.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/animations.h
	${CMAKE_CURRENT_SOURCE_DIR}/lua/script_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/lua/memory_interface.h
	)

set(HALOFORGE_CPP
	${CMAKE_CURRENT_SOURCE_DIR}/Direct3D/d3d9hook.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/extended/addlog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/forge.cpp
	)

add_subdirectory(interface)

set(CEINTERNAL_CPP
	${CEINTERNAL_H}
	${GLOBAL_H}
	${GAMESTATE_H}
	${GAMESTATE_CPP}
	${HALOFORGE_CPP}
	#	${INTERFACE_H}
	${CMAKE_CURRENT_SOURCE_DIR}/function_rewrite.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/exceptions/exception_handler.cpp

	PARENT_SCOPE)
