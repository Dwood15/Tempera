#pragma once

#include <handleapi.h>
#include "../cseries/base.h"
#include "cache_files.h"

namespace Yelo::Cache {
	struct s_cache_file_request {
		OVERLAPPED overlapped;
		uint       size;
		void *address;
		bool block;
		bool reading;
		bool running;
		unsigned char : 8;
		::Yelo::Enums::cache_file_request_source source;
		unsigned char : 8;
		unsigned short              : 16;
		s_cache_file_request_params params;
	}; static_assert(sizeof(s_cache_file_request) == 0x30);

	struct s_cached_map_file {
		HANDLE         file_handle;
		FILETIME       file_time;
		s_cache_header header;

		void Close() {
			CloseHandle(file_handle);
			file_handle = INVALID_HANDLE_VALUE; // NOT: engine doesn't actually do this
		}
	};

	static_assert(sizeof(s_cached_map_file) == 0x80C);

	struct s_cache_file_globals {
		// reversed engineered based on halo xbox code...PC code seems to have some slight differences, but it doesn't really use this anyway so who cares
		struct s_decompression_state {
			// struct s_read_request { short read_sequence_index; };
			// struct s_write_request { short write_sequence_index; };
			//
			// char           file_name[_MAX_PATH];
			// s_cache_header header;
			// unsigned long  flags;
			// z_stream       zstream;
			// void *zlib_buffer;
			// uint zlib_buffer_size;
			// void *next_allocation;
			// HANDLE copy_start_event,
			// 		 copy_stop_event,
			// 		 copy_complete_event,
			// 		 progress_update_event;
			// HANDLE copy_thread;
			// void *buffer;
			// void *read_buffers[Enums::k_number_of_cache_read_buffers];
			// void *write_buffers[Enums::k_number_of_cache_write_buffers];
			// UNKNOWN_TYPE(bool); unsigned char : 8;
			// unsigned short                    : 16;
			// HANDLE                            write_file_handle, read_file_handle;
			// unsigned long                     overlapped_in_use_flags[BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_cache_overlapped_structures)];
			// unsigned long                     overlapped_completed_flags[BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_cache_overlapped_structures)];
			// OVERLAPPED                        overlapped[Enums::k_number_of_cache_overlapped_structures];
			// s_read_request                    read_requests[Enums::k_number_of_cache_read_buffers];
			// s_write_request                   write_requests[Enums::k_number_of_cache_write_buffers];
			// unsigned short : 16;
			// uint          read_file_size;
			// uint          async_read_bytes_left, read_bytes_left;
			// uint          async_write_bytes_left, write_bytes_left;
			// real_fraction progress;
			// long          current_write_offset, current_read_offset;
			// s_read_request  *current_request;
			// s_write_request *current_write_request;
			// long write_requests_pending;
			// UNKNOWN_TYPE(short); // a read_sequence_index
			// UNKNOWN_TYPE(short); // a read_sequence_index
			// short current_write_buffer_index;
			// UNKNOWN_TYPE(short); // current_write_sequence_count?
			// short next_write_sequence_index;
			// short current_read_sequence_count;
			// unsigned short : 16; // more than likely an unused next_read_sequence_index
			// unsigned short : 16;
			byte pad[0xAC8];
		};

		static_assert(sizeof(s_decompression_state) == 0xAC8);

		bool tags_loaded;
		unsigned char : 8;
		unsigned short : 16;
		s_cache_header cache_header;
		s_cache_tag_header              *tag_index;
		TagGroups::structure_bsp_header *structure_bsp_header;
		unsigned long : 32;
		s_decompression_state decompression_state;

		s_cached_map_file map_files[Enums::k_number_of_cached_map_files];

		bool copy_in_progress;
		unsigned char : 8;
		short      copy_map_file_index;
		tag_string copy_map_file_name;
		short      open_map_file_index;
		unsigned short : 16;

		HANDLE  sleep_event;
		HANDLE  thread;
		HGLOBAL requests; // pointer to s_cache_file_request[k_maximum_simultaneous_cache_requests]
		unsigned long : 32;

		s_data_file_globals data_files;

		short FindMapFileIndexByName(const char *scenario_name);

		s_cached_map_file *OpenMapFile() {
			return open_map_file_index != NONE
					 ? &map_files[open_map_file_index]
					 : nullptr;
		}

		bool OpenMapFileOpen(const char *scenario_name, s_cache_header *header);

		void OpenMapFileClose();

		s_cache_file_request *Requests() {
			return reinterpret_cast<s_cache_file_request *>(requests);
		}

		// blocks the thread until all read requests have finished
		void RequestsWaitAll();
	};

	static_assert(sizeof(s_cache_file_globals) == 0x4418);
};
