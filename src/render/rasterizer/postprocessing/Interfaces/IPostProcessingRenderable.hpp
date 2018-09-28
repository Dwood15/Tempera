
#pragma once

#include <enums/postprocessing.h>

namespace Yelo::Rasterizer::PostProcessing {
	struct IPostProcessingRenderable {
		virtual bool Render(Enums::postprocess_render_stage render_stage) { return false; }
	};
};
