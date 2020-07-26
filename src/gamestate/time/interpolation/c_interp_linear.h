#pragma once

#include "c_interp_base.h"

namespace Yelo::Time::Interpolation {
	template <int ValueCount>
	class c_interp_linear final : public c_interp_base<ValueCount> {
		struct {
			real current_value;
			real change_time;
		} m_members_linear;

		void ClearMembers() {
			m_members_linear.current_value = 0.0f;
			m_members_linear.change_time   = 0.0f;
		}

	public:
		void Ctor() override {
			ClearMembers();
		}

		void Dtor() override {
			ClearMembers();
		}

		void Update(real delta_time) override {
			// increase the interpolation value at the required rate
			if (m_members_linear.change_time == 0.0f)
				m_members_linear.current_value = 1.0f;
			else
				m_members_linear.current_value += CAST(real, (1.0f / m_members_linear.change_time) * delta_time);

			// clamp to a maximum of 1.0f
			if (m_members_linear.current_value > 1.0f)
				m_members_linear.current_value = 1.0f;

			SetValues(m_members_linear.current_value);
		}

		void Begin(const real interp_time) {
			// set the change time and reset the interpolation to zero
			m_members_linear.change_time   = interp_time;
			m_members_linear.current_value = 0.0f;

			// if change time is 0 immediately finish the transition
			if (m_members_linear.change_time == 0.0f)
				m_members_linear.current_value = 1.0f;

			SetValues(m_members_linear.current_value);
		}

		void Reset() {
			m_members_linear.change_time   = 0.0f;
			m_members_linear.current_value = 0.0f;

			SetValues(m_members_linear.current_value);
		}

		void Set(real change_time, real current_value) {
			m_members_linear.current_value = current_value;
			m_members_linear.change_time   = change_time;

			Update(0.0f);
		}

	private:
		void SetValues(real interp) {
			// in the linear interpolator, all channels have the same value
			for (int i = 0; i < ValueCount; i++) {
				this->m_members.values[i] = m_members_linear.current_value;
			}
		}
	};
};
