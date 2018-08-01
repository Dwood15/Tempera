
#pragma once

#include <precompile.h>

namespace Yelo
{
	namespace AI
	{
		struct s_actor_meta_data
		{
			short type;
			bool swarm;
			unsigned char : 8; // unknown field
			bool active;
			bool encounterless;
			unsigned char : 8; // unknown field
			unsigned char : 8;
			unsigned long : 32; // unknown field
			unsigned short : 16; // unknown field
			unsigned char : 8; // unknown field
			bool dormant;
			unsigned short : 16; // unknown field
			unsigned short : 16;
			datum_index unit_index;
			unsigned char : 8; // unknown field
			unsigned char : 8;
			short swarm_unit_count;
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			datum_index swarm_unit_index;
			datum_index swarm_cache_index;
			datum_index encounter_next_actor_index;
			unsigned long : 32; // unknown field
			datum_index encounter_index;
			unsigned short : 16; // unknown field
			short squad_index;
			short platoon_index;
			Enums::game_team team;
			unsigned char : 8; // unknown field
			unsigned char : 8; unsigned short : 16;
			unsigned long : 32; // unknown field
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			bool timeslice;
			unsigned char : 8; unsigned short : 16;
			datum_index first_prop_index;
			unsigned long : 32; // unknown field
			datum_index actor_definition_index;
			datum_index actor_variant_definition_index;
		}; static_assert( sizeof(s_actor_meta_data) == 0x5C );

		struct s_actor_state_data
		{
			Enums::actor_action action;
			unsigned short : 16; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8;
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			PAD(1, 14);
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8;
			unsigned short : 16; // unknown field
			unsigned short : 16;
			unsigned long : 32; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned short : 16;
			byte action_data[132];
		}; static_assert( sizeof(s_actor_state_data) == 0xB4 );

		struct s_actor_input_data
		{
			PAD_TYPE(real_point3d); // unknown field
			PAD_TYPE(real_vector3d); // unknown field
			PAD(1, 32);
			datum_index vehicle_index;
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned short : 16; // unknown field
			bool vehicle_passenger;
			bool vehicle_driver;
			unsigned short : 16;
			long surface_index;
			real_point3d position;
			real_vector3d facing_vector;
			real_vector3d aiming_vector;
			real_vector3d looking_vector;
			PAD(2, 24);
			unsigned long : 32; // unknown field
			unsigned char : 8; // unknown field
			bool burning_to_death;
			unsigned short : 16;
			real unit_damage_body;
			real unit_damage_shield;
			real unit_damage_body_recent;
			real unit_damage_shield_recent;
		}; static_assert( sizeof(s_actor_input_data) == 0xA8 );

		struct s_actor_target_data
		{
			unsigned short : 16; // unknown field
			unsigned short : 16;
			unsigned long : 32; // unknown field
			datum_index target_prop_index;
			unsigned char : 8; // unknown field
			unsigned char : 8; unsigned short : 16;
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8;
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
		}; static_assert( sizeof(s_actor_target_data) == 0x18 );

		struct s_actor_danger_zone_data
		{
			PAD(1, 104);
			unsigned short : 16; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
		}; static_assert( sizeof(s_actor_danger_zone_data) == 0x6C );

		struct s_actor_stimulus_data
		{
			unsigned char : 8; // unknown field
			unsigned char : 8;
			unsigned short : 16; // unknown field
			unsigned long : 32;
			unsigned long : 32; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; unsigned short : 16;
			PAD_TYPE(real_vector3d); // unknown field
			unsigned short : 16; // unknown field
			unsigned short : 16;
			unsigned long : 32; // unknown field
			unsigned short : 16; // unknown field
			short combat_transition;
			unsigned char : 8; // unknown field
			unsigned char : 8; unsigned short : 16;
			PAD_TYPE(real_vector3d); // unknown field
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; unsigned short : 16;
			PAD_TYPE(real_vector3d); // unknown field
			unsigned long : 32; // unknown field
			datum_index combat_transition_prop_index;
			unsigned long : 32; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8;
			unsigned short : 16; // unknown field
			unsigned long : 32; // unknown field
		}; static_assert( sizeof(s_actor_stimulus_data) == 0x64 );

