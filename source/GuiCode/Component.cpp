#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"

namespace GuiCode
{


	Component::Component()
	{

		listener += [this](const GuiCode::Render& r) 
		{
			Render(r.collection);
		};
		
	}


}