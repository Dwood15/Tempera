#pragma once

#include <windows.h>
#include <addlog.h>
#include "../cseries/base.h"
#include "../cseries/errors.h"
#include "../memory/memory_yelo.hpp"

namespace Yelo {
	namespace Cache {
		struct s_data_file_header {
			signed long type; // Enums::data_file_reference_type
			// when building or updating a data file, acts as the write cursor for new items
			long     file_names_offset;
			long     file_index_table_offset;
			long     tag_count;
		}; static_assert(sizeof(s_data_file_header) == 0x10);

		struct s_data_file_item {
			long name_offset;
			long size;
			long data_offset;
		}; static_assert(sizeof(s_data_file_item) == 0xC);


		struct index_tbl {
			s_data_file_item *address;
			long            count;
		};

		struct data_file_info {
			long total_size;
			long used_size;
			char  *address;

			byte *AsByteBuffer() { return reinterpret_cast<byte *>(address); }

			const byte *AsByteBuffer() const { return reinterpret_cast<const byte *>(address); }
		};

		struct count_w_size {
			long count;
			long size;
		};

		struct s_data_file {
			s_data_file_header header;
			index_tbl          file_index_table;
			data_file_info     file_names;

			bool writable;
			unsigned char : 8; unsigned short : 16;
			count_w_size item_hits;
			count_w_size item_adds_or_misses;
			const char *       name;
			HANDLE       file_handle;

		private:
			void FreeResources() {
				if (file_names.address != nullptr) {
					YELO_DELETE_ARRAY(file_names.address);
				}

				if (file_index_table.address != nullptr) {
					YELO_DELETE_ARRAY(file_index_table.address);
				}
			}

			bool Read(uint position, void *buffer, size_t buffer_size) {
				DWORD bytes_read;

				OVERLAPPED overlapped = {};
				overlapped.Offset = position;
				if (ReadFile(file_handle, buffer, buffer_size, &bytes_read, &overlapped) != FALSE) {
					return bytes_read == buffer_size;
				} else if (GetLastError() == ERROR_IO_PENDING &&
							  GetOverlappedResult(file_handle, &overlapped, &bytes_read, TRUE) != FALSE) {
					return bytes_read == buffer_size;
				}

				return false;
			}

			bool ReadHeader(Enums::data_file_reference_type expected_type) {
				if (!Read(0, &header, sizeof(header))) {
					DWORD last_error = GetLastError();
					//printf_s("Failed to read data file header %s\n", name);
					return false;
				}

				if (header.type != expected_type) {
					memset(&header, 0, sizeof(header));
					//printf_s("Invalid data file id in data file %s\n", name);
					return false;
				}

				return true;
			}

			bool ReadFileNames() {
				DWORD buffer_size = header.file_index_table_offset - header.file_names_offset;
				file_names.address = YELO_NEW_ARRAY(char, buffer_size);

				if (!Read(header.file_names_offset, file_names.address, buffer_size)) {
					DWORD last_error = GetLastError();
					// printf_s("Invalid format in data file %s\n", name);
					return false;
				}

				file_names.total_size = file_names.used_size = buffer_size;

				return true;
			}

			bool ReadFileIndexTable() {
				DWORD buffer_size = sizeof(s_data_file_item) * header.tag_count;
				file_index_table.address = YELO_NEW_ARRAY(s_data_file_item, header.tag_count);

				if (!Read(header.file_index_table_offset, file_index_table.address, buffer_size)) {
					DWORD last_error = GetLastError();
					//printf_s("Invalid format in data file %s\n", name);
					return false;
				}

				file_index_table.count = header.tag_count;

				return true;
			}

		public:

			// full_path should NOT be allocated on the stack, as it will become the value of 'name'
			// TODO: OpenForEdit and OpenForRead
			bool Open(Yelo::Enums::data_file_reference_type data_file, bool store_resources, const char *  full_path) {
				memset(this, 0, sizeof(*this));
				this->name     = full_path;
				this->writable = store_resources;

				DWORD file_access = GENERIC_READ;

				if (store_resources) {
					file_access |= GENERIC_WRITE;
				}

				DWORD file_share_mode = 0;

				DWORD file_flags = FILE_ATTRIBUTE_NORMAL;
				file_flags |= FILE_FLAG_SEQUENTIAL_SCAN;
				file_flags |= FILE_FLAG_OVERLAPPED;

				this->file_handle = CreateFileA(full_path, file_access, file_share_mode, nullptr, OPEN_ALWAYS, file_flags, nullptr);

				if (file_handle != INVALID_HANDLE_VALUE &&
					 ReadHeader(data_file) && ReadFileNames() && ReadFileIndexTable()) {
					SetFilePointer(file_handle, header.file_names_offset, nullptr, FILE_BEGIN);
					return true;
				}

				FreeResources();
				if (store_resources) {
					header.type              = data_file;
					header.file_names_offset = sizeof(header);
					DWORD bytes_written;
					WriteFile(file_handle, &header, sizeof(header), &bytes_written, nullptr);
				}

				DWORD last_error = GetLastError();
				Print<true>("### FAILED TO OPEN DATA-CACHE FILE: %s.\n\n", this->name);
				return false;
			}

			bool Close() {
				CloseHandle(file_handle);
				FreeResources();

				memset(&header, 0, sizeof(header));
				file_handle = INVALID_HANDLE_VALUE; // engine doesn't do this
				return true;
			}

			bool GetItemDataInfo(long item_index, long &out_data_offset, long &out_data_size) const {
				if (item_index < 0 || item_index >= file_index_table.count)
					return false;

				s_data_file_item *item = &file_index_table.address[item_index];
				out_data_offset = item->data_offset;
				out_data_size   = item->size;

				return true;
			}

			bool ReadItemData(uint position, void *buffer, size_t buffer_size) {
				return Read(position, buffer, buffer_size);
			}

		public:
			long GetItemDataOffset(long item_index) {
				if (item_index < 0 || item_index >= header.tag_count) {
					return NONE;
				}

				return file_index_table.address[item_index].data_offset;
			}
		};

		static_assert(sizeof(s_data_file) == 0x40);

		// build_cache_file_globals (tools) and cache_file_globals (runtime) share the same ordering of the s_data_file instances, so I wrap them in this fake struct
		struct s_data_file_globals {
			Cache::s_data_file sounds;
			Cache::s_data_file locale;
			Cache::s_data_file bitmaps;

			s_data_file &Get(Yelo::Enums::data_file_reference_type data_file);

		};

		static_assert(sizeof(s_data_file_globals) == 0xC0);
	};
};
