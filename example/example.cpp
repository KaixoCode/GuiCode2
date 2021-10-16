#include "GuiCode2/Window.hpp"
#include "GuiCode2/Frame.hpp"

using namespace GuiCode;

int main() {
	Gui _gui;

	_gui.push_back(new Frame{ {
		.name = "Window"
	} });

	while (_gui.Loop());
}