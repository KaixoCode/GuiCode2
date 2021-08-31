#include "GuiCode2/pch.hpp"

#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/Layout.hpp"
#include <GuiCode2/Window.hpp>

using namespace GuiCode;

/*
 * aspects:
 *  - Events
 *  - Layouts
 *  - Scrolling
 *  - Graphics
 *  - Windows
 * 
 *  - Need to know which object the mouse is over.
 *  - Need to know the viewport of each object.
 * 
 * Your object -> Register for events in other object.
 * 
 * ideas:
 *  - ability to add own events
 * 
 * 
 * 
 * Layout
 *  
 * 
 * 
 * 
 */

class Apple : public Component
{
public:
	Apple(int ids)
		: id(ids)
	{
		listener += [this](const Focus& e)
		{
			std::cout << "Focus!" << id << std::endl;
		};

		listener += [this](const Unfocus& e)
		{
			std::cout << "Unfocus!" << id << std::endl;
		};

		listener += [this](const MouseEnter& e)
		{
			std::cout << "Enter!" << id << std::endl;
		};

		listener += [this](const MouseExit& e)
		{
			std::cout << "Exit!" << id << std::endl;
		};

		listener += [this](const MousePress& e)
		{
			std::cout << "Press!" << id << std::endl;
		};
		
		listener += [this](const MouseClick& e)
		{
			std::cout << "Click!" << id << std::endl;
		};

		listener += [this](const MouseRelease& e)
		{
			std::cout << "Release!" << id << std::endl;
		};

		listener += [this](const MouseMove& e)
		{
			std::cout << "Move!" << id << std::endl;
		};

		listener += [this](const MouseDrag& e)
		{
			std::cout << "Drag!" << id << std::endl;
		};
	}

	void Render(CommandCollection& d) const override
	{
		if (State<Pressed>())
			d.Fill({ 200, 200, 200, 255 });
		else if (State<Hovering>())
			d.Fill({ 235, 235, 235, 255 });
		else
			d.Fill({ 255, 255, 255, 255 });

		d.Quad(dimensions);
	}

	int id;
};

int main()
{

	WindowsWindow window{ { .name = "Hello" }};


	window.panel.State<UseDepth>(true);
	Apple& _comp = window.panel.Emplace<Apple>(1);
	Apple& _comp2 = window.panel.Emplace<Apple>(2);
	Apple& _comp3 = window.panel.Emplace<Apple>(3);
	_comp.dimensions = { 8, 8, 50, 50 };
	_comp.zIndex = 10;
	_comp2.dimensions = { 40, 40, 50, 50 };
	_comp2.zIndex = 5;	
	_comp3.dimensions = { 10, 35, 50, 50 };
	_comp3.zIndex = 0;

	while (true)
	{
		window.Loop();
	}
}