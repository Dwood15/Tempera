/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <minwindef.h>
#include <minwinbase.h>
#include <fileapi.h>
#include "../cseries/base.h"

namespace Yelo {
	namespace Enums {
		enum file_io_open_error {
			_file_io_open_error_none = 0,
			_file_io_open_error_file_does_not_exist,
			_file_io_open_error_file_in_use,
			_file_io_open_error_file_is_empty,
			_file_io_open_error_md5_mismatch,
			_file_io_open_error_unknown,

			_file_io_open_error
		};
		enum file_io_open_access_type {
			_file_io_open_access_type_read  = 1 << 0,
			_file_io_open_access_type_write = 1 << 1,

			_file_io_open_access_type
		};
		enum file_io_open_create_option {
			_file_io_open_create_option_open_existing,
			_file_io_open_create_option_new,
			_file_io_open_create_options_create_if_missing,

			_file_io_open_create_option
		};
		enum file_io_read_error {
			_file_io_read_error_none = 0,
			_file_io_read_error_reading_past_eof,
			_file_io_read_error_failed_to_read_from_file,
			_file_io_read_error_file_is_writeonly,
			_file_io_read_error_file_is_memory_mapped,
			_file_io_read_error_writeonly_mapping,
			_file_io_read_error_failed_to_memory_map_file,
			_file_io_read_error_destination_pointer_is_null,
			_file_io_read_error_file_is_empty,
			_file_io_read_error_unknown,

			_file_io_read_error
		};
		enum file_io_write_error {
			_file_io_write_error_none = 0,
			_file_io_write_error_file_is_readonly,
			_file_io_write_error_write_offset_past_eof,
			_file_io_write_error_device_is_write_protected,
			_file_io_write_error_failed_to_write_to_file,
			_file_io_write_error_unknown,

			_file_io_write_error
		};
		enum file_io_delete_error {
			_file_io_delete_error_none    = 0,
			_file_io_delete_error_does_not_exist,
			_file_io_delete_error_in_use,
			_file_io_delete_error_directory_not_empty,
			_file_io_delete_error_denied,
			_file_io_delete_error_unknown = 0,

			_file_io_delete_error
		};
	};

	namespace FileIO {
		struct s_file_info {
			struct {
				bool is_readable;
				bool is_writable;
				bool is_memory_mapped;
				unsigned char : 8;
			} m_flags;

			HANDLE file_handle;
			HANDLE file_mapping_handle;
			DWORD  file_size;

			void *data_pointer;
			DWORD data_length;

