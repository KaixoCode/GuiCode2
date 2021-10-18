#include "GuiCode2/Window/Gui.hpp"
#include "GuiCode2/Window/Frame.hpp"
#include "GuiCode2/Components/Menu.hpp"

using namespace GuiCode;

int main() {
	Gui _gui;

	Window::Settings _settings{
		.name = "Window"
	};

	Frame& _frame = _gui.emplace<Frame>(_settings);
	
	_frame.Create();

	MenuBarButton& _file = _frame.titlebar.menu.push_back(new MenuBarButton{ {
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

	while (_gui.Loop()) {
		MEASURE_FPS;
	}
}