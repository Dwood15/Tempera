/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum game_state_life_cycle
		{
			_game_state_life_cycle_before_save,
			_game_state_life_cycle_before_load,
			_game_state_life_cycle_after_load,

			k_number_of_game_state_life_cycles
		};

		enum {
			// How much additional memory, if any, we allocate for the objects pool
			k_game_state_allocation_size_object_memory_pool_upgrade = 0x10000,
			// 0x42974 bytes available in game state allocation for Clients.
			// Dedis should have a little bit more (since they don't alloc a few render based things) 
			// but to keep things simple we'll limit both sides of the spectrum to the same size.
			// UPDATE: 0x50A78 bytes for Dedi (stock only allocates 0x3EF588 bytes)
			// NOTE: Basically, the game doesn't fully use the amount it sets aside for game state data 
			// (ie, objects, players, etc), so we're able to steal some of this memory for our own use. 
			// Why? Because the game state is saved to file when the player saves the game or dumps a core. 
			// So if there's a project component which requires serialization, we'd want to allocate its 
			// memory in the game state, not anywhere else.
			k_game_state_allocation_maximum_size_for_yelo = 0x42970
				- k_game_state_allocation_size_object_memory_pool_upgrade,
		};
		static_assert( k_game_state_allocation_maximum_size_for_yelo >= 0 );
	};

	namespace GameState
	{
		struct s_yelo_header_data
		{
			unsigned char : 8; // since the unused 32 bytes in s_header_data is more than likely a tag_string, we don't want to touch what is actually the first char (we want it to stay zero)
			struct {
				bool initialized : 1;
				bool game_state_upgrades_on : 1;
				bool update_unit_grenade_types_count : 1;
			}flags;									// 0x1
			struct {
				byte major;							// 0x2
				byte minor;							// 0x3
				unsigned short build;						// 0x4
			}version;
			byte unit_grenade_types_count;			// 0x6
			unsigned char : 8;
		}; static_assert( sizeof(s_yelo_header_data) <= 0x20 );


		// Are OS-modified game states in effect?
		// If so, these will render game saves incompatible with stock games
		bool YeloGameStateEnabled();

		// Allocate an object of type [T] [count] times inside the game state memory and return its address.
		// Note: Also updates the game state's cpu allocation size by adding 'sizeof([T]) * count'
		template<typename T> inline
		T* GameStateMalloc(const bool k_update_allocation_crc = true, const size_t count = 1)
		{
			extern void* GameStateMalloc(const bool k_update_allocation_crc, const size_t size_of);

			return reinterpret_cast<T *>(GameStateMalloc(k_update_allocation_crc, sizeof(T) * count));
		}
	};
};
