#pragma once

#include <macros_generic.h>

namespace Yelo::Rasterizer::PostProcessing {
	struct IPostProcessingUpdatable {
		virtual void Update(real delta_time) {}
	};
};
