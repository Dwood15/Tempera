/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include "macros_generic.h"
#include "files.hpp"
#include "../cseries/yelo_base.h"
#include "../cseries/errors.h"

namespace Yelo {
	namespace FileIO {
		Enums::file_io_open_error OpenFileByID(s_file_info &info_out, const char *file_id) {
			return Enums::_file_io_open_error;
		}
	};

	class c_packed_file {
		struct s_header {
			enum {
				k_header_signature = 'head',
				k_footer_signature = 'foot',
			};

			uint header;
			uint file_size;
			uint element_count;
			uint footer;

			inline bool IsValid() {
				return header == k_header_signature &&
						 footer == k_footer_signature;
			}

			inline void Ctor() {
				header    = k_header_signature;
				file_size = element_count = 0;
				footer    = k_footer_signature;
			}
		};

		struct s_element {
			uint element_id_offset;
			uint element_size;
			uint element_offset;
		};

		FileIO::s_file_info file_info;
		union {
			void     *m_address;
			s_header *m_header;

			// utility member for pointer math
			uint m_base_address;
		};
		bool                m_file_mapped;
		unsigned char : 8;
		unsigned short : 16;
	public:
		struct s_element_editor : s_element {
			char *source_id;
			char *source_data;

			s_element_editor() : source_id(nullptr), source_data(nullptr) {}

			void Delete() {
				SafeDeleteArray(source_id);
				SafeDeleteArray(source_data);
			}
		};

		// Opens a packed file and memory maps it, read for accessing by pointer
		void OpenFile(const char *packed_file, bool is_file_id = false) {
			// Enums::file_io_open_error open_success;
			// if(is_file_id) {
			// 	open_success = FileIO::OpenFileByID(file_info, packed_file);
			// } else {
			// 	open_success = FileIO::OpenFile(file_info, packed_file);
			// }
			//
			// if(open_success != Enums::_file_io_open_error_none) return;
			//
			// Enums::file_io_read_error read_success = FileIO::MemoryMapFile(file_info);
			//
			// if(read_success != Enums::_file_io_read_error_none) return;
			//
			// m_address = file_info.data_pointer;
			//
			// if(m_header != nullptr && m_header->IsValid()){
			// 	m_file_mapped = true;
			// }
		}

		// Releases the mapped file
		void CloseFile() {
			if (m_file_mapped) {
				m_header = nullptr;

				FileIO::CloseFile(file_info);

				m_file_mapped = false;
			}
		}

		// Returns a pointer to a block of data reference by index. Returns NULL if invalid.
		// The data block size is put into data_size if not null.
		void *GetDataPointer(uint index, _Out_opt_ uint *data_size) {
			if (!m_file_mapped || index > m_header->element_count)
				return nullptr;

			s_element *element = reinterpret_cast<s_element *>(m_base_address + sizeof(s_header) + (sizeof(s_element) * index));

			if (data_size)
				*data_size = element->element_size;

			return reinterpret_cast<void *>(m_base_address + element->element_offset);
		}

		// Returns a pointer to a block of data reference by a string id. Returns NULL if invalid.
		// The data block size is put into data_size if not null.
		void *GetDataPointer(const char *data_id, _Out_opt_ uint *data_size) {
			if (!m_file_mapped || is_null_or_empty(data_id))
				return nullptr;

			for (uint i = 0; i < m_header->element_count; i++) {
				s_element *element = reinterpret_cast<s_element *>(m_base_address + sizeof(s_header) + (sizeof(s_element) * i));

				if (strcmp(data_id, reinterpret_cast<char *>(m_base_address + (uint) element->element_id_offset)) == 0) {
					if (data_size)
						*data_size = element->element_size;

					return reinterpret_cast<void *>(m_base_address + element->element_offset);
				}
			}

			if (data_size)
				*data_size = 0;
			return nullptr;
		}

		// Returns whether the file has been successfully mapped
		inline bool IsFileMapped() const { return m_file_mapped; }
	};
};
