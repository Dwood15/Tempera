
set(A_ACTIONS_H
	${CMAKE_CURRENT_SOURCE_DIR}/actions/actions.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_alert.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_avoid.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_charge.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_wait.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_vehicle.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_uncover.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_sleep.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_search.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_obey.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_guard.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_flee.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_fight.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/actions/action_converse.hpp)

set(A_ACTORS_H
	${CMAKE_CURRENT_SOURCE_DIR}/actors/actor_definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/actors/actor_looking.h
	${CMAKE_CURRENT_SOURCE_DIR}/actors/actor_moving.h
	${CMAKE_CURRENT_SOURCE_DIR}/actors/actor_perception.h
	${CMAKE_CURRENT_SOURCE_DIR}/actors/actor_stimulus.h
	${CMAKE_CURRENT_SOURCE_DIR}/actors/actor_structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/actors/actors.h
	${CMAKE_CURRENT_SOURCE_DIR}/actors/tranform_manager.h)

set(AI_CXX
	PARENT_SCOPE)

set(AI_H
	${A_ACTIONS_H}
	${A_ACTORS_H}
	${CMAKE_CURRENT_SOURCE_DIR}/ai.h
	${CMAKE_CURRENT_SOURCE_DIR}/ai_communication.h
	${CMAKE_CURRENT_SOURCE_DIR}/ai_scenario_definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/ai_script.h
	${CMAKE_CURRENT_SOURCE_DIR}/ai_structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/encounters.h
	${CMAKE_CURRENT_SOURCE_DIR}/path.h
	${CMAKE_CURRENT_SOURCE_DIR}/prop_structures.h
	${CMAKE_CURRENT_SOURCE_DIR}/props.h
	PARENT_SCOPE)
