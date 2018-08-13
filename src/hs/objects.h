namespace Yelo::Objects {
	static void *scripting_objects_distance_to_object_evaluate(void **arguments);

	static void *scripting_object_data_get_real_evaluate(void **arguments);

	static void *scripting_object_data_set_real_evaluate(void **arguments);

	static void *scripting_object_data_set_vector_evaluate(void **arguments);

	static void *scripting_object_data_save_vector_evaluate(void **arguments);

	//////////////////////////////////////////////////////////////////////////
	// WEAPONS
	static void *scripting_weapon_data_get_real_evaluate(void **arguments);

	static void *scripting_weapon_data_set_real_evaluate(void **arguments);

	//////////////////////////////////////////////////////////////////////////
	// WEAPONS - MAGAZINES
	static void *scripting_weapon_data_magazine_get_integer_evaluate(void **arguments);

	static void *scripting_weapon_data_magazine_set_integer_evaluate(void **arguments);
	//////////////////////////////////////////////////////////////////////////
	// WEAPONS - TRIGGERS

	static void *scripting_weapon_data_trigger_set_real_evaluate(void **arguments);

	//////////////////////////////////////////////////////////////////////////
	// UNITS
	static void *scripting_unit_data_get_object_evaluate(void **arguments);

	static void *scripting_unit_data_get_integer_evaluate(void **arguments);

	static void *scripting_unit_data_set_integer_evaluate(void **arguments);

	static void *scripting_unit_data_get_real_evaluate(void **arguments);

	static void *scripting_unit_data_set_real_evaluate(void **arguments);

	static void *scripting_vehicle_remapper_enabled_evaluate(void **arguments);
};
