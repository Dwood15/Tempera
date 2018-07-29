/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <cstdio>
#include <enums/ai_enums.h>
#include "../../math/colors.h"
#include "../../memory/datum_index.h"
namespace Yelo {

	namespace AI {
		typedef void (__cdecl *proc_action_function)(datum_index actor_index);

		typedef void (__cdecl *proc_action_function_2C)(datum_index actor_index, datum_index, datum_index);

		struct s_action_function_definition {
			Enums::actor_action action;
			unsigned short : 16;
			const char *               name;
			const real_argb_color debug_color;
			size_t                action_data_size;
			Enums::action_class   action_class;
			unsigned short : 16;
			proc_action_function    begin;
			proc_action_function    perform;
			proc_action_function    update;
			proc_action_function    control;
			proc_action_function    end;
			proc_action_function    func28;
			proc_action_function_2C func2C;
			proc_action_function    func30;
			proc_action_function    func34;
			proc_action_function    func38;
		};
	};
};
