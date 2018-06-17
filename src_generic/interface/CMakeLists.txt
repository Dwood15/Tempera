
set(I_EDITOR_H
	${CMAKE_CURRENT_SOURCE_DIR}/editor/field_dialog_structures.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/editor/tags_interface.hpp
	)

set(I_HUD_H
	${CMAKE_CURRENT_SOURCE_DIR}/hud/equipment_hud_interface_definition.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/first_person_weapons.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/first_person_weapons_structures.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_chat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_definitions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_draw.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_messaging_structures.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_messaging_definitions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_messaging.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_nav_points.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_unit.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_unit_structures.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_weapon_structures.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/hud_weapon.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/motion_sensor.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/unit_hud_interface_definition.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/hud/weapon_hud_interface_definition.hpp
	)

set(I_INPUT_H
	${CMAKE_CURRENT_SOURCE_DIR}/input/abstraction.h
	${CMAKE_CURRENT_SOURCE_DIR}/input/player_control.h
	${CMAKE_CURRENT_SOURCE_DIR}/input/window.h
	)

set(I_STRINGS_H
	${CMAKE_CURRENT_SOURCE_DIR}/strings/draw_string.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/strings/text_group.hpp
	)

set(I_WIDGETS_H
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/ui.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/ui_event_handler_functions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/ui_game_data_input_functions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/ui_group.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/ui_text_search_and_replace_functions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/widgets/widgets.hpp
	)

set(I_CAMERA_H
	${CMAKE_CURRENT_SOURCE_DIR}/camera/camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/camera/dead.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera/definitions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera/director.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera/director_structures.hpp
	)


set(INTERFACE_H
	${I_EDITOR_H}
	${I_HUD_H}
	${I_INPUT_H}
	${I_STRINGS_H}
	${I_WIDGETS_H}
	${CMAKE_CURRENT_SOURCE_DIR}/interface.hpp
	${I_CAMERA_H}
	PARENT_SCOPE)
