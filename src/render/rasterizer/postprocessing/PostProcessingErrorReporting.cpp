/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/PostProcessingErrorReporting.hpp"

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/main/console.hpp>

#include "Settings/Settings.hpp"
#include "Game/GameState.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace ErrorReporting
	{
		void Write(cstring string)
		{
			if(!GameState::DevmodeEnabled())
				return;

			// console messages are only displayed when developer mode is 4 or above
			byte previous_dev_mode = *GameState::DeveloperMode();
			*GameState::DeveloperMode() = Enums::k_developer_mode_level_debug_output;

			// print the string to the console
			blam::console_printf(false, string);

			*GameState::DeveloperMode() = previous_dev_mode;
		}

		void WriteLine(cstring format, ...)
		{
			if(!GameState::DevmodeEnabled())
				return;

			va_list argptr;
			va_start(argptr, format);

			// get the required length of the buffer and create it
			size_t length = (size_t)_vscprintf(format, argptr) + 1;
			char* buffer = new char[length];
			memset(buffer, 0, length);

			// write the arguments to the buffer
			vsprintf_s(buffer, length, format, argptr);
			va_end(argptr);
			
			// write the buffer to the console
			Write(buffer);
			delete [] buffer;
		}

		void WriteD3DXErrors(LPD3DXBUFFER buffer)
		{
			// errors are only in devmode
			if(!GameState::DevmodeEnabled())
				return;

			if(!buffer)
				return;

			char* string_pointer = CAST_PTR(char*, buffer->GetBufferPointer());

			bool remove_path = false;
			size_t work_dir_path_len = strlen(Settings::WorkingDirectoryPath());

			size_t string_length = strlen(string_pointer);
			int32 count = 0;
			while(string_pointer[0] != '\0' && count < k_max_error_line_count)
			{
				if(strlen(string_pointer) < work_dir_path_len)
					remove_path = false;
				else
				{
					// determine whether the line starts with the HaloCE path
					// if it does, chop it out
					char* line_start = new char[work_dir_path_len + 1];
					line_start[0] = 0;
					strncpy_s(line_start, work_dir_path_len + 1, string_pointer, _TRUNCATE);

					if(strcmp(line_start, Settings::WorkingDirectoryPath()) == 0)
						remove_path = true;

					delete [] line_start;
				}

				// move the pointer to the start of the line
				string_pointer += (remove_path ? work_dir_path_len + 7 : 0);

				// get the line length by reading to the next newline character
				size_t line_length = strcspn(string_pointer, "\n");

				// copy the line to a new buffer
				char* line_buffer = new char[line_length + 1];
				strncpy_s(line_buffer, line_length + 1, string_pointer, _TRUNCATE);

				// move the string pointer past the current line
				string_pointer += line_length + 1;

				// only print strings that have "error" in them
				if(strstr(line_buffer, "error"))
				{
					Write(line_buffer);
					count++;
				}

				delete[] line_buffer;
				line_buffer = NULL;
			}
		};};};
	};
};
#endif