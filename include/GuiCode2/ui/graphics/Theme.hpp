#pragma once
#include "GuiCode/pch.hpp"

struct Theme
{
	struct Window
	{ 
		static inline Color             background{  23,  23,  23, 255 };
	};						          
							          
							          
	struct Gui				          
	{
		static inline Color             background{   0,   0,   0,   0 };

		static inline Color            button_text{   0,   0,   0,   0 };
		static inline Color                 button{   0,   0,   0,   0 };
		static inline Color           button_hover{   0,   0,   0,   0 };
		static inline Color           button_press{   0,   0,   0,   0 };
	};

	struct Menu
	{
		static inline Color             background{  18,  18,  18, 255 };
		static inline Color                 border{  64,  64,  64, 255 };

		static inline Color			   button_text{ 255, 255, 255, 255 };
		static inline Color			 keycombo_text{ 128, 128, 128, 255 };
		static inline Color			        button{ 255, 255, 255,   0 };
		static inline Color			  button_hover{ 255, 255, 255,  13 };
		static inline Color			  button_press{ 255, 255, 255,  26 };

		static inline Color   disabled_button_text{ 179, 179, 179, 255 };
		static inline Color disabled_keycombo_text{  77,  77,  77, 255 };
		static inline Color        disabled_button{ 255, 255, 255,   0 };
		static inline Color  disabled_button_hover{ 255, 255, 255,   0 };
		static inline Color  disabled_button_press{ 255, 255, 255,   0 };
	};

	struct TitleBar
	{
		static inline Color			    background{  25,  25,  25, 255 };
									 
		static inline Color			          text{ 179, 179, 179, 255 };
									 								  
		static inline Color			 button_border{  64,  64,  64, 255 };
		static inline Color			   button_text{ 255, 255, 255, 255 };
		static inline Color			        button{ 255, 255, 255,   0 };
		static inline Color			  button_hover{ 255, 255, 255,  12 };
		static inline Color			  button_press{  18,  18,  23, 255 };
	};

	struct View
	{
		static inline Color			    background{  38,  38,  38, 255};
	};

	struct Arrangement
	{
		static inline Color		   background_dark{ 0.18, 0.18, 0.18, 1.00 };
		static inline Color		  background_light{ 0.22, 0.22, 0.22, 1.00 };
		static inline Color					  line{ 0.00, 0.00, 0.00, 0.60 };

		static inline Color       track_background{ 0.18, 0.18, 0.18, 1.00 };
	};
};