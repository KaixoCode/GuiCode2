#include "GuiCode2/Frame.hpp"

namespace GuiCode
{
	Frame::Button::Button()
	{
		listener += [this](const MouseRelease& e)
		{
			if (e.button & MouseButton::Left && Hitbox(e.pos))
				callback();
		};
	}

	Frame::TitleBar::TitleBar()
	{
		components.push_back(&close);
		components.push_back(&minimize);
		components.push_back(&maximize);
	}

	void Frame::TitleBar::Update()
	{
		close.dimensions = { x + width - 46 * 1, y, 46, 32 };
		maximize.dimensions = { x + width - 46 * 2, y, 46, 32 };
		minimize.dimensions = { x + width - 46 * 3, y, 46, 32 };
	}

	void Frame::TitleBar::Render(CommandCollection& d) const
	{
		d.Fill(color);
		d.Quad(dimensions);
	}

	bool Frame::TitleBar::Hitbox(const Vec2<float>& v) const
	{
		return Component::Hitbox(v) && !(v.x < 8 || v.x >= width - 8 || v.y < 8);
	}
	
	Frame::Frame(const WindowData& data)
		: Window(data)
	{
		titlebar.close.callback = [this]() { State<Visible>(Close); };
		titlebar.minimize.callback = [this]() { State<Visible>(Minimize); };
		titlebar.maximize.callback = [this]() { State<Visible>() == Maximize ? State<Visible>(Show) : State<Visible>(Maximize); };

		titlebar.close.hover = { 255, 0, 0, 111 };
		titlebar.close.press = { 170, 0, 0, 111 };
		titlebar.close.render = [this](CommandCollection& d)
		{
			if (titlebar.close.State<Pressed>()) d.Fill(titlebar.close.press);
			else if (titlebar.close.State<Hovering>()) d.Fill(titlebar.close.hover);
			else d.Fill({ 0, 0, 0, 0 });

			d.Quad(titlebar.close.dimensions);

			d.Fill({ 255, 255, 255, 255 });
			int pad = 5;
			float centerx = titlebar.close.x + titlebar.close.width / 2;
			float centery = titlebar.close.y + titlebar.close.height / 2 - 1;
			d.Line({ centerx - pad, centery - pad, centerx + pad, centery + pad }, 1.7);
			d.Line({ centerx - pad, centery + pad, centerx + pad, centery - pad }, 1.7);
		};

		titlebar.minimize.hover = { 128, 128, 128, 92 };
		titlebar.minimize.press = { 128, 128, 128, 92 };
		titlebar.minimize.render = [this](CommandCollection& d)
		{
			if (titlebar.minimize.State<Pressed>()) d.Fill(titlebar.minimize.press);
			else if (titlebar.minimize.State<Hovering>()) d.Fill(titlebar.minimize.hover);
			else d.Fill({ 0, 0, 0, 0 });

			d.Quad(titlebar.minimize.dimensions);

			d.Fill({ 255, 255, 255, 255 });
			float centerx = titlebar.minimize.x + titlebar.minimize.width / 2;
			float centery = titlebar.minimize.y + titlebar.minimize.height / 2;
			d.Quad({ centerx - 5, centery, 10, 1 });
		};

		titlebar.maximize.hover = { 128, 128, 128, 92 };
		titlebar.maximize.press = { 128, 128, 128, 92 };
		titlebar.maximize.render = [this](CommandCollection& d)
		{
			if (titlebar.maximize.State<Pressed>()) d.Fill(titlebar.maximize.press);
			else if (titlebar.maximize.State<Hovering>()) d.Fill(titlebar.maximize.hover);
			else d.Fill({ 0, 0, 0, 0 });

			d.Quad(titlebar.maximize.dimensions);

			d.Fill({ 255, 255, 255, 255 });
			float centerx = titlebar.maximize.x + titlebar.maximize.width / 2;
			float centery = titlebar.maximize.y + titlebar.maximize.height / 2;
			if (State<Visible>() == Maximize)
			{
				d.Quad({ centerx - 5, centery + 3, 8, 1 });
				d.Quad({ centerx + 2, centery - 4, 1, 8 });
				d.Quad({ centerx - 5, centery - 4, 8, 1 });
				d.Quad({ centerx - 5, centery - 4, 1, 8 });

				d.Quad({ centerx - 3, centery - 6, 1, 2 });
				d.Quad({ centerx - 3, centery - 6, 8, 1 });
				d.Quad({ centerx + 3, centery + 1, 2, 1 });
				d.Quad({ centerx + 4, centery - 6, 1, 8 });
			}
			else
			{
				d.Quad({ centerx - 5, centery + 3, 10, 1 });
				d.Quad({ centerx + 4, centery - 6, 1, 10 });
				d.Quad({ centerx - 5, centery - 6, 10, 1 });
				d.Quad({ centerx - 5, centery - 6, 1, 10 });
			}
		};

		titlebar.maximize.listener += [](const MouseExit& e)
		{
			std::cout << "eifsone f aef ae a" << std::endl;
		};

		components.push_back(&titlebar);
		components.push_back(&panel);
	}

	void Frame::ForwardUpdate()
	{
		float padding = State<Visible>() == Maximize ? 8 : 0;

		titlebar.dimensions = { padding, padding, width - 2 * padding, 32 };
		panel.settings.size = { width - 2 * padding, height - 2 * padding - 32 };
		panel.SetDimensions({ padding, 32 + padding, width - 2 * padding, height - 2 * padding - 32 });
		panel.zIndex = std::numeric_limits<int>::max();
		Window::ForwardUpdate();
	}

	void Frame::ForwardRender(CommandCollection& d)
	{
		d.Fill(background);
		d.Quad({ 0, 0, width, height });
		Window::ForwardRender(d);
	}

	bool Frame::BorderHitbox(const Vec2<float>& v) const 
	{
		// Window border is hit when hovering over titlebar, but not over the buttons
		// or when hovering over the outer 8 pixels, which is the resize part.
		return (titlebar.State<Hovering>() && !titlebar.close.Hitbox(v) &&
			!titlebar.minimize.Hitbox(v) && !titlebar.maximize.Hitbox(v))
			|| v.x < 8 || v.y >= height - 8 || v.x >= width - 8 || v.y < 8;
	}
}