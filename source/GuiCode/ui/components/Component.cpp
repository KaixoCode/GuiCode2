#include "GuiCode/ui/components/Component.hpp"

bool Component::WithinBounds(const Vec2<int>& pos) const
{
	return pos.x >= Position().x && pos.x <= Position().x + Size().width
		&& pos.y >= Position().y && pos.y <= Position().y + Size().height;
};

void Component::Update(const Vec4<int>& viewport) 
{
	m_Viewport = viewport;
}