#pragma once

#include <precompile.h>

namespace Enums {
	enum ai_communication_type : short {
		_ai_communication_type_death,
		_ai_communication_type_killing_spree,
		_ai_communication_type_hurt,
		_ai_communication_type_damage,
		_ai_communication_type_sighted_enemy,
		_ai_communication_type_found_enemy,
		_ai_communication_type_unexpected_enemy,
		_ai_communication_type_found_dead_friend,
		_ai_communication_type_allegiance_changed,
		_ai_communication_type_grenade_throwing,
		_ai_communication_type_grenade_startle,
		_ai_communication_type_grenade_sighted,
		_ai_communication_type_grenade_danger,
		_ai_communication_type_lost_contact,
		_ai_communication_type_blocked,
		_ai_communication_type_alert_noncombat,
		_ai_communication_type_search_start,
		_ai_communication_type_search_query,
		_ai_communication_type_search_report,
		_ai_communication_type_search_abandon,
		_ai_communication_type_search_group_abandon,
		_ai_communication_type_uncover_start,
		_ai_communication_type_advance,
		_ai_communication_type_retreat,
		_ai_communication_type_cover,
		_ai_communication_type_sighted_friend_player,
		_ai_communication_type_shooting,
		_ai_communication_type_shooting_vehicle,
		_ai_communication_type_shooting_berserk,
		_ai_communication_type_shooting_group,
		_ai_communication_type_shooting_traitor,
		_ai_communication_type_flee,
		_ai_communication_type_flee_leader_died,
		_ai_communication_type_flee_idle,
		_ai_communication_type_attempted_flee,
		_ai_communication_type_hiding_finished,
		_ai_communication_type_vehicle_entry,
		_ai_communication_type_vehicle_exit,
		_ai_communication_type_vehicle_woohoo,
		_ai_communication_type_vehicle_scared,
		_ai_communication_type_vehicle_falling,
		_ai_communication_type_surprise,
		_ai_communication_type_berserk,
		_ai_communication_type_melee,
		_ai_communication_type_dive,
		_ai_communication_type_uncover_exclamation,
		_ai_communication_type_falling,
		_ai_communication_type_leap,
		_ai_communication_type_postcombat_alone,
		_ai_communication_type_postcombat_unscathed,
		_ai_communication_type_postcombat_wounded,
		_ai_communication_type_postcombat_massacre,
		_ai_communication_type_postcombat_triumph,
		_ai_communication_type_postcombat_check_enemy,
		_ai_communication_type_postcombat_check_friend,
		_ai_communication_type_postcombat_shoot_corpse,
		_ai_communication_type_postcombat_celebrate,

		_ai_communication_type,
	};

	enum ai_communication_priority {
		_ai_communication_priority_none,
		_ai_communication_priority_filler,
		_ai_communication_priority_chatter,
		_ai_communication_priority_talk,
		_ai_communication_priority_communicate,
		_ai_communication_priority_shout,
		_ai_communication_priority_yell,
		_ai_communication_priority_exclaim,

		_ai_communication_priority,
	};

	enum //ai_communication_team
	{
		_ai_communication_team_unteamed = NONE,
		_ai_communication_team_human    = 0,
		_ai_communication_covenant,

		k_number_of_ai_communication_teams
	};

	enum {
		_ai_communication_hostility_type_none,
		_ai_communication_hostility_type_self,
		_ai_communication_hostility_type_friend,
		_ai_communication_hostility_type_enemy,
		_ai_communication_hostility_type_traitor,

		k_number_of_ai_communication_hostility_types
	};
	enum {
		_ai_communication_status_type_never,
		_ai_communication_status_type_dead,
		_ai_communication_status_type_lost,
		_ai_communication_status_type_not_visual,
		_ai_communication_status_type_no_danger,
		_ai_communication_status_type_visual,

		k_number_of_ai_communication_status_types,
	};
};

namespace AI {
	struct s_ai_communication_event : TStructImpl(16) {
};
typedef s_ai_communication_event ai_communication_dialogue_events_t[105];
typedef s_ai_communication_event ai_communication_reply_events_t[46];

struct s_ai_conversation_datum : TStructImpl(100)
{
};
typedef Memory::DataArray<s_ai_conversation_datum, 8>
	ai_conversation_data_t;

struct s_ai_communication_packet {
	UNKNOWN_TYPE(long);
	UNKNOWN_TYPE(short); // 4
	Enums::ai_communication_type dialogue_type_index; // 6
	UNKNOWN_TYPE(short); // 8
	unsigned short : 16; // A ?

	UNKNOWN_TYPE(short); // C
	unsigned short : 16; // E ?
	unsigned long : 32; // 10 ?
	UNKNOWN_TYPE(short); // 14
	unsigned short : 16; // 16 ?
										  UNKNOWN_TYPE(short); // 18
										  UNKNOWN_TYPE(short); // 1A
	bool                         broken; // 1C false = reformed
	unsigned char : 8; unsigned short : 16;
}; static_assert(sizeof(s_ai_communication_packet) == 0x20);
};
