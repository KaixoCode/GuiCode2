#include "GuiCode2/pch.hpp"

#include "GuiCode2/Component.hpp"
#include "GuiCode2/Event.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/Layout.hpp"

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

	int id;
};

int main()
{
	Panel _myPanel;
	_myPanel.div = { { .id = 1 } };
	Apple& _comp = _myPanel.Emplace<Apple>(1);
	Apple& _comp2 = _myPanel.Emplace<Apple>(2);
	_comp.dimensions = { 8, 8, 4, 4 };
	_comp2.dimensions = { 1, 1, 6, 6 };


	_myPanel.listener(MouseMove{ { 10, 10 } });
	_myPanel.listener(MousePress{ { 10, 10 } });
	_myPanel.listener(MouseDrag{ { 2, 5 } });
	_myPanel.listener(MouseRelease{ { 2, 2 } });
	_myPanel.listener(MouseMove{ { 2, 2 } });
	_myPanel.listener(MousePress{ { 2, 5 } });


}