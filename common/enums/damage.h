#pragma once
namespace Yelo::Enums {
	enum damage_side_effect {
		_damage_side_effect_none,
		_damage_side_effect_harmless,
		_damage_side_effect_legthal_to_unsuspecting,
		_damage_side_effect_emp,

		_damage_side_effect,
	};

	enum damage_category {
		_damage_category_none,
		_damage_category_falling,
		_damage_category_bullet,
		_damage_category_grenade,
		_damage_category_high_explosive,
		_damage_category_sniper,
		_damage_category_melee,
		_damage_category_flame,
		_damage_category_mounted_weapon,
		_damage_category_vehicle,
		_damage_category_plasma,
		_damage_category_needle,
		_damage_category_shotgun,

		_damage_category
	};
};
