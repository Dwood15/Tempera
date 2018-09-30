#pragma once

#include <macros_generic.h>

namespace Yelo::Rasterizer::PostProcessing::Scripting {
	void *HS_Load();

	void *HS_Unload();

	namespace Globals {
		bool Enabled_Internal();

		bool Enabled_External();

		bool Enabled_MotionBlur();

		bool Enabled_FXAA();

		real MotionBlur_Amount();
	};

	namespace Bloom {
		void *HS_BloomSetSize(void **arguments);

		void *HS_BloomSetExposure(void **arguments);

		void *HS_BloomSetMixAmount(void **arguments);

		void *HS_BloomSetMinimumColor(void **arguments);

		void *HS_BloomSetMaximumColor(void **arguments);
	};

	namespace Internal {
		void *HS_SetEffectInstanceActive(void **arguments);

		void *HS_SetEffectInstanceFade(void **arguments);

		void *HS_GetEffectInstanceIndexByName(void **arguments);

		void *HS_GetEffectInstanceCurrentFade(void **arguments);

		void *HS_GetEffectInstanceFadeDirection(void **arguments);

		void *HS_GetEffectIndexByName(void **arguments);

		void *HS_GetEffectIsValid(void **arguments);

		void *HS_GetEffectShaderVariableIndexByName(void **arguments);

		void *HS_SetEffectShaderVariableBoolean(void **arguments);

		void *HS_SetEffectShaderVariableInteger(void **arguments);

		void *HS_SetEffectShaderVariableReal(void **arguments);

		void *HS_SetEffectShaderInstanceActive(void **arguments);
	};
};
