#pragma once

#include <windows.h>
#include <macros_generic.h>
#include "../../memory/datum_index.h"
#include "extended_definitions.h"
#include "definitions.h"

namespace Yelo::Objects {
	using namespace Yelo;
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the unit extension definition for a unit. </summary>
	///
	/// <param name="unit_index">	Datum index of the unit. </param>
	///
	/// <returns>	null if it fails, else the unit extension definition. </returns>
	const Yelo::TagGroups::unit_extensions *GetUnitExtensionDefinition(const Yelo::datum_index unit_index);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the specified seat definition from a unit. </summary>
	///
	/// <param name="unit_index">	Datum index of the unit. </param>
	/// <param name="seat_index">	Index of the seat. </param>
	///
	/// <returns>	The seat definition. </returns>
	const TagGroups::unit_seat *GetSeatDefinition(const datum_index unit_index, const int16 seat_index);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the seat extension definition for a seat. </summary>
	///
	/// <param name="unit_index">	Datum index of the unit. </param>
	/// <param name="seat_index">	Datum index of the seat. </param>
	///
	/// <returns>	null if it fails, else the seat extension definition. </returns>
	const TagGroups::unit_seat_extensions *GetSeatExtensionDefinition(const datum_index unit_index, const int16 seat_index);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the unit in an objects seat. </summary>
	///
	/// <param name="unit_index">	Datum index of the unit. </param>
	/// <param name="seat_index">	Datum index of the seat. </param>
	///
	/// <returns>	The unit in seat. </returns>
	datum_index GetUnitInSeat(const datum_index unit_index, const int16 seat_index);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handler for biped's meleeing whilst seated. </summary>
	///
	/// <param name="unit_index">	Datum index of the unit. </param>
	void BipedSeatedMelee(const datum_index unit_index);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether a unit can enter a seat, handles multi team vehicles. </summary>
	///
	/// <param name="unit_index">		  	Datum index of the unit. </param>
	/// <param name="target_unit_index">  	Datum index of the target unit. </param>
	/// <param name="target_seat_index">  	Datum index of the target seat. </param>
	/// <param name="return_unit_in_seat">	[out] If non-null, the return unit in seat. </param>
	/// <param name="result">			  	[in,out] The result. </param>
	void UnitCanEnterSeatMultiteam(datum_index unit_index, datum_index target_unit_index, int16 target_seat_index,
											 datum_index *return_unit_in_seat, bool &result);

	namespace Units {
		namespace Animations {
			typedef void (*animation_state_keyframe_handler_t)(const datum_index, const unit_animation_keyframe);

			void AnimationStateDefinedKeyframe(const datum_index unit_index, const unit_animation_state state);

			void AnimationStateFinalKeyframe(const datum_index unit_index, const unit_animation_state state);

			void SetAnimationStateKeyframeHandler(const unit_animation_state state, animation_state_keyframe_handler_t handler);
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Called by BipedSeatedMelee when melee damage needs to be dealt. Implemented externally.
		/// </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		void UnitCausePlayerSeatedMelee(const datum_index unit_index);
	};
};
