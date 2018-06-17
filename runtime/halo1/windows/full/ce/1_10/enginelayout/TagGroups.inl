
#include <engine_functions.h>

#ifdef ENGINE_DPTR
#undef ENGINE_DPTR
#endif

#ifdef ENGINE_PTR
#undef ENGINE_PTR
#endif

#ifdef FUNC_PTR
#undef FUNC_PTR
#endif

#define ENGINE_DPTR(type, name, ...) static auto** const name = reinterpret_cast<type**>(__VA_ARGS__);
#define ENGINE_PTR(type, name, ...) static auto* const name = reinterpret_cast<type*>(__VA_ARGS__);
#define DUO_PTR(...) __VA_ARGS__
#define FUNC_PTR(name, value, ...) static auto const name  =  value
#define CAST_PTR(type, ptr)      reinterpret_cast<type>(ptr)

//////////////////////////////////////////////////////////////////////////
// CacheFiles.cpp
	namespace DataFiles
	{
		static auto const CACHE_FILES_INITIALIZE = K_CACHE_FILES_INITIALIZE;
		static auto const CACHE_FILES_INITIALIZE_HOOK = K_CACHE_FILES_INITIALIZE_HOOK;
		static auto const CACHE_FILES_INITIALIZE_RETN = K_CACHE_FILES_INITIALIZE_RETN;
		static auto const CACHE_FILES_DISPOSE = K_CACHE_FILES_DISPOSE;
		static auto const CACHE_FILES_DISPOSE_HOOK = K_CACHE_FILES_DISPOSE_HOOK;
		static auto const CACHE_FILES_DISPOSE_RETN = K_CACHE_FILES_DISPOSE_RETN;
		static auto const DATA_FILE_READ = K_DATA_FILE_READ;

		static auto const SCENARIO_TAGS_LOAD_HOOK = K_SCENARIO_TAGS_LOAD_HOOK;
	};

	namespace Cache
	{
		static uint* K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_LIST[] = {
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_0),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_1),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_2),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_3),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_4),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_5),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_6),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_7),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_8),
			reinterpret_cast<uint *>(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_9),
		};

		static void* K_CACHE_PATH_SPRINTF_CALLS[] = {
			reinterpret_cast<void *>(K_CACHE_PATH_SPRINTF_CALL_0),
			reinterpret_cast<void *>(K_CACHE_PATH_SPRINTF_CALL_1),
			reinterpret_cast<void *>(K_CACHE_PATH_SPRINTF_CALL_2),
		};
		static void* K_CACHE_PATH_SNPRINTF_CALLS[] = {
			NULL,
		};

		static void* K_CACHE_FILE_READ_HEADER_CALLS[] = {
			reinterpret_cast<void *>(K_CACHE_FILE_READ_HEADER_CALL_0),
			reinterpret_cast<void *>(K_CACHE_FILE_READ_HEADER_CALL_1),
			reinterpret_cast<void *>(K_CACHE_FILE_READ_HEADER_CALL_2),
		};

		static const char ** K_UI_SCENARIO_NAME_REFERENCES[] = {
			reinterpret_cast<const char * *>(K_UI_SCENARIO_NAME_REFERENCE_0),
			reinterpret_cast<const char * *>(K_UI_SCENARIO_NAME_REFERENCE_1),
		};

		static auto *const cache_file_globals = reinterpret_cast<s_cache_file_globals *>(K_CACHE_FILE_GLOBALS);;
		static auto *const maps_folder_parent_dir = reinterpret_cast<char *>(K_MAPS_FOLDER_PARENT_DIR);;
	};
	namespace Interface
	{
		static auto *const multiplayer_maps = reinterpret_cast<map_list_data_t *>(K_MULTIPLAYER_MAPS);;
		static auto const MULTIPLAYER_MAP_LIST_INITIALIZE = K_MULTIPLAYER_MAP_LIST_INITIALIZE;
		static auto const MULTIPLAYER_MAP_LIST_DISPOSE = K_MULTIPLAYER_MAP_LIST_DISPOSE;
	};

//////////////////////////////////////////////////////////////////////////
// TagGroups.cpp
static auto **const tag_instances = reinterpret_cast<Cache::s_cache_tag_instance **>(K_TAG_INSTANCES);;
