/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Interface/Controls.hpp"
#include "Game/EngineFunctions.hpp"
#include "Memory/FunctionInterface.hpp"


namespace Yelo
{
	namespace Hud
	{
		struct s_hud_globals
		{
			struct
			{
				bool show_hud;
				bool scale_hud;
				bool show_crosshair;
				unsigned char : 8;
			}m_flags;

			point2d			m_screen_size;

			struct
			{
				real_point2d	scale;
				real_point2d	projection;
				real_point2d	translation;
			}m_original_screen, m_hud_screen, m_widget_screen;

			struct
			{
				real_point2d	projection;
				real_point2d	translation;
			}m_projection;

			struct
			{
				real half_width;
			}m_damage_anchor;
		};
		static s_hud_globals g_hud_globals;

		struct s_multiplayer_icon
		{
			const point2d m_stock_position;
			unsigned short* m_position_x;
			unsigned short* m_position_y;
		};
		static s_multiplayer_icon g_multiplayer_icons[] =
		{
			// ctf
			{
				{ 456 - 640, 6 },
				GET_PTR2(HUD_MULTIPLAYER_ICON_CTF_X),
				GET_PTR2(HUD_MULTIPLAYER_ICON_CTF_Y)
			},
			// king
			{
				{ 458 - 640, 6 },
				GET_PTR2(HUD_MULTIPLAYER_ICON_KING_X),
				nullptr
			},
			// oddball
			{
				{ 456 - 640, 7 },
				GET_PTR2(HUD_MULTIPLAYER_ICON_ODDBALL_X),
				GET_PTR2(HUD_MULTIPLAYER_ICON_ODDBALL_Y)
			},
			// race
			{
				{ 453 - 640, 6 },
				GET_PTR2(HUD_MULTIPLAYER_ICON_RACE_X),
				GET_PTR2(HUD_MULTIPLAYER_ICON_RACE_Y)
			},
			// slayer
			{
				{ 457 - 640, 6 },
				GET_PTR2(HUD_MULTIPLAYER_ICON_SLAYER_X),
				nullptr
			},
			// background
			{
				{ 445 - 640, 6 },
				GET_PTR2(HUD_MULTIPLAYER_ICON_BACKGROUND_X),
				GET_PTR2(HUD_MULTIPLAYER_ICON_BACKGROUND_Y)
			},
		};

		class c_settings_container : public Yelo::Configuration::c_configuration_container {
		public:
			Yelo::Configuration::c_configuration_value<bool> m_show_hud;
			Yelo::Configuration::c_configuration_value<bool> m_scale_hud;
			Yelo::Configuration::c_real_point2d_container m_hud_scale;

			c_settings_container()
				: Yelo::Configuration::c_configuration_container("HUD")
				, m_show_hud("ShowHUD", true)
				, m_scale_hud("ScaleHUD", true)
				, m_hud_scale("HUDScale", 1.0f, 1.0f)
			{ }
			
		protected:
			const std::vector<i_configuration_value* const> GetMembers() final override
			{
				return std::vector<i_configuration_value*> {
					&m_show_hud,
					&m_scale_hud,
					&m_hud_scale
				};
			}
		};

		class c_settings_hud
			: public Settings::c_settings_singleton<c_settings_container, c_settings_hud>
		{
		public:
			void PostLoad() final override
			{
				auto& settings_instance = Get();
				g_hud_globals.m_flags.scale_hud = settings_instance.m_scale_hud;
				g_hud_globals.m_flags.show_hud = settings_instance.m_show_hud;
				g_hud_globals.m_hud_screen.scale.x = settings_instance.m_hud_scale.GetConst().x;
				g_hud_globals.m_hud_screen.scale.y = settings_instance.m_hud_scale.GetConst().y;
			}

			void PreSave() final override
			{
				auto& settings_instance = Get();
				settings_instance.m_scale_hud = g_hud_globals.m_flags.scale_hud;
				settings_instance.m_show_hud = g_hud_globals.m_flags.show_hud;
				settings_instance.m_hud_scale.Get().x = g_hud_globals.m_hud_screen.scale.x;
				settings_instance.m_hud_scale.Get().y = g_hud_globals.m_hud_screen.scale.y;
			}
		};

