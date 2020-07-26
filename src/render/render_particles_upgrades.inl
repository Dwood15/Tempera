/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace render_particles_mods
{
	struct s_rendered_particle
	{
		int16 particle_index;
		int16 particle_definition_index; // datum's 0x4 field
		int16 particle_datum_field_2C;
		bool particle_datum_flags_bit5;
		PAD8;
	}; BOOST_STATIC_ASSERT( sizeof(s_rendered_particle) == 8 );
	static s_rendered_particle rendered_particles[Enums::k_maximum_number_of_particles_per_map_upgrade];

	//////////////////////////////////////////////////////////////////////////
	// Game client instructions
	NAKED_FUNC_WRITER_ASM_BEGIN(lea_ecx_rendered_particles_mul8)
		lea ecx, rendered_particles[ecx*8]					NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(lea_eax_rendered_particles_mul8)
		lea eax, rendered_particles[eax*8]					NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(lea_ecx_rendered_particles)
		lea ecx, rendered_particles
		nop													NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(lea_eax_rendered_particles_add4)
		lea eax, rendered_particles.particle_datum_field_2C
		nop													NAKED_FUNC_WRITER_ASM_END();
	NAKED_FUNC_WRITER_ASM_BEGIN(lea_eax_rendered_particles)
		lea eax, rendered_particles
		nop													NAKED_FUNC_WRITER_ASM_END();

#if PLATFORM_IS_USER
	static void Initialize()
	{
		Memory::c_naked_func_writer<lea_ecx_rendered_particles_mul8>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[0] );
		Memory::c_naked_func_writer<lea_eax_rendered_particles_mul8>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[1] );
		Memory::c_naked_func_writer<lea_ecx_rendered_particles>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[2] );
		Memory::c_naked_func_writer<lea_eax_rendered_particles_add4>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[3] );
		Memory::c_naked_func_writer<lea_eax_rendered_particles>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[4] );

		*CAST_PTR(size_t*, GET_FUNC_VPTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES)) = 
			Enums::k_maximum_number_of_particles_per_map_upgrade;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Editor instructions

	NAKED_FUNC_WRITER_ASM_BEGIN(lea_ebx_rendered_particles)
		lea ebx, rendered_particles
		nop													NAKED_FUNC_WRITER_ASM_END();

#if PLATFORM_IS_EDITOR
	static void Initialize()
	{
		Memory::c_naked_func_writer<lea_eax_rendered_particles_mul8>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[0] );
		Memory::c_naked_func_writer<lea_ecx_rendered_particles>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[1] );
		Memory::c_naked_func_writer<lea_eax_rendered_particles_add4>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[2] );
		Memory::c_naked_func_writer<lea_ebx_rendered_particles>::
			Write( K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[3] );

		*CAST_PTR(size_t*, GET_FUNC_VPTR(PARTICLES_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES)) = 
			Enums::k_maximum_number_of_particles_per_map_upgrade;
	}
#endif 
};