		struct s_actor_emotion_data
		{
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned short : 16; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			short crouch_switching_change_timer;
			unsigned short : 16; // unknown field
			unsigned short : 16; // unknown field
			unsigned short : 16;
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			bool ignorant;
			unsigned char : 8; // unknown field
			bool berserking;
			unsigned char : 8; // unknown field
			unsigned short : 16;
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; unsigned short : 16;
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned short : 16; // unknown field
			unsigned short : 16;
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
			unsigned long : 32; // unknown field
		}; static_assert( sizeof(s_actor_emotion_data) == 0x68 );

		struct s_actor_firing_position_data
		{
			unsigned short : 16; // unknown field
			PAD(1, 6);
			unsigned long : 32; // unknown field
			PAD(2, 32); // unknown field
			unsigned short : 16; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
		}; static_assert( sizeof(s_actor_firing_position_data) == 0x30 );

		struct s_actor_orders_data
		{
			PAD(1, 128); // unknown field
			unsigned short : 16; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
		}; static_assert( sizeof(s_actor_orders_data) == 0x84 );

		struct s_path
		{
			bool valid;															//	0x0
			PAD(1, 3);															//	0x1
			byte destination[20];												//	0x4
			unsigned char : 8; // unknown field												//	0x18
			unsigned char : 8; // unknown field												//	0x19
			unsigned char : 8; // unknown field												//	0x1A
			unsigned char : 8;
			PAD(2, 16);															//	0x1C
			PAD(3, 16);															//	0x2C
			PAD(4, 16);															//	0x3C
			PAD(5, 16);															//	0x4C
		}; static_assert( sizeof(s_path) == 0x5C );

		struct s_actor_control_direction_specification
		{
			short type;															//	0x0
			unsigned short : 16;
			datum_index prop_index;												//	0x4
			PAD(1, 6);
			unsigned char : 8; // unknown field												//	0xE
			unsigned char : 8; // unknown field												//	0xF
		}; static_assert( sizeof(s_actor_control_direction_specification) == 0x10 );

		struct s_actor_control_data
		{
			unsigned short : 16; // unknown field												//	0x0
			unsigned short : 16;
			unsigned long : 32; // unknown field												//	0x4
			PAD(1, 12);

			struct
			{
				struct
				{
					datum_index	ignore_target_object_index;						//	0x14
					bool at_destination;										//	0x18
					unsigned char : 8; unsigned short : 16;
				} destination_orders;

				PAD_TYPE(real_vector3d); // unknown field						//	0x1C
				unsigned long : 32; // unknown field											//	0x28
				unsigned long : 32; // unknown field											//	0x2C
				unsigned long : 32;
				unsigned long : 32; // unknown field											//	0x34
				unsigned char : 8; // unknown field											//	0x38
				unsigned char : 8; unsigned short : 16;
				s_path path;													//	0x3C
			} path;

