#include "GuiCode2/Dimensions.hpp"

namespace GuiCode
{
	Dimensions& Dimensions::operator=(Dimensions&& other)
	{
		dimensions = other.dimensions;
		zIndex = other.zIndex;
		min = other.min;
		max = other.max;
		return *this;
	}

	Dimensions& Dimensions::operator=(const Dimensions& other)
	{
		dimensions = other.dimensions;
		zIndex = other.zIndex;
		min = other.min;
		max = other.max;
		return *this;
	}
}