			s_file_info() :
				file_handle(INVALID_HANDLE_VALUE),
				file_mapping_handle(INVALID_HANDLE_VALUE),
				file_size(0),
				data_pointer(nullptr),
				data_length(0) {
				m_flags.is_readable = false;
				m_flags.is_writable      = false;
				m_flags.is_memory_mapped = false;
			}
		};

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Converts a windows file open error number to a file io enum. </summary>
		/// <param name="error">	The error number. </param>
		/// <returns>	The file io enum value. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_open_error GetOpenErrorEnum(DWORD error) {
			switch (error) {
				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
					return Enums::_file_io_open_error_file_does_not_exist;

				case ERROR_SHARING_VIOLATION:
				case ERROR_LOCK_VIOLATION:
					return Enums::_file_io_open_error_file_in_use;

				default:
					return Enums::_file_io_open_error_unknown;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Converts a windows file delete error number to a file io enum. </summary>
		/// <param name="error">	The error number. </param>
		/// <returns>	The file io enum value. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_delete_error GetDeleteErrorEnum(DWORD error) {
			switch (error) {
				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
					return Enums::_file_io_delete_error_does_not_exist;

				case ERROR_SHARING_VIOLATION:
				case ERROR_LOCK_VIOLATION:
				case ERROR_CURRENT_DIRECTORY:
					return Enums::_file_io_delete_error_in_use;

				case ERROR_ACCESS_DENIED:
					return Enums::_file_io_delete_error_denied;

				default:
					return Enums::_file_io_delete_error_unknown;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Builds a directory tree. </summary>
		/// <param name="directory_tree">	The directory tree to create. </param>
		///-------------------------------------------------------------------------------------------------
		bool BuildDirectoryTree(const char *directory_tree) {
			std::string            directory(directory_tree);
			std::string::size_type offset = 0;

			bool success = true;

			// look for all instances of \ and /, and create the directory tree for each one
			while ((offset = directory.find_first_of("\\/", offset)) != std::string::npos) {
				// TODO: a memory optimization trick we could probably do would be to std::swap the character
				// at [offset] with '\0' before and after the mkdir call, thus avoiding the need to substring
				std::string path = directory.substr(0, offset);
				if (_mkdir(path.c_str()) != 0) {
					if (errno != EEXIST) {
						success = false;
						break;
					}
				}
				offset++;
			}

			return success;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Queries if a given path exists. </summary>
		/// <param name="path">	Path to test. </param>
		/// <returns>	true if it exists, false if it doesn't. </returns>
		///-------------------------------------------------------------------------------------------------
		bool PathExists(const char *path) {
			if (GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES)
				return true;

			DWORD error = GetLastError();

			if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
				return false;

			// there are other cases we may wish to explicitly check for...

			return false;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Combines a set of directories into a single path string. </summary>
		/// <param name="destination"> 	[out] The destination buffer for the path. </param>
		/// <param name="append_slash">	true to append a slash to the end. </param>
		/// <param name="path_count">  	The number of paths being combined. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool PathBuild(char *destination, bool append_slash, uint path_count, ...) {
			destination[0] = 0;

			va_list list;
				va_start(list, path_count);

			bool        success = true;
			for (uint i       = 0; success && (i < path_count); i++) {
				const char *path = va_arg(list, const char*);

				if (!PathCombine(destination, destination, path))
					success = false;
			}

				va_end(list);

			if (success && append_slash)
				AppendDirectorySlash(destination, MAX_PATH);

			return success;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Appends a directory slash to a path string. </summary>
		/// <param name="path">			 	[in,out] The character buffer containing the path. </param>
		/// <param name="length">		 	The length of the buffer. </param>
		/// <param name="separator_char">	(Optional) The separator character to append. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AppendDirectorySlash(char *path, uint length, const char separator_char = '\\') {
			const char *final_char = strrchr(path, '\0');

			if (!final_char)
				return false;

			char separator[2] = {0, 0};
			separator[0] = separator_char;

			errno_t result = k_errnone;
			final_char--;
			if (*final_char != separator_char)
				result = strcat_s(path, length, separator);

			return (result == k_errnone);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the directory path from a path with a file name. </summary>
		/// <param name="destination">	[out] The destination buffer for the path. </param>
		/// <param name="size">		  	The size of the destination buffer. </param>
		/// <param name="path">		  	Full pathname of the file. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool GetDirectoryPath(char *destination, uint size, const char * path) {
			if (!destination || (size <= 1))
				return false;

			// find the last occurrence of '\\'
			const char *filename_offset = strrchr(path, '\\');
			if (!filename_offset)
				return false;

			destination[0] = '\0';
			// calculate the character index from the two pointers
			uint index = CAST(uint, filename_offset - path);
			// include the path divider
			index++;
			// copy the directory path to the destination
			errno_t error = strncpy_s(destination, size, path, index);

			return error == k_errnone;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a file extension from a path. </summary>
		/// <param name="destination">	[out] The destination buffer for the extension. </param>
		/// <param name="size">		  	The size of the destination buffer. </param>
		/// <param name="path">		  	Full pathname of the file. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool GetFileExtension(char *destination, uint size, const char * path) {
			const char *extension_start = strrchr(path, '.');
			if (!extension_start)
				return false;

			const char *string_end = strrchr(extension_start, '\0');
			// I'd like to see this ever happen...
			if (!string_end)
				return false;

			if ((string_end - 1) == extension_start)
				return true;

			if (k_errnone != strcpy_s(destination, size, extension_start))
				return false;

			return true;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Deletes a file from the disc. </summary>
		/// <param name="path">	Full pathname of the file. </param>
		/// <returns>	An Enums::file_io_delete_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_delete_error FileDelete(const char *path) {
			if (!PathExists(path))
				return Enums::_file_io_delete_error_does_not_exist;

			BOOL success = DeleteFile(path);

			if (!success) {
				DWORD error = GetLastError();

				return GetDeleteErrorEnum(error);
			}

			return Enums::_file_io_delete_error_none;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Deletes a directory from the disc. </summary>
		/// <param name="path">			  	Full path of the directory. </param>
		/// <param name="delete_contents">
		/// 	True to delete the contents before deleting the directory.
		/// </param>
		/// <param name="recursive">
		/// 	True to delete all files and directories within the directory.
		/// </param>
		/// <returns>	An Enums::file_io_delete_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_delete_error DirectoryDelete(const char * path, const bool delete_contents, const bool recursive) {
			if (!PathExists(path))
				return Enums::_file_io_delete_error_does_not_exist;

			if (delete_contents) {
				WIN32_FIND_DATAA file_search;

				char search_filter[MAX_PATH] = "";
				PathCombine(search_filter, path, "*");

				// returns the "." directory
				HANDLE search_handle = FindFirstFileA(search_filter, &file_search);
				if (search_handle != INVALID_HANDLE_VALUE) {
					// returns the ".." directory
					FindNextFile(search_handle, &file_search);

					char deletion_path[MAX_PATH] = "";
					// returns the first actual file/directory
					while (FindNextFile(search_handle, &file_search)) {
						PathCombine(deletion_path, path, file_search.cFileName);

						if (recursive && ((file_search.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)) {
							auto recursive_error = DirectoryDelete(deletion_path, delete_contents, recursive);

							if (Enums::_file_io_delete_error_none != recursive_error)
								return recursive_error;
						} else {
							auto file_delete_error = FileDelete(deletion_path);

							if (Enums::_file_io_delete_error_none != file_delete_error) {
								return file_delete_error;
							}
						}
					}

					FindClose(search_handle);
				}
			}

			BOOL success = RemoveDirectory(path);

			if (!success) {
				DWORD error = GetLastError();

				return GetDeleteErrorEnum(error);
			}

			return Enums::_file_io_delete_error_none;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Updates the file size of the file described by info. </summary>
		/// <param name="info">	[in,out] The file info to update. </param>
		///-------------------------------------------------------------------------------------------------
		void UpdateFileSize(s_file_info &info) {
			info.file_size = GetFileSize(info.file_handle, nullptr);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Closes a file. </summary>
		/// <param name="info_out">	[in,out] The file info containing the file handle to close. </param>
		///-------------------------------------------------------------------------------------------------
		void CloseFile(s_file_info &info) {
			// reset flags
			info.m_flags.is_readable = false;
			info.m_flags.is_writable = false;

			// either delete the file in memory or unmap it
			if (info.m_flags.is_memory_mapped) {
				UnmapViewOfFile(info.data_pointer);
				CloseHandle(info.file_mapping_handle);
				info.file_mapping_handle = INVALID_HANDLE_VALUE;
			} else
				SafeDeleteArray(info.data_pointer);
			info.file_size           = 0;
			info.data_length         = 0;

			// release the file
			CloseHandle(info.file_handle);
			info.file_handle = INVALID_HANDLE_VALUE;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Opens a file. </summary>
		/// <param name="info_out">			[out] The file info struct to fill. </param>
		/// <param name="file_path">		Full pathname of the file. </param>
		/// <param name="access_type">  	(Optional) Type of file access to open the file with. </param>
		/// <param name="creation_type">
		/// 	(Optional) The behaviour to take with missing/existing files.
		/// </param>
		/// <returns>	An Enums::file_io_open_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_open_error OpenFile(s_file_info &info_out,
													  const char * file_path,
													  const Enums::file_io_open_access_type access_type,
													  const Enums::file_io_open_create_option creation_type) {
			if (!PathExists(file_path) && (creation_type == Enums::_file_io_open_create_option_open_existing))
				return Enums::_file_io_open_error_file_does_not_exist;

			// create the access and sharing flags value for reading and/or writing
			DWORD access_flags  = 0;
			DWORD sharing_flags = 0;
			if (Enums::_file_io_open_access_type_read & access_type) {
				info_out.m_flags.is_readable = true;
				access_flags |= GENERIC_READ;
				sharing_flags |= FILE_SHARE_READ;
			}
			if (Enums::_file_io_open_access_type_write & access_type) {
				info_out.m_flags.is_writable = true;
				access_flags |= GENERIC_WRITE;
				sharing_flags |= FILE_SHARE_WRITE;
			}

			// create the creation flags for when a file does not exist
			DWORD creation_flags = 0;
			switch (creation_type) {
				case Enums::_file_io_open_create_option_open_existing:
					creation_flags = OPEN_EXISTING;
					break;
				case Enums::_file_io_open_create_option_new:
					creation_flags = CREATE_ALWAYS;
					break;
				case Enums::_file_io_open_create_options_create_if_missing:
					creation_flags = OPEN_ALWAYS;
					break;
			}

			// create/open the file
			info_out.file_handle = CreateFile(file_path,
														 access_flags, sharing_flags, nullptr, creation_flags, FILE_ATTRIBUTE_NORMAL, nullptr);

			// report errors
			if (info_out.file_handle == INVALID_HANDLE_VALUE)
				return GetOpenErrorEnum(GetLastError());

			info_out.file_size = GetFileSize(info_out.file_handle, nullptr);

			return Enums::_file_io_open_error_none;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Memory maps a file on disc. </summary>
		/// <param name="info_out">		[out] The file info struct to fill. </param>
		/// <param name="offset">	   	(Optional) The offset in the file to start the map at. </param>
		/// <param name="mapping_size">	(Optional) The size of the mapping. </param>
		/// <returns>	An Enums::file_io_read_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_read_error MemoryMapFile(s_file_info &info, DWORD offset = 0, DWORD mapping_size = 0) {
			// create access flags for the file mapping matching those the file was opened with
			DWORD access_flags = 0;
			if (info.m_flags.is_readable && info.m_flags.is_writable)
				access_flags = PAGE_READWRITE;
			else if (info.m_flags.is_readable && !info.m_flags.is_writable)
				access_flags |= PAGE_READONLY;
			else if (!info.m_flags.is_readable && info.m_flags.is_writable)
				return Enums::_file_io_read_error_writeonly_mapping;

			info.file_mapping_handle = CreateFileMapping(info.file_handle, nullptr, access_flags, 0, 0, nullptr);

			if (info.file_mapping_handle == INVALID_HANDLE_VALUE)
				return Enums::_file_io_read_error_failed_to_memory_map_file;

			// create access flags for the view mapping that match those the file were opened with
			access_flags = 0;
			if (info.m_flags.is_readable)
				access_flags |= FILE_MAP_READ;
			if (info.m_flags.is_writable)
				access_flags |= FILE_MAP_WRITE;

			// map the requested section of the file
			info.data_pointer = MapViewOfFile(info.file_mapping_handle, access_flags, offset, 0, mapping_size);
			if (!info.data_pointer)
				return Enums::_file_io_read_error_failed_to_memory_map_file;

			if (mapping_size == 0)
				info.data_length = info.file_size;
			else
				info.data_length = mapping_size;

			info.m_flags.is_memory_mapped = true;
			return Enums::_file_io_read_error_none;
		}
		//std::size(

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Reads a file to memory handled outside of the file io system. </summary>
		/// <remarks>
		/// 	Reads from an opened file into an external pointer. This function will allocate memory
		/// 	for the read data and store the pointer in data_out. The memory allocated is NOT release
		/// 	when the file is closed so must be deleted elsewhere.
		/// </remarks>
		/// <param name="info">	   	[in,out] The file info to read from. </param>
		/// <param name="data_out">	[out] Pointer to the pointer to read the data to. </param>
		/// <param name="offset">  	The offset in the file to read from. </param>
		/// <param name="length">  	The length of data to read. </param>
		/// <returns>	An Enums::file_io_read_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_read_error ReadFileToMemory(s_file_info &info, void **data_out, DWORD offset, DWORD length) {
			// check the file is valid for reading
			if (!data_out)
				return Enums::_file_io_read_error_destination_pointer_is_null;
			if (!info.m_flags.is_readable)
				return Enums::_file_io_read_error_file_is_writeonly;
			if (info.m_flags.is_memory_mapped)
				return Enums::_file_io_read_error_file_is_memory_mapped;

			UpdateFileSize(info);

			if (info.file_size == 0)
				return Enums::_file_io_read_error_file_is_empty;

			// zero length defaults to reading the entire file
			if (length == 0)
				length = info.file_size;

			// must not attempt to read beyond the end of the file
			if (info.file_size < (offset + length))
				return Enums::_file_io_read_error_reading_past_eof;

			*data_out = new byte[length];

			// read the file section
			DWORD bytes_read = 0;

			OVERLAPPED file_read_setup;
			file_read_setup.hEvent     = nullptr;
			file_read_setup.Offset     = offset;
			file_read_setup.OffsetHigh = 0;
			BOOL success = ReadFile(info.file_handle, *data_out, length, &bytes_read, &file_read_setup);

			if (!success || (bytes_read != length)) {
				delete[] *data_out;
				*data_out = nullptr;
				return Enums::_file_io_read_error_failed_to_read_from_file;
			}

			return Enums::_file_io_read_error_none;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Reads a file to memory stored in the info struct. </summary>
		/// <remarks>
		/// 	Reads a section of a file into memory. This function will allocate memory for the data,
		/// 	with the pointer stored in the s_file_info struct. If memory is already allocated it is
		/// 	deleted and replaced.The memory allocated here is deleted when the file is closed. This
		/// 	function is meant for reading a single block from a file that rarely changes if
		/// 	ever.< / remarks>
		/// </remarks>
		/// <param name="info">  	[in,out] The file info struct to read. </param>
		/// <param name="offset">	The offset in the file to read from. </param>
		/// <param name="length">	The length of data to read. </param>
		/// <returns>	An Enums::file_io_read_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_read_error ReadFileToInfoMemory(s_file_info &info, DWORD offset, DWORD length) {
			// delete the existing section
			SafeDeleteArray(info.data_pointer);

			info.data_length = 0;
			Enums::file_io_read_error success = ReadFileToMemory(info, &info.data_pointer, offset, length);
			if (success != Enums::_file_io_read_error_none)
				return success;

			// zero length defaults to reading the entire file
			if (length == 0)
				info.data_length = info.file_size;
			else
				info.data_length = length;

			return Enums::_file_io_read_error_none;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Writes to a file. </summary>
		/// <param name="info">  	[in,out] The file info to write with. </param>
		/// <param name="data">  	The data to write. </param>
		/// <param name="length">	The length of data to write. </param>
		/// <param name="offset">	The offset in the file to start writing from. </param>
		/// <returns>	An Enums::file_io_write_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_write_error WriteToFile(s_file_info &info, const void *data, DWORD length, DWORD offset = NONE) {
			if (!info.m_flags.is_writable)
				return Enums::_file_io_write_error_file_is_readonly;

			UpdateFileSize(info);

			// if no offset is set, write from the end of the file
			if (offset == NONE)
				offset = info.file_size;

			// must not attempt to write to an offset beyond the end of the file
			if (offset > info.file_size)
				return Enums::_file_io_write_error_write_offset_past_eof;

			DWORD      bytes_written = 0;
			OVERLAPPED file_read_setup;
			file_read_setup.hEvent     = nullptr;
			file_read_setup.Offset     = offset;
			file_read_setup.OffsetHigh = 0;
			BOOL success = WriteFile(info.file_handle, data, length, &bytes_written, &file_read_setup);

			UpdateFileSize(info);

			if (!success || (bytes_written != length))
				return Enums::_file_io_write_error_failed_to_write_to_file;

			return Enums::_file_io_write_error_none;
		}
	};
};
