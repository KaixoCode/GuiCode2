#include "GuiCode2/Frame.hpp"

namespace GuiCode
{
	Frame::Button::Button()
		: color(this)
	{
		listener += [this](const MouseRelease& e)
		{
			if (e.button & MouseButton::Left && Hitbox(e.pos))
				callback();
		};
	}

	Frame::TitleBar::TitleBar(std::string& title)
		: title(title)
	{
		components.push_back(close);
		components.push_back(minimize);
		components.push_back(maximize);
	}

	void Frame::TitleBar::Update()
	{
		close.dimensions = { x + width - 46 * 1, y, 46, 30 };
		maximize.dimensions = { x + width - 46 * 2, y, 46, 30 };
		minimize.dimensions = { x + width - 46 * 3, y, 46, 30 };
	}

	void Frame::TitleBar::Render(CommandCollection& d) const
	{
		d.Fill(background);
		d.Quad(dimensions);

		if (font == "")
			return;

		d.Fill(text);
		d.TextAlign(textAlign);
		d.FontSize(textSize);
		d.Font(font);
		d.Text(title, { x + height, y + height / 2 });
	}

	bool Frame::TitleBar::Hitbox(const Vec2<float>& v) const
	{
		return Component::Hitbox(v) && !(v.x < 8 || v.x >= width - 8 || v.y < 8);
	}
	
	Frame::Frame(const WindowData& data)
		: Window(data), titlebar(info.name)
	{
		titlebar.close.callback = [this]() { State<Visible>(Close); };
		titlebar.minimize.callback = [this]() { State<Visible>(Minimize); };
		titlebar.maximize.callback = [this]() { State<Visible>() == Maximize ? State<Visible>(Show) : State<Visible>(Maximize); };

		titlebar.close.color.base = { 26, 26, 26, 255 };
		titlebar.close.color.State<Pressed>({ 170, 0, 0, 255 });
		titlebar.close.color.State<Hovering>({ 255, 0, 0, 255 });
		titlebar.close.render = [this](CommandCollection& d)
		{
			d.Fill(titlebar.close.color.Current());
			d.Quad(titlebar.close.dimensions);

			d.Fill({ 255, 255, 255, 255 });
			int pad = 5;
			float centerx = titlebar.close.x + titlebar.close.width / 2;
			float centery = titlebar.close.y + titlebar.close.height / 2 - 1;
			d.Line({ centerx - pad, centery - pad, centerx + pad, centery + pad }, 1.0);
			d.Line({ centerx - pad, centery + pad, centerx + pad, centery - pad }, 1.0);
		};

		titlebar.minimize.color.base = { 26, 26, 26, 255 };
		titlebar.minimize.color.State<Pressed>({ 64, 64, 64, 255 });
		titlebar.minimize.color.State<Hovering>({ 78, 78, 78, 255 });
		titlebar.minimize.render = [this](CommandCollection& d)
		{
			d.Fill(titlebar.minimize.color.Current());
			d.Quad(titlebar.minimize.dimensions);

			d.Fill({ 255, 255, 255, 255 });
			float centerx = titlebar.minimize.x + titlebar.minimize.width / 2;
			float centery = titlebar.minimize.y + titlebar.minimize.height / 2;
			d.Quad({ centerx - 5, centery, 10, 1 });
		};

		titlebar.maximize.color.base = { 26, 26, 26, 255 };
		titlebar.maximize.color.State<Pressed>({ 64, 64, 64, 255 });
		titlebar.maximize.color.State<Hovering>({ 78, 78, 78, 255 });
		titlebar.maximize.render = [this](CommandCollection& d)
		{
			d.Fill(titlebar.maximize.color.Current());
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

		components.push_back(titlebar);
		components.push_back(panel);
	}

	void Frame::ForwardUpdate()
	{
		float padding = State<Visible>() == Maximize ? 8 : 1;

		titlebar.dimensions = { padding, padding, width - 2 * padding, 30 };
		panel.settings.size = { width - 2 * padding, height - 2 * padding - 30 };
		panel.SetDimensions({ padding, 30 + padding, width - 2 * padding, height - 2 * padding - 30 });
		panel.zIndex = std::numeric_limits<int>::max();
		Window::ForwardUpdate();
	}

	void Frame::ForwardRender(CommandCollection& d)
	{
		d.Fill(border);
		d.Quad({ 0, 0, width, height });

		d.Fill(background);
		d.Quad({ 1, 1, width - 2, height - 2 });

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