#pragma once
#include <enums/weapon_enums.h>
#include "../../math/real_math.h"
#include "../objects.h"

struct s_weapon_datum_network_data {
	real_point3d  position;
	real_vector3d transitional_velocity;
	int           a;
	int           b;
	int           c; // not used in the update...probably a real_vector3d (angular_velocity?)
	short         magazine_rounds_totals[MAX_MAGAZINES_PER_WEAPON];
	real          age;
}; static_assert(sizeof(s_weapon_datum_network_data) == 0x2C);

struct s_trigger_state {
	sbyte                idle_time;                    // 0x0 used for determining when to fire next projectile (rounds per second)
	weapon_trigger_state state;
	short                time;
	long                 flags0;            // 0x4
	__int16              __unk_type_i16_0;                // 0x8 firing effect related
	__int16              __unk_type_i16_1;                // 0xA firing effect related
	__int16              __unk_type_i16_2;                // 0xC firing effect related
	short                rounds_since_last_tracer;
	float                rate_of_fire;                    // 0x10
	float                ejection_port_recovery_time;    // 0x14
	float                illumination_recovery_time;    // 0x18
	float                __unk_type_flt_0;    // 0x1C used in the calculation of projectile error angle
	datum_index          charging_effect_index;    // 0x20
	sbyte                network_delay_time;            // 0x24 hardedcoded to delay fire/reload by 10 frames in networked game
	byte                 __pad_byte_0;
	__int16              __pad_i16_0;
};
static_assert(sizeof(s_trigger_state) == 0x28);

// __, unk, pad, '?'
//means IDK if its actually padding or there are values there. If there are, IDK their types (could be a boolean!)
struct s_magazine_state {
	weapon_magazine_state state;
	__int16               reload_time_remaining;        // 0x2 in ticks
	__int16               reload_time;                    // 0x4 in ticks
	__int16               rounds_unloaded;                // 0x6
	__int16               rounds_loaded;                // 0x8
	__int16               rounds_left_to_recharge;        // 0xA number of rounds left to apply to rounds_loaded (based on tag's rounds_recharged)
	__int16               __UNKNOWN_TYPE0;                // 0xC I just know a WORD is here, may be an _enum
	__int16               __pad0; // ?
}; static_assert(sizeof(s_magazine_state) == 0x10);

struct s_weapon_data {
	struct s_start_reload_data {
		__int16 starting_total_rounds[MAX_MAGAZINES_PER_WEAPON];
		__int16 starting_loaded_rounds[MAX_MAGAZINES_PER_WEAPON];
	}; static_assert(sizeof(s_start_reload_data) == 0x8);

	struct s_first_16_bytes {
		unsigned long  flags;                            // 0x0
		unsigned short owner_unit_flags;                // 0x4
		__int16        __pad_i16_1;                    // 0x6
		real           primary_trigger;                // 0x8
		weapon_state   weapon_state;                    // 0xC	//weap state is supposed to be the size of a byte
		unsigned char  __pad_byte_1;                    // 0xD
		short          ready_time;                        // 0xE
	} first_16_bytes;

	static_assert(sizeof(s_first_16_bytes) == 0x10);
	struct s_2nd_16_bytes {
		real heat;                            // 0x0
		real age;                            // 0x4
		real illumination_fraction;            // 0x8
		real integrated_light_power;            // 0xC
	}
	second_16_bytes;

	static_assert(sizeof(s_2nd_16_bytes) == 0x10);

	struct s_3rd_16_bytes {
		int         __unused_pad0;                    // 0x0
		datum_index tracked_object_index;            // 0x4
		long long     __pad64_0;                        // 0x8
	}

	third_16_bytes;

	STAT_ASSERT(s_3rd_16_bytes, 0x10);

	struct next_set {
		short   alt_shots_loaded;                // 0x0
		__int16 __pad_i16_2;                    // 0x2
	}

	first_4_bytes;

	STAT_ASSERT(next_set, 0x4);

	s_trigger_state triggers[2];                    // 0x34, size == 0x50

