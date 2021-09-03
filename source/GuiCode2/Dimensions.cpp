#include "GuiCode2/Dimensions.hpp"

namespace GuiCode
{
	Dimensions& Dimensions::operator=(Dimensions&& other)
	{
		dimensions = other.dimensions;
		zIndex = other.zIndex;
		return *this;
	}

	Dimensions& Dimensions::operator=(const Dimensions& other)
	{
		dimensions = other.dimensions;
		zIndex = other.zIndex;
		return *this;
	}
}