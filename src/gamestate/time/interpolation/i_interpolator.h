#pragma once

#include <macros_generic.h>

namespace Yelo::Time::Interpolation {
	class i_interpolator {
	public:
		virtual void Ctor() {}

		virtual void Dtor() {}

		virtual real *GetValues() { return nullptr; }

		virtual void Update(real delta_time) {}
	};
};
