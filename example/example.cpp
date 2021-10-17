#include "GuiCode2/Window.hpp"
#include "GuiCode2/Frame.hpp"

using namespace GuiCode;

int main() {
	Gui _gui;

	Pointer<Frame> _frame{ new Frame{ {
		.name = "Window"
	} } };

	_gui.push_back(_frame);

	while (_gui.Loop()) {
		MEASURE_FPS;
	}
}