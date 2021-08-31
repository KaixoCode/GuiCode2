#include "GuiCode2/pch.hpp"

#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/Layout.hpp"
#include "GuiCode2/Frame.hpp"

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
			press = e.pos;
			std::cout << "Press!" << id << std::endl;
		};
		
		listener += [this](const MouseClick& e)
		{
			std::cout << "Click!" << id << std::endl;
		};

		listener += [this](const MouseRelease& e)
		{
			zIndex = 0;
			std::cout << "Release!" << id << std::endl;
		};

		listener += [this](const MouseMove& e)
		{
			std::cout << "Move!" << id << std::endl;
		};

		listener += [this](const MouseDrag& e)
		{
			if (press.x > x + width - 10 && press.y > y + height - 10)
			{
				size = e.pos - position;
				press = e.pos;
			} else {
				position += e.pos - press;
				press = e.pos;
				zIndex = 10;
			}
			std::cout << "Drag!" << id << std::endl;
		};
	}

	void Render(CommandCollection& d) const override
	{
		d.Fill({ 255, 255, 255, 255 });
		d.Quad(dimensions);

		if (State<Pressed>())
			d.Fill({ 200, 0, 0, 255 });
		else if (State<Hovering>())
			d.Fill({ 235, 0, 0, 255 });
		else
			d.Fill({ 255, 0, 0, 255 });

		d.Quad(dimensions - 1);

	}

	void Update() override
	{

	}

	Vec2<float> press;
	int id;
};

int main()
{

	Frame window{ {
		.name = "Hello", 
		.dimensions{ 500, 100, 500, 500 }, 
		.state = Show
	} };

	window.panel.State<UseDepth>(true);
	Apple& _comp = window.panel.Emplace<Apple>(1);
	Apple& _comp2 = window.panel.Emplace<Apple>(2);
	Apple& _comp3 = window.panel.Emplace<Apple>(3);
	_comp.dimensions = { 8, 8, 50, 50 };
	_comp2.dimensions = { 40, 40, 50, 50 };
	_comp3.dimensions = { 10, 35, 50, 50 };

	_comp3.State<Visible>(false);

	while (window.Loop())
	{
		LIMIT_FPS(60);
	}
}