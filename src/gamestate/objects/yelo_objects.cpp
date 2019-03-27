#pragma once

#include <macros_generic.h>
#include <functional>
#include "yelo_objects.h"
#include "../objects.h"
#include "../../models/animations/model_definitions.h"
#include "../../game/objects/objects.h"

namespace Yelo::Objects {
	datum_index GetUltimateObject(const datum_index object_index) {
		if (object_index.IsNull()) {
			return object_index;
		}

		// const auto *object_header_datums = nullptr; //Objects::ObjectHeader().Datums();
		// const auto *current_obj          = object_header_datums[object_index.index]._object;
		//
		// datum_index parent_index;
		// while (!(parent_index = current_obj->parent_object_index).IsNull()) {
		// 	current_obj = object_header_datums[parent_index.index]._object;
		// }

		return object_index;
	}

	datum_index GetNextObjectN(datum_index object_index, int32 step_count) {
		if (!object_index.IsNull()) {
			// const auto *object_header_datums = nullptr; //Objects::ObjectHeader().Datums();
			// const auto *current_obj          = object_header_datums[object_index.index]._object;
			//
			// for (object_index = current_obj->next_object_index; !object_index.IsNull() && step_count > 0; object_index = current_obj->next_object_index, --step_count) {
			// 	current_obj = object_header_datums[object_index.index]._object;
			// }
		}

		return step_count == 0 ? object_index : datum_index::null();
	}

	TagGroups::s_object_definition *GetObjectDefinition(const datum_index object_index) {
		return nullptr;
		// if (object_index.IsNull()) {
		// 	return nullptr;
		// }
		//
		// // const auto *object = blam::object_get(object_index);
		//
		// return blam::tag_get<TagGroups::s_object_definition>(object->definition_index);
	}

	TagGroups::model_animation_graph const *GetObjectAnimations(const datum_index object_index) {
		return nullptr;
		// if (object_index.IsNull()) {
		// 	return nullptr;
		// }
		//
		// const auto *object   = blam::object_get(object_index);
		// auto       tag_index = object->animation.definition_index;
		//
		// return blam::tag_get<TagGroups::model_animation_graph>(tag_index);
	}

	real GetObjectDistanceFromPoint(const datum_index object_index, const real_vector3d &destination_point) {
		real dist = -1.0f;

		if (object_index.IsNull()) {
			return dist;
		}

		// real_vector3d object_origin;
		// blam::object_get_origin(object_index, object_origin);
		//
		// real_vector3d relative_pos = object_origin - destination_point;
		// dist = relative_pos.Magnitude();

		return dist;
	}
};

