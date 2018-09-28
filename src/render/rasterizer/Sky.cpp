
#include "../../memory/datum_index.h"
#include "../../memory/memory_interface_base.hpp"
#include "../../scenario/yelo/Scenario.hpp"

namespace Yelo {
	namespace Render {
		namespace Sky {
			static c_sky_manager g_sky_manager;

			/// <summary>	Hooks the sky tag index for atmospheric fog. </summary>
			static __declspec(naked) void AtmosphericFog_Hook() {
				static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(FOG_ATMOSPHERIC_SKY_TAG_INDEX_RETN);
				static datum_index     SKY_TAG_INDEX;

				__asm
				{
					push ecx
					push edx

					call   Render::RenderGlobals
					xoredx, edx
					movsx
					edx, [eax]s_render_globals.visible_sky_index
					push edx
					lea
					edx, [SKY_TAG_INDEX]
					push edx
					lea  ecx, g_sky_manager
					call   Yelo::Render::Sky::c_sky_manager::GetSkyTagIndex
					mov
					eax, [SKY_TAG_INDEX]

					pop edx
					pop ecx
					jmp RETN_ADDRESS
				}
			}

			/// <summary>	Hooks the sky tag index for planar fog. </summary>
			static __declspec(naked) void PlanarFog_Hook() {
				static uintptr_t   RETN_ADDRESS = 0x5587C2;//GET_FUNC_PTR(FOG_PLANAR_SKY_TAG_INDEX_RETN);
				static datum_index SKY_TAG_INDEX;

				__asm
				{
					//NOTE: Different registers used, no copy pasta plz
					push eax
					push edx

					call   Render::RenderGlobals
					xoredx, edx
					movsx
					edx, [eax]s_render_globals.visible_sky_index
					push edx
					lea
					edx, [SKY_TAG_INDEX]
					push edx
					lea  ecx, g_sky_manager
					call   Yelo::Render::Sky::c_sky_manager::GetSkyTagIndex
					mov
					ecx, [SKY_TAG_INDEX]

					pop edx
					pop eax
					jmp RETN_ADDRESS
				}
			}

			/// <summary>	Hooks the sky tag index for rendering the sky. </summary>
			static __declspec(naked) void RenderSky_Hook() {
				static uintptr_t   RETN_ADDRESS = 0x514505;//GET_FUNC_PTR(RENDER_SKY_SKY_TAG_INDEX_RETN);
				static datum_index SKY_TAG_INDEX;

				__asm {
						push   ecx
					push   edx

					call   Render::RenderGlobals
					xor      edx, edx
					movsx   edx,[eax]s_render_globals.visible_sky_index
					push   edx
					lea      edx,[SKY_TAG_INDEX]
					push   edx
					lea      ecx, g_sky_manager
					call   Yelo::Render::Sky::c_sky_manager::GetSkyTagIndex
					mov      eax,[SKY_TAG_INDEX]

					pop      edx
					pop      ecx
					jmp      RETN_ADDRESS
					};
			}

			/// <summary>	Hooks the sky index for determining whether the sky needs to be rendered. </summary>
			static __declspec(naked) void VisibleSkyEnable_Hook() {
				static uintptr_t   RETN_ADDRESS = 0x55696A; // GET_FUNC_PTR(VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_RETN);
				static datum_index SKY_TAG_INDEX;

				__asm
					{
						push   eax
					lea      ecx,[SKY_TAG_INDEX]
					push   ecx
					lea      ecx, g_sky_manager
					call   Yelo::Render::Sky::c_sky_manager::GetSkyTagIndex
					mov      ecx,[SKY_TAG_INDEX]

					jmp      RETN_ADDRESS
					};
			}

			/// <summary>	Initializes hooks and sky manager. </summary>
			void Initialize() {
				Memory::WriteRelativeJmp(&AtmosphericFog_Hook, reinterpret_cast<void *>(0x541C5F), true);
				Memory::WriteRelativeJmp(&PlanarFog_Hook, reinterpret_cast<void *>(0x5587BC), true);
				Memory::WriteRelativeJmp(&RenderSky_Hook, reinterpret_cast<void *>(0x5144FE), true);
				Memory::WriteRelativeJmp(&VisibleSkyEnable_Hook, reinterpret_cast<void *>(0x556960), true);

				g_sky_manager.Clear();
			}

			/// <summary>	Clears the sky manager. </summary>
			void Dispose() {
				g_sky_manager.Clear();
			}

			/// <summary>	Initializes for new map. </summary>
			void InitializeForNewMap() {
				g_sky_manager.Clear();

				g_sky_manager.SetScenarioSkies(Scenario::Scenario()->skies);
			}

			/// <summary>	Dispose from old map. </summary>
			void DisposeFromOldMap() {
				g_sky_manager.Clear();
			}

			/// <summary>	Resets the skies in the sky manager. </summary>
			void Reset() {
				g_sky_manager.Reset();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets up a sky override. </summary>
			///
			/// <param name="sky_index">	 	Zero-based index of the sky to override. </param>
			/// <param name="override_index">	Zero-based index of the override sky. </param>
			void SetSkyIndex(const byte sky_index, const datum_index override_index) {
				g_sky_manager.SetSkyIndex(sky_index, override_index);
			}
		};
	};
};
