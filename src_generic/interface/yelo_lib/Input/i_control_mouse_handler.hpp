#pragma once
namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Input 
	{
		/// <summary>	The mouse handler interface. </summary>
		class i_control_mouse_handler
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the mouse position update action. </summary>
			///
			/// <param name="absolute">	The absolute position. </param>
			/// <param name="relative">	The relative position. </param>
			virtual void OnMousePositionUpdate(const point2d& absolute, const point2d& relative) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the mouse button update action. </summary>
			///
			/// <param name="button">	The button. </param>
			/// <param name="value"> 	The button state. </param>
			virtual void OnMouseButtonUpdate(const int button, const bool value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the mouse wheel update action. </summary>
			///
			/// <param name="value">	The wheel value. </param>
			virtual void OnMouseWheelUpdate(const int value) = 0;
		};
	};};};
};
#endif