			unsigned char : 8; // unknown field												//	0x98
			unsigned char : 8; // unknown field												//	0x99
			unsigned char : 8; // unknown field												//	0x9A
			unsigned char : 8; // unknown field												//	0x9B
			unsigned char : 8; // unknown field												//	0x9C
			unsigned char : 8;
			unsigned short : 16; // unknown field												//	0x9E
			PAD_TYPE(real_point3d); // unknown field							//	0xA0
			PAD_TYPE(real_vector3d); // unknown field							//	0xAC
			PAD(2, 12);
			unsigned char : 8; // unknown field												//	0xC4
			unsigned char : 8; unsigned short : 16;
			unsigned long : 32; // unknown field												//	0xC8
			unsigned long : 32; // unknown field												//	0xCC
			unsigned long : 32; // unknown field												//	0xD0
			unsigned long : 32; // unknown field												//	0xD4
			short secondary_look_type;											//	0xD8
			unsigned short : 16;
			unsigned short : 16; // unknown field												//	0xDC
			unsigned short : 16;
			s_actor_control_direction_specification secondary_look_direction;	//	0xE0
			bool idle_major_active;												//	0xF0
			unsigned char : 8; // unknown field												//	0xF1
			unsigned char : 8; // unknown field												//	0xF2
			bool idle_minor_active;												//	0xF3
			unsigned long : 32; // unknown field												//	0xF4
			long idle_major_timer;												//	0xF8
			long idle_minor_timer;												//	0xFC
			s_actor_control_direction_specification idle_major_direction;		//	0x100
			s_actor_control_direction_specification idle_minor_direction;		//	0x110
			unsigned char : 8; // unknown field												//	0x120
			unsigned char : 8; // unknown field												//	0x121
			unsigned char : 8; // unknown field												//	0x122
			unsigned char : 8; // unknown field												//	0x123
			unsigned char : 8; // unknown field												//	0x124
			unsigned char : 8; // unknown field												//	0x125
			PAD(3, 6);
			PAD_TYPE(real_vector3d); // unknown field							//	0x12C
			real_vector3d  desired_facing_vector;								//	0x138
			PAD_TYPE(real_vector3d); // unknown field							//	0x144
			PAD_TYPE(real_vector3d); // unknown field							//	0x150
			PAD(4, 16); // unknown field										//	0x15C
			unsigned short : 16; // unknown field												//	0x16C
			unsigned short : 16;
			PAD_TYPE(real_vector3d); // unknown field							//	0x170
			PAD(5, 8);
			unsigned short : 16; // unknown field												//	0x184
			short fire_state;													//	0x186
			unsigned short : 16; // unknown field												//	0x188
			unsigned short : 16; // unknown field												//	0x18A
			unsigned short : 16; // unknown field												//	0x18C
			unsigned short : 16; // unknown field												//	0x18E
			unsigned short : 16; // unknown field												//	0x190
			unsigned short : 16; // unknown field												//	0x192
			unsigned char : 8; // unknown field												//	0x194
			unsigned char : 8; // unknown field												//	0x195
			unsigned char : 8; // unknown field												//	0x196
			unsigned char : 8; // unknown field												//	0x197
			unsigned char : 8; // unknown field												//	0x198
			unsigned char : 8; unsigned short : 16;
			unsigned long : 32; // unknown field												//	0x19C
			short current_fire_target_type;										//	0x1A0
			unsigned short : 16;
			datum_index	current_fire_target_prop_index;							//	0x1A4
			PAD(6, 8);
			unsigned long : 32; // unknown field												//	0x1B0
			unsigned char : 8; // unknown field												//	0x1B4
			unsigned char : 8; // unknown field												//	0x1B5
			unsigned char : 8; // unknown field												//	0x1B6
			unsigned char : 8; // unknown field												//	0x1B7
			unsigned char : 8; // unknown field												//	0x1B8
			unsigned char : 8;
			unsigned short : 16; // unknown field												//	0x1BA
			unsigned char : 8; // unknown field												//	0x1BC
			unsigned char : 8; unsigned short : 16;
			PAD_TYPE(real_vector3d); // unknown field							//	0x1C0
			unsigned long : 32; // unknown field												//	0x1CC
			real_vector3d  fire_target_aim_vector;								//	0x1D0
			unsigned long : 32; // unknown field												//	0x1DC
			PAD_TYPE(real_vector3d); // unknown field							//	0x1E0
			PAD_TYPE(real_point3d); // unknown field							//	0x1EC
			PAD(7, 12);
			PAD_TYPE(real_point3d); // unknown field							//	0x204
			PAD_TYPE(real_point3d); // unknown field							//	0x210
			unsigned char : 8; // unknown field												//	0x21C
			unsigned char : 8; unsigned short : 16;
			real_vector3d  burst_aim_vector;									//	0x220
			PAD(8, 8);
			unsigned char : 8; // unknown field												//	0x234
			unsigned char : 8; unsigned short : 16;
			unsigned long : 32; // unknown field												//	0x238
			PAD_TYPE(real_vector3d); // unknown field							//	0x23C
			unsigned long : 32; // unknown field												//	0x248
			PAD(9, 20);
			unsigned char : 8; // unknown field												//	0x260
			unsigned char : 8;
			unsigned short : 16; // unknown field												//	0x262
			struct
			{
				unsigned short	control_flags;										//	0x264
				unsigned short : 16;
				short	persistent_control_ticks;								//	0x268
				unsigned short : 16;
				long_flags	persistent_control_flags;							//	0x26C
				unsigned short : 16; // unknown field											//	0x270
				unsigned short : 16;
				real_vector3d  throttle;										//	0x274
				unsigned short : 16; // unknown field											//	0x280
				unsigned short : 16;
				PAD_TYPE(real_point2d); // unknown field						//	0x284
				sbyte	aiming_speed;											//	0x28C
				unsigned char : 8; unsigned short : 16;
				real_vector3d  facing_vector;									//	0x290
				real_vector3d  aiming_vector;									//	0x29C
				real_vector3d  looking_vector;									//	0x2A8
				datum_index	primary_trigger;									//	0x2B4
			} output;
		}; static_assert( sizeof(s_actor_control_data) == 0x2B8 );

