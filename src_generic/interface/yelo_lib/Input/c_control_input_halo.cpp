/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Input/c_control_input_halo.hpp"

#include "Interface/Controls.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Input
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Maps a virtual key code to halo's key_code enum. </summary>
		///
		/// <param name="vk_code">	The virtual key code. </param>
		///
		/// <returns>	A key_code. </returns>
		static Enums::key_code MapVirtualKeyCode(const unsigned short vk_code)
		{
			auto key_code = Enums::k_number_of_keys;

			switch(vk_code)
			{
			case VK_ESCAPE: key_code = Enums::_key_code_escape; break;
			case VK_F1: key_code = Enums::_key_code_f1; break;
			case VK_F2: key_code = Enums::_key_code_f2; break;
			case VK_F3: key_code = Enums::_key_code_f3; break;
			case VK_F4: key_code = Enums::_key_code_f4; break;
			case VK_F5: key_code = Enums::_key_code_f5; break;
			case VK_F6: key_code = Enums::_key_code_f6; break;
			case VK_F7: key_code = Enums::_key_code_f7; break;
			case VK_F8: key_code = Enums::_key_code_f8; break;
			case VK_F9: key_code = Enums::_key_code_f9; break;
			case VK_F10: key_code = Enums::_key_code_f10; break;
			case VK_F11: key_code = Enums::_key_code_f11; break;
			case VK_F12: key_code = Enums::_key_code_f12; break;
			case VK_PRINT: key_code = Enums::_key_code_print_screen; break;
			case VK_SCROLL: key_code = Enums::_key_code_scroll_lock; break;
			case VK_PAUSE: key_code = Enums::_key_code_pause; break;
			case VK_BACK: key_code = Enums::_key_code_backspace; break;
			case VK_TAB: key_code = Enums::_key_code_tab; break;
			case VK_CAPITAL: key_code = Enums::_key_code_caps; break;
			case VK_RETURN: key_code = Enums::_key_code_enter; break;
			case VK_SHIFT: key_code = Enums::_key_code_shift; break;
			case VK_CONTROL: key_code = Enums::_key_code_ctrl; break;
			case VK_UP: key_code = Enums::_key_code_up; break;
			case VK_DOWN: key_code = Enums::_key_code_down; break;
			case VK_LEFT: key_code = Enums::_key_code_left; break;
			case VK_RIGHT: key_code = Enums::_key_code_right; break;
			case VK_INSERT: key_code = Enums::_key_code_insert; break;
			case VK_HOME: key_code = Enums::_key_code_home; break;
			case VK_PRIOR: key_code = Enums::_key_code_page_up; break;
			case VK_DELETE: key_code = Enums::_key_code_delete; break;
			case VK_END: key_code = Enums::_key_code_end; break;
			case VK_NEXT: key_code = Enums::_key_code_page_down; break;
			case VK_NUMLOCK: key_code = Enums::_key_code_nums_lock; break;
			case VK_NUMPAD0: key_code = Enums::_key_code_Num0; break;
			case VK_NUMPAD1: key_code = Enums::_key_code_Num1; break;
			case VK_NUMPAD2: key_code = Enums::_key_code_Num2; break;
			case VK_NUMPAD3: key_code = Enums::_key_code_Num3; break;
			case VK_NUMPAD4: key_code = Enums::_key_code_Num4; break;
			case VK_NUMPAD5: key_code = Enums::_key_code_Num5; break;
			case VK_NUMPAD6: key_code = Enums::_key_code_Num6; break;
			case VK_NUMPAD7: key_code = Enums::_key_code_Num7; break;
			case VK_NUMPAD8: key_code = Enums::_key_code_Num8; break;
			case VK_NUMPAD9: key_code = Enums::_key_code_Num9; break;
			}

			return key_code;
		}

		c_control_input_halo::c_control_input_halo()
			: m_mouse_input_handlers()
			, m_keyboard_input_handlers()
			, m_mouse_bounds()
			, m_mouse_position()
		{ }

		void c_control_input_halo::SetMouseBounds(const int min_x, const int max_x, const int min_y, const int max_y)
		{
			m_mouse_bounds.left = min_x;
			m_mouse_bounds.right = max_x;
			m_mouse_bounds.bottom = min_y;
			m_mouse_bounds.top = max_y;
		}

		void c_control_input_halo::SetMousePosition(const int x, const int y)
		{
			m_mouse_position.x = __min(x,					m_mouse_bounds.right);
			m_mouse_position.x = __max(m_mouse_position.x,	m_mouse_bounds.left);
			m_mouse_position.y = __min(y,					m_mouse_bounds.top);
			m_mouse_position.y = __max(m_mouse_position.y,	m_mouse_bounds.bottom);
		}

		void c_control_input_halo::Update()
		{
			UpdateMouseMovement();
			UpdateMouseButtons();
		}

		void c_control_input_halo::AttachMouseInputHandler(i_control_mouse_handler* handler)
		{
			auto existing_entry = std::find(m_mouse_input_handlers.begin(), m_mouse_input_handlers.end(), handler);
			if(existing_entry == m_mouse_input_handlers.end())
			{
				m_mouse_input_handlers.push_back(handler);
			}
		}

		void c_control_input_halo::DetachMouseInputHandler(const i_control_mouse_handler* handler)
		{
			auto existing_entry = std::find(m_mouse_input_handlers.begin(), m_mouse_input_handlers.end(), handler);
			if(existing_entry != m_mouse_input_handlers.end())
			{
				m_mouse_input_handlers.erase(existing_entry);
			}
		}

		void c_control_input_halo::AttachKeyboardInputHandler(i_control_keyboard_handler* handler)
		{
			auto existing_entry = std::find(m_keyboard_input_handlers.begin(), m_keyboard_input_handlers.end(), handler);
			if(existing_entry == m_keyboard_input_handlers.end())
			{
				m_keyboard_input_handlers.push_back(handler);
			}
		}

		void c_control_input_halo::DetachKeyboardInputHandler(const i_control_keyboard_handler* handler)
		{
			auto existing_entry = std::find(m_keyboard_input_handlers.begin(), m_keyboard_input_handlers.end(), handler);
			if(existing_entry != m_keyboard_input_handlers.end())
			{
				m_keyboard_input_handlers.erase(existing_entry);
			}
		}

		void c_control_input_halo::UpdateMouseMovement()
		{
			// Update the mouse position
			int x = m_mouse_position.x + Yelo::Input::GetMouseAxisState(Enums::_MouseAxisX);
			int y = m_mouse_position.y + -Yelo::Input::GetMouseAxisState(Enums::_MouseAxisY);
			int dx = x - m_mouse_position.x;
			int dy = y - m_mouse_position.y;

			m_mouse_position.x = __min(x,					m_mouse_bounds.right);
			m_mouse_position.x = __max(m_mouse_position.x,	m_mouse_bounds.left);
			m_mouse_position.y = __min(y,					m_mouse_bounds.top);
			m_mouse_position.y = __max(m_mouse_position.y,	m_mouse_bounds.bottom);

			for(auto handler : m_mouse_input_handlers)
			{
				handler->OnMousePositionUpdate(m_mouse_position, point2d { dx, dy });
			}
		}

		void c_control_input_halo::UpdateMouseButtons()
		{
#ifdef WM_MOUSEWHEEL
			for(auto handler : m_mouse_input_handlers)
			{
				handler->OnMouseWheelUpdate(Yelo::Input::GetMouseAxisState(Enums::_MouseAxisWheel));
			}
#endif

			// Update the mouse button's
			auto get_mouse_input =
				[&](const Enums::MouseButton button, const long index)
				{
					auto value = Yelo::Input::GetMouseButtonState(Enums::_MouseButton1) > 0;

					if(value != m_mouse_button_states[index])
					{
						for(auto handler : m_mouse_input_handlers)
						{
							handler->OnMouseButtonUpdate(index, value);
						}

						m_mouse_button_states[index] = value;
					}
				};
			
			get_mouse_input(Enums::_MouseButton1, 0);
			get_mouse_input(Enums::_MouseButton2, 1);
			get_mouse_input(Enums::_MouseButton3, 2);
		}

		void c_control_input_halo::SendKeyboardButton(const Enums::key_code key, const bool state)
		{
			// Send the button state to the handlers
			for(auto handler : m_keyboard_input_handlers)
			{
				handler->OnKeyboardButtonUpdate(key, state);
			}
		}

		void c_control_input_halo::SendKeyboardCharacter(const wchar_t character)
		{
			// Send the character to the handlers
			for(auto handler : m_keyboard_input_handlers)
			{
				handler->OnKeyboardCharacterPressed(character);
			}
		}

		void c_control_input_halo::HandleMessage(const MSG* message)
		{
			switch(message->message)
			{
				case WM_CHAR:
					SendKeyboardCharacter(message->wParam);
					break;
				case WM_KEYDOWN:
				{
					auto key_code = MapVirtualKeyCode(message->wParam);

					if(key_code != Enums::k_number_of_keys)
					{
						SendKeyboardButton(key_code, true);
					}
					break;
				}
				case WM_KEYUP:
				{
					auto key_code = MapVirtualKeyCode(message->wParam);

					if(key_code != Enums::k_number_of_keys)
					{
						SendKeyboardButton(key_code, false);
					}
					break;
				}
			}
		}
	};};};
};
