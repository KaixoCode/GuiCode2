#pragma once
#include "GuiCode2/Menu.hpp"
#include "GuiCode2/Window.hpp"

namespace GuiCode
{
	class Frame : public Window
	{
	public:
		/**
		 * Simple button class for the close/minimize/maximize 
		 * buttons on the frame.
		 */
		class Button : public Component
		{
		public:
			Button();
			Button(const Button&) = delete;
			Button(Button&&) = delete;
			Button& operator=(const Button& other);

			std::string type;
			StateColors color;
			std::function<void(void)> callback;
			std::function<void(CommandCollection&)> render;

			void Render(CommandCollection& d) const override { render(d); }
		};

		/**
		 * Title bar, contains the close/maximize/minimize 
		 * buttons.
		 */
		class TitleBar : public Component
		{
		public:
			friend class Frame;
			TitleBar();
			TitleBar& operator=(const TitleBar& other);

			Color background{ 26, 26, 26, 255 };
			Color text{ 255, 255, 255, 255 };
			Button close, maximize, minimize;
			float textSize = 12;
			int textAlign = Align::Left | Align::CenterY;
			std::string_view font;
			Menu menu{ false /* Not vertical menu */ };

			void Update() override;
			void Render(CommandCollection& d) const override;
			bool Hitbox(const Vec2<float>& v) const override;

		private:
			std::string* title = nullptr;
		};

		Frame(const WindowData& data);

		Color background{ 13, 13, 13, 255 };
		Color border{ 64, 64, 64 };
		TitleBar titlebar;
		Panel panel;

		void ForwardUpdate() override;
		void ForwardRender(CommandCollection& d) override;

	protected:
		bool BorderHitbox(const Vec2<float>& v) const override;
	};

	class TitleBarButtonParser : public TagParser
	{
	public:
		TitleBarButtonParser()
		{
			settings.name = "titlebar-button";
			Attribute("color", &Frame::Button::color);
			Attribute("type", &Frame::Button::type);
		}

		Pointer<Component> Create() override
		{
			return new Frame::Button{ };
		};

		void Append(Component& c, Pointer<Component>& obj) override
		{}
	};

	class TitleBarParser : public TagParser
	{
	public:
		TitleBarParser()
		{
			Parser::Link<TitleBarButtonParser>();
			settings.name = "titlebar";
			Attribute("background", &Frame::TitleBar::background);
			Attribute("text", &Frame::TitleBar::text);
			Attribute("text-size", &Frame::TitleBar::textSize);
			Attribute("text-align", &Frame::TitleBar::textAlign);
			Attribute("font", &Frame::TitleBar::font);
		}

		Pointer<Component> Create() override
		{
			return new Frame::TitleBar{ };
		};

		void Append(Component& c, Pointer<Component>& obj) override
		{
			Frame::TitleBar* _t = dynamic_cast<Frame::TitleBar*>(&c);
			if (!_t)
				return;

			Frame::Button* _other1 = obj;
			if (_other1)
			{
				if (_other1->type == "close")
					_t->close = *_other1;
				if (_other1->type == "minimize")
					_t->minimize = *_other1;
				if (_other1->type == "maximize")
					_t->maximize = *_other1;
				return;
			}

			Menu* _other2 = obj;
			if (_other2)
			{
				_t->menu = *_other2;
				return;
			}
		}
	};

	class FrameParser : public TagParser
	{
	public:
		FrameParser()
		{
			Parser::Link<TitleBarParser>();
			Parser::Link<PanelParser>();
			settings.name = "frame";
			Attribute("background", &Frame::background);
			Attribute("border", &Frame::border);
			Attribute("info", &Frame::info);
			Attribute("info.name", &WindowInfo::name);
			Attribute("info.hideonclose", &WindowInfo::hideOnClose);
			Attribute("z-index", &Panel::zIndex);
			Attribute("size", &Panel::size);
			Attribute("size.width", &Vec2<float>::width);
			Attribute("size.height", &Vec2<float>::height);
			Attribute("min", &Panel::min);
			Attribute("min.width", &Vec2<float>::width);
			Attribute("min.height", &Vec2<float>::height);
			Attribute("max", &Panel::max);
			Attribute("max.width", &Vec2<float>::width);
			Attribute("max.height", &Vec2<float>::height);
		}

		Pointer<Component> Create() override
		{
			return new Frame{ {} };
		};

		void Append(Component& c, Pointer<Component>& obj) override
		{
			Frame* _t = dynamic_cast<Frame*>(&c);
			if (!_t)
				return;

			Frame::TitleBar* _other1 = obj;
			if (_other1)
			{
				_t->titlebar = *_other1;
				return;
			}

			Panel* _other2 = obj;
			if (_other2)
			{
				_t->panel = *_other2;
				return;
			}
		}

	};
}