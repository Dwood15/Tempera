#include "../math/real_math.h"
#include "../memory/datum_index.h"
#include "../gamestate/objects/objectcontroller.h"
#include "../CurrentEngine.h"

namespace Yelo::blam {
	real_point3d *object_get_origin(datum_index idx, real_point3d &out_origin) {
		auto        current = CurrentEngine.GetGenericObject(idx.index);
		real_point3d *real_current = std::addressof(out_origin);

		if (current->parent_object_index.IsNull()) {
			out_origin.x = current->World.x;
			out_origin.y = current->World.y;
			out_origin.z = current->World.z;
			return real_current;
		}

		static auto matrix4x3_transform_point = CurrentEngine.getFunctionBegin("matrix4x3_transform_point");
		if (matrix4x3_transform_point) {
			throw "Can't evaluate matrix tform point right now.";
			//TODO: Finish this, or wrap in the game engine version of the full function.
			// 			auto parent = core->GetGenericObject(current->parent_object_index.index);
			// 			auto tmp =   52 *current->parent_node_index + parent->node_matrix_block.offset;
			// #ifndef __GNUC__
			// 			__asm mov eax, real_current
			// 			__asm mov eax, &current->World
			// 			::calls::DoCall<Convention::m_cdecl, real_point3d*, >
			// #endif
		}
		return nullptr;
	}

};
