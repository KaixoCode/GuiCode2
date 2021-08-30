#pragma once
#include "GuiCode/ui/events/Event.hpp"
#include "GuiCode/ui/Components/Component.hpp"
#include "GuiCode/ui/components/panels/Panel.hpp"

// --------------------------------------------------------------------------
// ---------------------------- View Base -----------------------------------
// --------------------------------------------------------------------------

class ViewBase : public Panel
{
public:
	/*void Draw() override 
	{
		Graphics::SetColor(Theme::View::background);
		Graphics::Quad({ m_X, m_Y, m_Width, m_Height });
	}*/
};

// --------------------------------------------------------------------------
// ------------------------------ View --------------------------------------
// --------------------------------------------------------------------------

template<typename T>
class View : public T
{
public:
	static T& Instance()
	{
		static T _t;
		return _t;
	}
};

class Test1 : public ViewBase
{};

class Test2 : public ViewBase
{};

class Test3 : public ViewBase
{};

class Test4 : public ViewBase
{};

class Test5 : public ViewBase
{};
