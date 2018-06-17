#pragma once
#include "Interface/Controls.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Input 
	{
		/// <summary>	The keyboard handler interface. </summary>
		class i_control_keyboard_handler
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the keyboard character pressed action. </summary>
			///
			/// <param name="character">	The pressed character. </param>
			virtual void OnKeyboardCharacterPressed(const wchar_t character) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the keyboard button update action. </summary>
			///
			/// <param name="key">  	The key. </param>
			/// <param name="value">	The button state. </param>
			virtual void OnKeyboardButtonUpdate(const Enums::key_code key, const bool value) = 0;
		};
	};};};
};
