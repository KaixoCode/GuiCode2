#include "GuiCode2/Window/Window.hpp"
#include "GuiCode2/Window/Frame.hpp"
#include "GuiCode2/Components/Menu.hpp"

using namespace GuiCode;

int main() {
	Gui _gui;

	Pointer<Frame> _frame{ new Frame{ {
		.name = "Window"
	} } };

	MenuBarButton& _file = _frame->titlebar.menu.push_back(new MenuBarButton{ {
		.name = "File",
	} });

	_file.menu.push_back(new MenuButton{ {
		.combo = 'A' | Mods::Control,
		.name = "test",
	} });

	_file.menu.push_back(new MenuButton{ {
		.combo = 'C' | Mods::Shift,
		.name = "test2",
	} });

	_file.menu.push_back(new MenuButton{ {
		.combo = 'Q' | Mods::Alt | Mods::Shift,
		.name = "test3",
	} });

	_file.menu.push_back(new MenuButton{ {
		.combo = Key::F11 | Mods::Control | Mods::Shift,
		.name = "test4",
	} });

	_gui.push_back(_frame);

	while (_gui.Loop()) {
		MEASURE_FPS;
	}
}