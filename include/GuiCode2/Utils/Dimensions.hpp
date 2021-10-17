#pragma once
#include "GuiCode2/pch.hpp"

namespace GuiCode
{
	/**
	 * Dimensions object contains a union for easy access to all
	 * aspects of dimensions, like position/size etc. Also contains
	 * a z-index
	 */
	struct Dimensions
	{
		Dimensions() : dimensions{ 0, 0, 0, 0 } {}
		Dimensions(float a, float b, float c, float d) : dimensions{ a, b, c, d } {}
		Dimensions(Dimensions&& other) noexcept { *this = other; }
		Dimensions(const Dimensions& other) { *this = other; }

		Vec2<float> min{  5,  5 }; // Minimum size, -1 = no minimum
		Vec2<float> max{ -1, -1 }; // Maximum size, -1 = no maximum

		union
		{
			struct { float x, y, width, height; };
			struct { Vec2<float> position, size; };
			Vec4<float> dimensions;
		};
		float zIndex = 0;

		Dimensions& operator=(Dimensions&& other);
		Dimensions& operator=(const Dimensions& other);
	};
}