	//STAT_ASSERT(triggers, sizeof(s_trigger_state) * 2);
	//INTELLISENSE_HACK(triggers);


	s_magazine_state magazines[2];                    //0x84
	INTELLISENSE_HACK(magazines);

	long                        last_trigger_fire_time;                        // 0xA0
	s_start_reload_data         start_reload_update;            // 0xA4
	long                        __pad32_1;                        // 0xAC need to verify this is unused
	bool                        baseline_valid;                                // 0xB0
	sbyte                       baseline_index;                                // 0xB1
	sbyte                       message_index;                                // 0xB2
	byte                        __pad8_4;                                        // 0xB3
	s_weapon_datum_network_data update_baseline;        // 0xB4
	bool                        __unk_char_type;                                // 0xDC probably delta_valid
	char                        _pad_c_byte[3];                                    // 0xDD
	s_weapon_datum_network_data update_delta;            // 0xE0
}; ///This stupid structure EFFFFFFFFFFFFFF

INTELLISENSE_HACK(s_weapon_datum_network_data)

//STAT_ASSERT(s_weapon_data, (k_object_size_weapon - k_object_size_item));
//static_assert(sizeof(s_weapon_data) == ());
/* OG struct:
 struct s_weapon_data {
			// FLAG(3) - _weapon_must_be_readied_bit
			long_flags flags;						// 0x22C
			word_flags owner_unit_flags;			// 0x230
			short : 16;
			real primary_trigger;					// 0x234
			Enums::weapon_state weapon_state;		// 0x238
			PAD8;
			int16 ready_time;						// 0x23A in ticks
			real heat;								// 0x23C
			real age;								// 0x240
			real illumination_fraction;				// 0x244
			real integrated_light_power;			// 0x248
			PAD32;									// 0x24C unused
			datum_index tracked_object_index;		// 0x250
			PAD64;									// 0x254 unused
			int16 alt_shots_loaded;					// 0x25C
			short : 16;									// 0x25E
			s_trigger_state triggers[Enums::k_maximum_number_of_triggers_per_weapon];	// 0x260
			s_magazine_state magazines[Enums::k_maximum_number_of_magazines_per_weapon];// 0x2B0
			int32 last_trigger_fire_time;			// 0x2D0
			s_start_reload_data start_reload_update;// 0x2D4
			PAD32; // 0x2DC need to verify this is unused
			bool baseline_valid;					// 0x2E0
			sbyte baseline_index;					// 0x2E1
			sbyte message_index;					// 0x2E2
			PAD8;									// 0x2E3
			s_weapon_datum_network_data update_baseline;	// 0x2E4
			UNKNOWN_TYPE(bool);						// 0x310 probably delta_valid
			PAD24;									// 0x311
			s_weapon_datum_network_data update_delta;		// 0x314
		}; static_assert(sizeof(s_weapon_data) == (Enums::k_object_size_weapon - Enums::k_object_size_item));
 */

struct s_weapon_datum : s_item_datum {
	enum {
		k_object_types_mask = FLAG(_object_type_weapon)
	};
	s_weapon_data weapon;
}; //static_assert(sizeof(s_weapon_datum) == k_object_size_weapon);


struct weapon_data {
	object_data _object;
	char Unknown5[72];
	long attacking;            // some sort of bitfield: 0x10 when used in a melee, 0x02 when shooting (at least for plasma, i think the 2 is for automatic.)
	char Unknown1[8];
	float heat;
	float age_of_battery;    // (1.0 - battery left) not sure why it's like that.
	char Unknown6[36];
	short toggle1;
	short toggle2;            // these 2 seem to toggle something on the weapon. From 3,0 it switches to 2,1 and then back. (At least for plasma rifle, and I noticed that the fire effect alternates to each tip of the 'claw' of the rifle)
	char Unknown7[70];
	short Reloading;        // 1=reloading
	short reload_time;        // countdown until reload is complete.
	short reserve_ammo;
	short current_ammo;
	char Unknown8[78];
	short reload_ammo;        // matches reserve_ammo after a reload or when you switch weapons, but not after collecting ammo.
	char Unknown9[518];
};
