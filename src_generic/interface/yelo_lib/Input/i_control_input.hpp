#pragma once

#include "Interface/OpenSauceUI/Input/i_control_input.hpp"
#include "Interface/OpenSauceUI/Input/i_control_mouse_handler.hpp"
#include "Interface/OpenSauceUI/Input/i_control_keyboard_handler.hpp"
#include "Interface/Controls.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Input 
	{
		/// <summary>	The control input interface. </summary>
		class i_control_input
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the extents of the mouses movement. </summary>
			///
			/// <param name="min_x">	The minimum x coordinate. </param>
			/// <param name="max_x">	The maximum x coordinate. </param>
			/// <param name="min_y">	The minimum y coordinate. </param>
			/// <param name="max_y">	The maximum y coordinate. </param>
			virtual void SetMouseBounds(const int min_x, const int max_x, const int min_y, const int max_y) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the position of the mouse. </summary>
			///
			/// <param name="x">	The mouse x position. </param>
			/// <param name="y">	The mouse y position. </param>
			virtual void SetMousePosition(const int x, const int y) = 0;

			/// <summary>	Updates the ui input. </summary>
			virtual void Update() = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attaches a mouse input handler. </summary>
			///
			/// <param name="handler">	[in] If non-null, the handler. </param>
			virtual void AttachMouseInputHandler(i_control_mouse_handler* handler) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Detaches a mouse input handler. </summary>
			///
			/// <param name="handler">	[in,out] If non-null, the handler. </param>
			virtual void DetachMouseInputHandler(const i_control_mouse_handler* handler) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attaches a keyboard input handler. </summary>
			///
			/// <param name="handler">	[in] If non-null, the handler. </param>
			virtual void AttachKeyboardInputHandler(i_control_keyboard_handler* handler) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Detaches a keyboard input handler. </summary>
			///
			/// <param name="handler">	[in,out] If non-null, the handler. </param>
			virtual void DetachKeyboardInputHandler(const i_control_keyboard_handler* handler) = 0;
		};
	};};};
};
