#pragma once
#include "Window.hpp"


namespace GuiCode
{
	class Frame : public Window
	{
		class Button : public Component
		{
		public:
			Button()
			{
				listener += [this](const MouseRelease& e)
				{
					if (e.button & MouseButton::Left && Hitbox(e.pos))
						callback();
				};
			}

			void Render(CommandCollection& d) const override
			{
				render(d);
			}

			std::function<void(void)> callback;
			std::function<void(CommandCollection&)> render;
		};

		class TitleBar : public Panel
		{
		public:
			TitleBar()
			{
				RegisterComponent(close);
				RegisterComponent(minimize);
				RegisterComponent(maximize);
			}

			void Update() override
			{
				close.dimensions =    { x + width - 46 * 1, y, 46, 32 };
				maximize.dimensions = { x + width - 46 * 2, y, 46, 32 };
				minimize.dimensions = { x + width - 46 * 3, y, 46, 32 };
			}

			void Render(CommandCollection& d) const override
			{
				d.Fill(color);
				d.Quad(dimensions);
			}

			Color color{ 26, 26, 26, 255 };
			Button close, maximize, minimize;
			
			bool Hitbox(const Vec2<double>& v) const override
			{
				return Panel::Hitbox(v) && !(v.x < 8 || v.x >= width - 8 || v.y < 8);
			}
		};

	public:
		Frame(const WindowData& data)
			: Window(data)
		{
			titlebar.close.callback = [this]() { State<Visible>(Close); };
			titlebar.minimize.callback = [this]() { State<Visible>(Minimize); };
			titlebar.maximize.callback = [this]() { State<Visible>() == Maximize ? State<Visible>(Show) : State<Visible>(Maximize); };

			titlebar.close.render = [this](CommandCollection& d)
			{
				if (titlebar.close.State<Pressed>()) d.Fill({ 170, 0, 0, 111 });
				else if (titlebar.close.State<Hovering>()) d.Fill({ 255, 0, 0, 111 });
				else d.Fill({ 0, 0, 0, 0 });

				d.Quad(titlebar.close.dimensions);

				d.Fill({ 255, 255, 255, 255 });
				int pad = 5;
				double centerx = titlebar.close.x + titlebar.close.width / 2;
				double centery = titlebar.close.y + titlebar.close.height / 2;
				d.Line({ centerx - pad, centery - pad, centerx + pad, centery + pad }, 2);
				d.Line({ centerx - pad, centery + pad, centerx + pad, centery - pad }, 2);
			};

			titlebar.minimize.render = [this](CommandCollection& d)
			{
				if (titlebar.minimize.State<Pressed>()) d.Fill({ 128, 128, 128, 92 });
				else if (titlebar.minimize.State<Hovering>()) d.Fill({ 128, 128, 128, 58 });
				else d.Fill({ 0, 0, 0, 0 });

				d.Quad(titlebar.minimize.dimensions);

				d.Fill({ 255, 255, 255, 255 });
				double centerx = titlebar.minimize.x + titlebar.minimize.width / 2;
				double centery = titlebar.minimize.y + titlebar.minimize.height / 2;
				d.Quad({ centerx - 5, centery, 10, 1});
			};

			titlebar.maximize.render = [this](CommandCollection& d)
			{
				if (titlebar.maximize.State<Pressed>()) d.Fill({ 128, 128, 128, 92 });
				else if (titlebar.maximize.State<Hovering>()) d.Fill({ 128, 128, 128, 58 });
				else d.Fill({ 0, 0, 0, 0 });

				d.Quad(titlebar.maximize.dimensions);

				d.Fill({ 255, 255, 255, 255 });
				double centerx = titlebar.maximize.x + titlebar.maximize.width / 2;
				double centery = titlebar.maximize.y + titlebar.maximize.height / 2;
				if (State<Visible>() == Maximize)
				{
					d.Quad({ centerx - 5, centery + 2, 8, 1 });
					d.Quad({ centerx + 2, centery - 5, 1, 8 });
					d.Quad({ centerx - 5, centery - 5, 8, 1 });
					d.Quad({ centerx - 5, centery - 5, 1, 8 });

					d.Quad({ centerx - 3, centery + 3, 1, 2 });
					d.Quad({ centerx - 3, centery + 4, 8, 1 });
					d.Quad({ centerx + 3, centery - 3, 2, 1 });
					d.Quad({ centerx + 4, centery - 3, 1, 8 });
				}
				else
				{
					d.Quad({ centerx - 5, centery + 4, 10, 1 });
					d.Quad({ centerx + 4, centery - 5, 1, 10 });
					d.Quad({ centerx - 5, centery - 5, 10, 1 });
					d.Quad({ centerx - 5, centery - 5, 1, 10 });
				}
			};

			RegisterComponent(titlebar);
			RegisterComponent(panel);
		}

		void Update() override
		{
			double padding = State<Visible>() == Maximize ? 8 : 0;

			titlebar.dimensions = { padding, padding, width - 2 * padding, 32 };
			panel.dimensions = { padding, 32 + padding, width - 2 * padding, height - 2 * padding };
		}

		void Render(CommandCollection& d) const override
		{
			d.Fill(background);
			d.Quad({ 0, 0, width, height });
		}

		Color background{ 26, 26, 26, 255 };
		TitleBar titlebar;
		Panel panel;

	protected:
		bool BorderHitbox(const Vec2<double>& v) const override 
		{ 
			// Window border is hit when hovering over titlebar, but not over the buttons
			// or when hovering over the outer 8 pixels, which is the resize part.
			return (titlebar.State<Hovering>() && !titlebar.close.Hitbox(v) &&
				!titlebar.minimize.Hitbox(v) && !titlebar.maximize.Hitbox(v)) 
				|| v.x < 8 || v.y >= height - 8 || v.x >= width - 8 || v.y < 8;
		}
	};
}