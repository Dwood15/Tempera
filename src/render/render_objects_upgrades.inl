/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace render_objects_mods
{
#if PLATFORM_IS_USER
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Queries whether an object should cast a shadow. </summary>
	///
	/// <param name="object_definition">	The object definition. </param>
	///
	/// <returns>	True if the object should cast shadows. </returns>
	static bool PLATFORM_API QueryObjectCastsShadow(const TagGroups::s_object_definition* object_definition)
	{
		bool cast_shadow = false;
		switch(object_definition->object.runtime_object_type)
		{
			case Enums::_object_type_scenery:
			case Enums::_object_type_machine:
			case Enums::_object_type_control:
			case Enums::_object_type_light_fixture:
			case Enums::_object_type_placeholder:
			case Enums::_object_type_sound_scenery:
				cast_shadow = TEST_FLAG(object_definition->object.flags, Flags::_object_definition_cast_shadow_by_default_yelo_bit);
				break;
			default:
				cast_shadow = true;
		}

		return (cast_shadow && !TEST_FLAG(object_definition->object.flags, Flags::_object_definition_does_not_cast_shadow_bit));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Hook for object_new that sets the shadow casting flag. </summary>
	static __declspec(naked) void ObjectNewSetShadowFlag_Hook()
	{
		static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(OBJECT_NEW_SET_SHADOW_FLAG_RETN);

		_asm
		{
			push	eax
			push	edx

			push	edi
			call	QueryObjectCastsShadow
			add		esp, 4

			test	al, al
			jnz		cast_shadows
			or		dword ptr [esi+10h], 40000h

		cast_shadows:
			pop		edx
			pop		eax

			jmp		RETN_ADDRESS
		};
	}
#endif

	static void Initialize()
	{
		static datum_index rendered_objects[Enums::k_maximum_rendered_objects_upgrade];

		for(auto ptr : K_MAXIMUM_RENDERED_OBJECTS_REFS_32bit)
			*CAST_PTR(int32*,ptr) = Enums::k_maximum_rendered_objects_upgrade;
		for(auto ptr : K_MAXIMUM_RENDERED_OBJECTS_REFS_16bit)
			*CAST_PTR(int16*,ptr) = Enums::k_maximum_rendered_objects_upgrade;

		for(auto ptr : K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REFS)
			*CAST_PTR(datum_index**,ptr) = rendered_objects;

		Memory::WriteRelativeJmp(&ObjectNewSetShadowFlag_Hook, GET_FUNC_VPTR(OBJECT_NEW_SET_SHADOW_FLAG_HOOK), true);

		// Nop-out the instructions that disable shadows for devices, scenery and sound scenery
		for(auto ptr : K_OBJECT_DISABLE_SHADOW_CAST_OVERRIDES)
		{
			static const byte k_nop_opcode[] = 
			{
				Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop
			};

			Memory::WriteMemory(ptr, k_nop_opcode);
		}
	}
};
