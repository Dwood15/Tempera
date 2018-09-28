/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/math/periodic_functions.hpp>

#include <YeloLib/Halo1/time/interpolation/c_interp_base.hpp>

namespace Yelo
{
	namespace Time { namespace Interpolation
	{
		template<int ValueCount>
		class c_interp_function
			: public c_interp_base<ValueCount>
		{
		protected:
			struct
			{
				real update_time;
				real animation_time;
			}m_members_function;

		public:
			c_interp_function() { }

			void Ctor() override
			{
				c_interp_base::Ctor();

				m_members_function.update_time = 0.0f;
				m_members_function.animation_time = 0.0f;
			}

			void Dtor() override
			{
				c_interp_base::Dtor();

				m_members_function.update_time = 0.0f;
				m_members_function.animation_time = 0.0f;
			}

			double GetPeriodicFunctionValue(Enums::periodic_function function, double input)
			{
				return blam::periodic_function_evaluate(function, input);
			}

			double GetTransitionFunctionValue(Enums::transition_function function, double input)
			{
				return blam::transition_function_evaluate(function, input);
			}

			void Update(real delta_time) override = 0;
		};
	};};
};