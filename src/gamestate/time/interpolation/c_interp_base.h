#pragma once

#include <array>

namespace Yelo {
	namespace Time {
		namespace Interpolation {
			template <int ValueCount>
			class c_interp_base : public i_interpolator {

			private:
				void ClearMembers() {
					m_members.values.fill(0.0f);
				}

			public:
				struct {
					std::array<real, ValueCount> values;
				} m_members;

				real *GetValues() override {
					return m_members.values.data();
				}

				virtual void Ctor() override {
					ClearMembers();
				}

				virtual void Dtor() override {
					ClearMembers();
				}

				void Update(real delta_time) override = 0;
			};
		};
	};
};