		struct s_actor_datum
		{
			unsigned short : 16; // unknown field
			unsigned short : 16;
			s_actor_meta_data meta;
			Enums::actor_default_state initial_state;
			Enums::actor_default_state return_state;
			unsigned long : 32; // unknown field
			unsigned short : 16;
			Enums::actor_default_state current_state;
			s_actor_state_data state;
			s_actor_input_data input;
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8; // unknown field
			unsigned char : 8;
			unsigned char : 8; // unknown field
			unsigned char : 8; unsigned short : 16;
			unsigned long : 32; // unknown field
			unsigned short : 16; // unknown field
			unsigned long : 32;unsigned short : 16;
			unsigned long : 32; // unknown field
			PAD_TYPE(datum_index); // unknown field
			short postcombat_state;
			unsigned short : 16;
			unsigned long : 32; // unknown field
			byte situation[123];
			unsigned char : 8; // unknown field
			s_actor_target_data target;
			s_actor_danger_zone_data danger_zone;
			s_actor_stimulus_data stimulus;
			s_actor_emotion_data emotions;
			s_actor_firing_position_data firing_positions;
			s_actor_orders_data orders;
			s_actor_control_data control;
		}; static_assert( sizeof(s_actor_datum) == 0x724 );

		typedef Memory::DataArray<s_actor_datum, 256> actor_data_t;

		struct s_swarm_datum : Memory::s_datum_base
		{
			short unit_count;			// 0x2
			datum_index actor_index;	// 0x4
			UNKNOWN_TYPE(short);		// 0x8
			unsigned short : 16;
			UNKNOWN_TYPE(real_vector3d);// 0xC
			datum_index unit_indices[Enums::k_maximum_number_of_units_per_swarm];
			datum_index component_indices[Enums::k_maximum_number_of_units_per_swarm];
		}; static_assert( sizeof(s_swarm_datum) == 0x98 );

		typedef Memory::DataArray<	s_swarm_datum, Enums::k_maximum_number_of_active_swarms> swarm_data_t;

		struct s_swarm_component_datum : TStructImpl(64)
		{
		};

		typedef Memory::DataArray<s_swarm_component_datum, 256> swarm_component_data_t;
		
		actor_data_t&					Actors();

		swarm_data_t&					Swarms();

		swarm_component_data_t&			SwarmComponents();
	};
};
