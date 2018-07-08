/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <precompile.h>

#include "../cseries/base.h"
#include "files.hpp"
#include "../cseries/yelo_base.h"
#include "../cseries/errors.h"

namespace Yelo
{
	namespace FileIO
	{
		Enums::file_io_open_error OpenFileByID(s_file_info& info_out, const char * file_id);
	};

	class c_packed_file
	{
		struct s_header
		{
			enum {
				k_header_signature = 'head',
				k_footer_signature = 'foot',
			};

			uint header;
			uint file_size;
			uint element_count;
			uint footer;

			inline bool IsValid()
			{
				return header == k_header_signature &&
					footer == k_footer_signature;
			}

			inline void Ctor()
			{
				header = k_header_signature;
				file_size = element_count = 0;
				footer = k_footer_signature;
			}
		};

		struct s_element
		{
			uint element_id_offset;
			uint element_size;
			uint element_offset;
		};

		FileIO::s_file_info	file_info;
		union {
			void*		m_address;
			s_header*	m_header;

			// utility member for pointer math
			uint		m_base_address;
		};
		bool			m_file_mapped;
		unsigned char : 8; unsigned short : 16;
	public:
		struct s_element_editor : s_element
		{
			char* source_id;
			char* source_data;

			s_element_editor() : source_id(nullptr), source_data(nullptr) {}
			void Delete()
			{
				SafeDeleteArray(source_id);
				SafeDeleteArray(source_data);
			}
		};
	private:

		s_header						m_header;
		std::vector<s_element_editor>	m_elements;

	public:
		// Opens a packed file and memory maps it, read for accessing by pointer
		void OpenFile(const char * packed_file, bool is_file_id = false) {
			Enums::file_io_open_error open_success;
			if(is_file_id) {
				open_success = FileIO::OpenFileByID(file_info, packed_file);
			} else {
				open_success = FileIO::OpenFile(file_info, packed_file);
			}

			if(open_success != Enums::_file_io_open_error_none) return;

			Enums::file_io_read_error read_success = FileIO::MemoryMapFile(file_info);

			if(read_success != Enums::_file_io_read_error_none) return;

			m_address = file_info.data_pointer;

			if(m_header != nullptr && m_header->IsValid()){
				m_file_mapped = true;
			}
		}

		// Releases the mapped file
		void CloseFile(){
			if(m_file_mapped)
			{
				m_header = nullptr;

				FileIO::CloseFile(file_info);

				m_file_mapped = false;
			}
		}
		// Returns a pointer to a block of data reference by index. Returns NULL if invalid.
		// The data block size is put into data_size if not null.
		void* GetDataPointer(uint index, _Out_opt_ uint* data_size) {
			if(!m_file_mapped || index > m_header->element_count)
				return nullptr;

			s_element* element = reinterpret_cast<s_element *>(m_base_address + sizeof(s_header) + (sizeof(s_element) * index));

			if(data_size)
				*data_size = element->element_size;

			return reinterpret_cast<void *>(m_base_address + element->element_offset);
		}
		// Returns a pointer to a block of data reference by a string id. Returns NULL if invalid.
		// The data block size is put into data_size if not null.
		void* GetDataPointer(const char * data_id, _Out_opt_ uint* data_size) {
			if(!m_file_mapped || is_null_or_empty(data_id))
				return nullptr;

			for(uint i = 0; i < m_header->element_count; i++)
			{
				s_element* element = reinterpret_cast<s_element *>(m_base_address + sizeof(s_header) + (sizeof(s_element) * i));

				if(strcmp(data_id, reinterpret_cast<char *>(m_base_address + (uint) element->element_id_offset)) == 0)
				{
					if(data_size)
						*data_size = element->element_size;

					return reinterpret_cast<void *>(m_base_address + element->element_offset);
				}
			}

			if(data_size)
				*data_size = 0;
			return nullptr;
		}
		// Returns whether the file has been successfully mapped
		inline bool IsFileMapped() const { return m_file_mapped; }

		c_packed_file() {
			m_header.Ctor();

			m_elements.clear();
			m_header.element_count = 0;
		}

		~c_packed_file() {
			for(auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
				iter->Delete();
			m_elements.clear();

			m_header.element_count = 0;
			m_header.file_size = 0;
		}

		void CalculateOffsets() {
			uint id_base_offset = sizeof(s_header) + (sizeof(s_element) * m_elements.size());
			uint id_offset = 0;

			for(auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
			{
				iter->element_id_offset = id_base_offset + id_offset;
				id_offset += strlen(iter->source_id) + 1;
			}

			uint data_base_offset = id_base_offset + id_offset;
			uint data_offset = 0;

			for(auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
			{
				iter->element_offset = data_base_offset + data_offset;
				data_offset += iter->element_size;
			}

			m_header.file_size = data_base_offset + data_offset;
		}

		HRESULT Save(const char * save_location) {
			CalculateOffsets();

			m_header.element_count = m_elements.size();

			if(m_header.element_count == 0)
			{
				YELO_WARN("c_packed_file: no elements to pack for %s", save_location);
				return E_FAIL;
			}

			std::ofstream file(save_location, std::ios::out | std::ios::binary);
			if(file.fail())
			{
				YELO_WARN("c_packed_file: failed to open file for writing (%s) %s",
							 "file may be in use?", save_location);

				return E_FAIL;
			}

			file.write(reinterpret_cast<char *>(&m_header), sizeof(m_header));

			for(auto iter = m_elements.cbegin(); iter != m_elements.cend(); ++iter)
				file.write(reinterpret_cast<const char *>(&(*iter)), sizeof(s_element));

			char null_char = 0;
			for(auto iter = m_elements.cbegin(); iter != m_elements.cend(); ++iter)
			{
				file.write(iter->source_id, strlen(iter->source_id));
				file.write(&null_char, sizeof(null_char));
			}

			for(auto iter = m_elements.cbegin(); iter != m_elements.cend(); ++iter)
				file.write(reinterpret_cast<char *>(iter->source_data), iter->element_size);

			file.flush();
			file.close();

			return S_OK;
		}

		void AddElement(s_element_editor& element) {
			m_elements.push_back(element);
		}
	};
};
