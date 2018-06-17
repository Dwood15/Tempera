/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Interface/OpenSauceUI/Input/i_control_input.hpp"
#include "Interface/OpenSauceUI/Input/i_control_mouse_handler.hpp"
#include "Interface/OpenSauceUI/Input/i_control_keyboard_handler.hpp"
#include "Interface/Controls.hpp"
#include "Interface/Keystone.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Input
	{
		/// <summary>	The control input class for the halo runtime. </summary>
		class c_control_input_halo final
			: public i_control_input
			, public Keystone::i_windows_message_handler
		{
			std::vector<i_control_mouse_handler*> m_mouse_input_handlers;
			std::vector<i_control_keyboard_handler*> m_keyboard_input_handlers;

			Yelo::rectangle2d m_mouse_bounds;
			Yelo::point2d m_mouse_position;
			bool m_mouse_button_states[3];
			unsigned char : 8;

		public:
			/// <summary>	Default constructor. </summary>
			c_control_input_halo();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the extents of the mouses movement. </summary>
			///
			/// <param name="min_x">	The minimum x coordinate. </param>
			/// <param name="max_x">	The maximum x coordinate. </param>
			/// <param name="min_y">	The minimum y coordinate. </param>
			/// <param name="max_y">	The maximum y coordinate. </param>
			void SetMouseBounds(const int min_x, const int max_x, const int min_y, const int max_y) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the position of the mouse. </summary>
			///
			/// <param name="x">	The mouse x position. </param>
			/// <param name="y">	The mouse y position. </param>
			void SetMousePosition(const int x, const int y) override;

			/// <summary>	Updates the ui input. </summary>
			void Update() override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attaches a mouse input handler. </summary>
			///
			/// <param name="handler">	[in] If non-null, the handler. </param>
			void AttachMouseInputHandler(i_control_mouse_handler* handler) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Detaches a mouse input handler. </summary>
			///
			/// <param name="handler">	[in,out] If non-null, the handler. </param>
			void DetachMouseInputHandler(const i_control_mouse_handler* handler) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attaches a keyboard input handler. </summary>
			///
			/// <param name="handler">	[in] If non-null, the handler. </param>
			void AttachKeyboardInputHandler(i_control_keyboard_handler* handler) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Detaches a keyboard input handler. </summary>
			///
			/// <param name="handler">	[in,out] If non-null, the handler. </param>
			void DetachKeyboardInputHandler(const i_control_keyboard_handler* handler) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Handles the message described by message. </summary>
			///
			/// <param name="message">	   	The windows message. </param>
			///
			/// <returns>	true if the message was handled, false if not. </returns>
			void HandleMessage(const MSG* message) override;

		private:
			/// <summary>	Updates the mouse movement. </summary>
			void UpdateMouseMovement();

			/// <summary>	Updates the mouse buttons. </summary>
			void UpdateMouseButtons();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sends a keyboard button. </summary>
			///
			/// <param name="key">  	The key. </param>
			/// <param name="state">	The key state. </param>
			void SendKeyboardButton(const Enums::key_code key, const bool state);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sends a keyboard character. </summary>
			///
			/// <param name="character">	The character. </param>
			void SendKeyboardCharacter(const wchar_t character);
		};
	};};};
};
