#pragma once
#include <precompile.h>
namespace Yelo
{

	namespace Enums
	{
		enum hud_chat_type : signed long;
		enum settings_adjustment_result : signed long;

		enum rasterize_score : signed long
		{
			_rasterize_score_welcome,
			_rasterize_score_s_died,
			_rasterize_score_s_was_killed_by_guardians,
			_rasterize_score_s_was_killed_by_vehicle,
			_rasterize_score_s_was_killed_by_s,
			_rasterize_score_s_was_betrayed_bs_s,
			_rasterize_score_s_commited_suicide,
			_rasterize_score_double_kill,
			_rasterize_score_you_killed_s,
			_rasterize_score_triple_kill,
			_rasterize_score_killtacular,
			_rasterize_score_killing_spree,
			_rasterize_score_running_riot,
			_rasterize_score_you_betrayed_s,
			_rasterize_score_killtacular_d,
			_rasterize_score_triple_kill_d,
			_rasterize_score_double_kill_d,
			_rasterize_score_running_riot_d,
			_rasterize_score_started_killing_spree,
			_rasterize_score_you_killed_s_d,
			_rasterize_score_you_are_odd_man_out,
			_rasterize_score_you_are_out_of_lives,
			_rasterize_score_rejoin_in_d,
			_rasterize_score_waiting_for_space,
			_rasterize_score_you_quit,
			_rasterize_score_s_quit,
			_rasterize_score_hold_s_for_score,
			_rasterize_score_d_remaining,
			_rasterize_score_telefragged,
			_rasterize_score,
		};
	};

	namespace GameUI
	{
		void Initialize();
		void Dispose();
		void __cdecl Update();

		void __cdecl FirstPersonWeaponsUpdate();


		// Predicate to use for determining when to send a message to players
		typedef bool (__stdcall* proc_send_hud_chat_predicate)(Players::s_player_datum* player,
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index);
		// Predicate for always sending a message to 
		bool SendHudChatToEveryonePredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index);

		// Send [messsage] to all valid players (machine_index != NONE) and who pass the [send_predicate]
		void SendHudChat(Enums::hud_chat_type msg_type, wconst char * message, byte player_number,
			proc_send_hud_chat_predicate send_predicate = SendHudChatToEveryonePredicate);
	};


	namespace Hud
	{
#pragma region Component Interface
		/// <summary>	Hooks the games HUD functions and values. </summary>
		void Initialize();

		/// <summary>	Unused. </summary>
		void Dispose();

		/// <summary>	Updates the HUD scaling controls. </summary>
		void Update();
#pragma endregion

#pragma region Properties
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets whether to show the HUD. </summary>
		///
		/// <returns>	A bool reference. </returns>
		bool& ShowHUD();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets whether to scale the HUD. </summary>
		///
		/// <returns>	A bool reference. </returns>
		bool& ScaleHUD();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets whether to show the crosshair. </summary>
		///
		/// <returns>	A bool reference. </returns>
		bool& ShowCrosshair();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the HUD X scale. </summary>
		///
		/// <returns>	A real reference. </returns>
		real& HUDScaleY();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the HUD Y scale. </summary>
		///
		/// <returns>	A real reference. </returns>
		real& HUDScaleX();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a pointer to the games HUD globals. </summary>
		///
		/// <returns>	void* to the games HUD globals. </returns>
		void* HudGlobals();

		/// <summary>	Updates the hud scale changes. </summary>
		void UpdateChanges();
#pragma endregion

#pragma region Component Settings

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Updates the UI scaling globals according to user input. </summary>
		///
		/// <returns>	An Enums::settings_adjustment_result. </returns>
		Enums::settings_adjustment_result AdjustSettings();
#pragma endregion

#pragma region Enable/Disable Scaling
		/// <summary>	Replaces the HUD projection and translation values with those that scale the UI widgets. </summary>
		void EnableWidgetScale();

		/// <summary>	Replaces the HUD projection and translation values with those that scale the players HUD. </summary>
		void EnableHUDScale();

		/// <summary>	Restores the original projection and translation values to disable any UI scaling. </summary>
		void DisableScale();
#pragma endregion

#pragma region Component DirectX Interface

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Sets the scale values for the initial screen resolution and allocates resources for drawing the menu.
		/// </summary>
		///
		/// <param name="pDevice">	[in] The render device. </param>
		/// <param name="pPP">	  	[in] The presentation parameters. </param>
		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);

		/// <summary>	Informs the menu text block that the device was lost. </summary>
		void OnLostDevice();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Resets the scale values for the current screen resolution and informs the menu text block that the device was
		/// 	reset.
		/// </summary>
		///
		/// <param name="pPP">	[in] The presentation parameters. </param>
		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);

		/// <summary>	Unused. </summary>
		void Render();

		/// <summary>	Releases the menu text block. </summary>
		void Release();
#pragma endregion
	};
};