		bool& ShowHUD() { return g_hud_globals.m_flags.show_hud; }

		bool& ScaleHUD() { return g_hud_globals.m_flags.scale_hud; }

		bool& ShowCrosshair() { return g_hud_globals.m_flags.show_crosshair; }

		real& HUDScaleX() { return g_hud_globals.m_hud_screen.scale.x; }

		real& HUDScaleY() { return g_hud_globals.m_hud_screen.scale.y; }

		void* HudGlobals() DPTR_IMP_GET2(hud_globals);
		void EnableWidgetScale()
		{
			g_hud_globals.m_projection.projection = g_hud_globals.m_widget_screen.projection;
			g_hud_globals.m_projection.translation = g_hud_globals.m_widget_screen.translation;
		}

		void EnableHUDScale()
		{
			g_hud_globals.m_projection.projection = g_hud_globals.m_hud_screen.projection;
			g_hud_globals.m_projection.translation = g_hud_globals.m_hud_screen.translation;
		}

		void DisableScale()
		{
			g_hud_globals.m_projection.projection = g_hud_globals.m_original_screen.projection;
			g_hud_globals.m_projection.translation = g_hud_globals.m_original_screen.translation;
		}

		/// <summary>	Restores the original translation values to disable any UI offsetting. </summary>
		static void DisableOffset()
		{
			g_hud_globals.m_projection.translation = g_hud_globals.m_original_screen.translation;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Override of the render widget recursive function which applies UI scaling. </summary>
		///
		/// <param name="widget_datum_index">	[in,out] Pointer the datum index of the widget. </param>
		/// <param name="arg_1">			 	[in,out] First argument from the stock function. </param>
		/// <param name="arg_2">			 	[in,out] Second argument from the stock function. </param>
		/// <param name="arg_3">			 	[in,out] Third argument from the stock function. </param>
		/// <param name="arg_4">			 	[in,out] Fourth argument from the stock function. </param>
		///
		/// <returns>	A __cdecl. </returns>
		static void __cdecl RenderWidgetRecursiveHook(datum_index* widget_datum_index, void* arg_1, void* arg_2,void* arg_3, void* arg_4)
		{
			typedef void (__cdecl* function_t)(void*, void*, void*, void*, void*);
			static const function_t render_widget_recursive = reinterpret_cast<function_t>(0x49D850);

			// get the widget tag
			const auto* widget = blam::tag_get<TagGroups::ui_widget_definition>(*widget_datum_index);

			// do not scale the widget if it is full screen
			bool scale_widget = ((widget->bounds.right - widget->bounds.left) != 640) && ((widget->bounds.bottom - widget->bounds.top) != 480);

			// default to enable widget scaling, disable if required
			EnableWidgetScale();
			if (!scale_widget)
			{
				DisableScale();
			}

			// render the widget
			render_widget_recursive(widget_datum_index, arg_1, arg_2, arg_3, arg_4);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Override of the render players function to apply HUD scaling. </summary>
		///
		/// <returns>	A __cdecl. </returns>
		static void __cdecl HudRenderPlayersHook()
		{
			typedef void (__cdecl* function_t)();
			static const function_t render_players = CAST_PTR(function_t, 0x4AD830);

			// disable the hud offset when rendering player indicators, reenable afterwards
			DisableOffset();

			render_players();

			EnableHUDScale();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Override of the render nav points function to apply HUD scaling. </summary>
		///
		/// <param name="arg0">	[in,out] First argument of the stock function. </param>
		///
		/// <returns>	A __cdecl. </returns>
		static void __cdecl HudRenderNavPointsHook(void* arg0)
		{
			typedef void (__cdecl* function_t)(void*);
			static const function_t render_nav_points = CAST_PTR(function_t, GET_FUNC_VPTR(HUD_RENDER_NAV_POINTS));

			// disable the hud offset when rendering nav points, reenable afterwards
			DisableOffset();

			render_nav_points(arg0);

			EnableHUDScale();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Override of the render in-game scoreboard function to apply UI scaling. </summary>
		///
		/// <returns>	A __cdecl. </returns>
		static void __cdecl HudRenderScoreboardInGameHook()
		{
			typedef void (__cdecl* function_t)();
			static const function_t render_scoreboard_ingame = CAST_PTR(function_t, GET_FUNC_VPTR(HUD_RENDER_SCOREBOARD_INGAME));

			// switch to the widget screen scaling when rendering the scoreboard, switch back to hud scaling afterwards
			EnableWidgetScale();

			render_scoreboard_ingame();

			EnableHUDScale();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Override of the render post-game scoreboard function to apply UI scaling. </summary>
		///
		/// <returns>	A __cdecl. </returns>
		static void __cdecl HudRenderScoreboardPostGameHook()
		{
			typedef void (__cdecl* function_t)();
			static const function_t render_scoreboard_postgame = CAST_PTR(function_t, GET_FUNC_VPTR(HUD_RENDER_SCOREBOARD_POSTGAME));

			// switch to the widget screen scaling when rendering the scoreboard, switch back to hud scaling afterwards
			EnableWidgetScale();

			render_scoreboard_postgame();

			EnableHUDScale();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Modifies the original screen projection matrix to enable UI scaling. </summary>
		///
		/// <param name="device">			[in] The graphics device. </param>
		/// <param name="StartRegister">	The register to start writing from. </param>
		/// <param name="pConstantData">	Pointer to the constants data. </param>
		/// <param name="Vector4fCount">	The number of vector4's to set. </param>
		///
		/// <returns>	A HRESULT result. </returns>
		static HRESULT SetVertexShaderConstantF_ScreenProj(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
		{
			auto* original_matrix = CAST_PTR(D3DXMATRIX*, (float*)pConstantData);

			original_matrix->_11 = g_hud_globals.m_projection.projection.x;
			original_matrix->_22 = g_hud_globals.m_projection.projection.y;
			original_matrix->_14 = g_hud_globals.m_projection.translation.x;
			original_matrix->_24 = g_hud_globals.m_projection.translation.y;

			return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Calculates the absolute position of an anchored point. </summary>
		///
		/// <param name="anchor">			The anchor type. </param>
		/// <param name="anchor_bounds">	The anchor bounds. </param>
		/// <param name="input">			The input point. </param>
		/// <param name="output">			[out] The output point. </param>
		static void CalculateAnchor(const Enums::hud_anchor anchor, const point2d anchor_bounds, const point2d input, point2d& output)
		{
			switch(anchor)
			{
			case Enums::_hud_anchor_top_left:
				output.x = input.x;
				output.y = input.y;
				break;
			case Enums::_hud_anchor_top_right:
				output.x = anchor_bounds.x + input.x;
				output.y = input.y;
				break;
			case Enums::_hud_anchor_bottom_left:
				output.x = input.x;
				output.y = anchor_bounds.y + input.y;
				break;
			case Enums::_hud_anchor_bottom_right:
				output.x = anchor_bounds.x + input.x;
				output.y = anchor_bounds.y + input.y;
				break;
			case Enums::_hud_anchor_center:
				output.x = (short)(anchor_bounds.x / 2) + input.x;
				output.y = (short)(anchor_bounds.y / 2) + input.y;
				break;
			}
		}

		/// <summary>	Updates the HUD scale globals for the current screen size. </summary>
		static void UpdateScreenScale()
		{
			real_point2d screen_ratio;
			screen_ratio.x = 1;
			screen_ratio.y = 1;

			if(g_hud_globals.m_flags.scale_hud)
			{
				const real old_ratio = (real)4/3;
				const real new_ratio = (real)g_hud_globals.m_screen_size.x / g_hud_globals.m_screen_size.y;

				if (new_ratio > old_ratio)
					screen_ratio.x = old_ratio / new_ratio;
				else
					screen_ratio.y = new_ratio / old_ratio;
			}

			g_hud_globals.m_widget_screen.projection = g_hud_globals.m_original_screen.projection;
			g_hud_globals.m_widget_screen.projection.x *= screen_ratio.x;
			g_hud_globals.m_widget_screen.projection.y *= screen_ratio.y;

			g_hud_globals.m_widget_screen.translation = screen_ratio;
			g_hud_globals.m_widget_screen.translation.x *= -1;
		}
		
		/// <summary>	Updates the HUD scale for the current HUD size. </summary>
		static void UpdateAnchorScale()
		{
			real_point2d anchor_scale;
			anchor_scale.x = 1;
			anchor_scale.y = 1;

			if(g_hud_globals.m_flags.scale_hud)
			{
				const real old_ratio = (real)4/3;
				const real new_ratio = (real)g_hud_globals.m_screen_size.x / g_hud_globals.m_screen_size.y;

				if (new_ratio > old_ratio)
					anchor_scale.x = new_ratio / old_ratio;
				else
					anchor_scale.y = old_ratio / new_ratio;
			}

			point2d anchor_scale_result;

			// set the navpoint values
			anchor_scale_result.x = (short)(640 * anchor_scale.x);
			anchor_scale_result.y = (short)(480 * anchor_scale.y);

			GET_PTR(HUD_RENDER_NAV_POINT_ANCHOR_HALF_X) = anchor_scale_result.x / 2;
			GET_PTR(HUD_RENDER_NAV_POINT_ANCHOR_HALF_Y) = anchor_scale_result.y / 2;
			GET_PTR(HUD_ANCHOR_SCREEN_X) = anchor_scale_result.x;
			GET_PTR(HUD_ANCHOR_SCREEN_Y) = anchor_scale_result.y;
			GET_PTR(HUD_ANCHOR_SCREEN_HALF_X) = anchor_scale_result.x / 2.0f;
			GET_PTR(HUD_ANCHOR_SCREEN_HALF_Y) = anchor_scale_result.y / 2.0f;

			// set the hud anchor values
			real_point2d hud_scale_amount;
			if(g_hud_globals.m_flags.scale_hud)
			{
				hud_scale_amount = g_hud_globals.m_hud_screen.scale;
			}
			else
			{
				hud_scale_amount.x = 1;
				hud_scale_amount.y = 1;
			}

			g_hud_globals.m_hud_screen.projection = g_hud_globals.m_widget_screen.projection;
			g_hud_globals.m_hud_screen.translation = hud_scale_amount;
			g_hud_globals.m_hud_screen.translation.x *= -1;

			anchor_scale_result.x = (short)(anchor_scale_result.x * hud_scale_amount.x);
			anchor_scale_result.y = (short)(anchor_scale_result.y * hud_scale_amount.y);

			GET_PTR(HUD_POINT_ANCHOR_WIDTH) = anchor_scale_result.x - 16;
			GET_PTR(HUD_POINT_ANCHOR_HEIGHT) = anchor_scale_result.y - 8;
			GET_PTR(HUD_POINT_ANCHOR_HALF_WIDTH) = anchor_scale_result.x / 2;
			GET_PTR(HUD_POINT_ANCHOR_HALF_HEIGHT) = anchor_scale_result.y / 2;

			// set the damage indicator anchor values
			GET_PTR(HUD_POINT_DAMAGE_ANCHOR_WIDTH) = anchor_scale_result.x - 8;
			GET_PTR(HUD_POINT_DAMAGE_ANCHOR_HEIGHT) = anchor_scale_result.y - 8;
			GET_PTR(HUD_POINT_DAMAGE_ANCHOR_HALF_HEIGHT) = anchor_scale_result.y / 2.0f;
			g_hud_globals.m_damage_anchor.half_width = anchor_scale_result.x / 2.0f;

			// set the multiplayer hud icon positions
			point2d position_out;
			for(int i = 0; i < std::size(g_multiplayer_icons); i++)
			{
				CalculateAnchor(Enums::_hud_anchor_top_right, anchor_scale_result, g_multiplayer_icons[i].m_stock_position, position_out);

				if (g_multiplayer_icons[i].m_position_x)
				{
					*g_multiplayer_icons[i].m_position_x = position_out.x;
				}
				if (g_multiplayer_icons[i].m_position_y)
				{
					*g_multiplayer_icons[i].m_position_y = position_out.y;
				}
			}
		}

		void UpdateChanges()
		{
			UpdateScreenScale();
			UpdateAnchorScale();
		}
		void Initialize()
		{
			// TODO: ranged for loop
			// hook the screen projection matrix for widgets, text and the motion tracker
			for(int i = 0; i < std::size(K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALLS); i++)
			{
				void*& call_address = K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALLS[i];

				Memory::WriteRelativeCall(SetVertexShaderConstantF_ScreenProj, call_address, true);
				byte* nop_offset = CAST_PTR(byte*, call_address) + 5;
				*nop_offset = Enums::_x86_opcode_nop;
			}

			// hook the various UI render functions
			Memory::WriteRelativeCall(HudRenderPlayersHook, 0x4ACA6E, true);
			Memory::WriteRelativeCall(HudRenderNavPointsHook, GET_FUNC_VPTR(HUD_RENDER_NAV_POINTS_CALL), true);

			Memory::WriteRelativeCall(HudRenderScoreboardPostGameHook, GET_FUNC_VPTR(HUD_RENDER_SCOREBOARD_POSTGAME_CALL), true);
			Memory::WriteRelativeCall(HudRenderScoreboardInGameHook, GET_FUNC_VPTR(HUD_RENDER_SCOREBOARD_INGAME_CALL), true);

			// TODO: ranged for loop
			for (int i = 0; i < std::size(K_RENDER_WIDGET_RECURSIVE_CALLS); i++)
			{
				Memory::WriteRelativeCall(RenderWidgetRecursiveHook, K_RENDER_WIDGET_RECURSIVE_CALLS[i], true);
			}

			// TODO: ranged for loop
			// point the half width values for damage indicators to a float we modify
			for (int i = 0; i < std::size(K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTRS); i++)
			{
				*K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTRS[i] = &g_hud_globals.m_damage_anchor.half_width;
			}

			g_hud_globals.m_flags.show_crosshair = true;

			c_settings_hud::Register(Settings::Manager());
		}

		void Dispose()
		{
			c_settings_hud::Unregister(Settings::Manager());
		}

		void Update()
		{
			// if the crosshair should be hidden modify the jump that skips it otherwise ensure it is not skipped
			if (g_hud_globals.m_flags.show_crosshair)
			{
				GET_PTR(RENDER_CROSSHAIRS_DISABLE_MOD) = 0x840F;	// set to jz
			}
			else
			{
				GET_PTR(RENDER_CROSSHAIRS_DISABLE_MOD) = 0xE990;	// set to nop, relative jmp
			}

			// update the function hook for disabling the hud rendering
			Memory::FunctionProcessRenderHudIsDisabled() = !g_hud_globals.m_flags.show_hud;
		}
		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP)
		{
			// set up the original projection and translation globals
			g_hud_globals.m_original_screen.projection.x = (2.0f / 640.0f);
			g_hud_globals.m_original_screen.projection.y = (-2.0f / 480.0f);
			g_hud_globals.m_original_screen.translation.x = -1;
			g_hud_globals.m_original_screen.translation.y = 1;

			// set the scale values for the current screen resolution
			g_hud_globals.m_screen_size.x = (short)pPP->BackBufferWidth;
			g_hud_globals.m_screen_size.y = (short)pPP->BackBufferHeight;
			UpdateScreenScale();
			UpdateAnchorScale();
		}

		void OnLostDevice() { }

		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP)
		{
			// set the scale values for the current screen resolution
			g_hud_globals.m_screen_size.x = (short)pPP->BackBufferWidth;
			g_hud_globals.m_screen_size.y = (short)pPP->BackBufferHeight;
			UpdateScreenScale();
			UpdateAnchorScale();
		}

		void Render() { }

		void Release() { }
	};
};
