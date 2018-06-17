#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_tool_import_files = 500,

			k_maximum_tool_import_files_upgrade = 2048,
		};
	};

	namespace Tool
	{
		typedef void (__cdecl* proc_import_class)(char* arguments[]);

		struct s_import_class {
			const char *				name;
			const char *				usage;
			long				argument_count;
			proc_import_class	import_proc;

			static int __cdecl CompareProc(void*, const s_import_class* lhs, const s_import_class* rhs);
			static int __cdecl SearchByNameProc(void*, const char * key, const s_import_class* element);
		};
#define IMPORT_CLASS_DEFINITION(name, arg_count, arguments, proc) {name, name " " arguments, arg_count, proc}
	};
};
