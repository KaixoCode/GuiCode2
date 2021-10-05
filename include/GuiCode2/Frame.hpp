#pragma once
#include "GuiCode2/Menu.hpp"
#include "GuiCode2/Window.hpp"

namespace GuiCode
{
	/**
	 * Parsers
	 */
	struct TitleBarButtonParser : public TagParser
	{
		TitleBarButtonParser();
		Pointer<Component> Create() override;
	};

	struct TitleBarParser : public TagParser
	{
		TitleBarParser();
		Pointer<Component> Create() override;
		void Append(Component& c, Pointer<Component>&& obj) override;
	};

	struct FrameParser : public ComponentParser
	{
		FrameParser();
		Pointer<Component> Create() override;
		void Append(Component& c, Pointer<Component>&& obj) override;
	};

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
			Button& operator=(Button&& other);

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
			TitleBar& operator=(TitleBar&& other);

			Color background{ 26, 26, 26, 255 };
			Color text{ 255, 255, 255, 255 };
			Button close, maximize, minimize;
			float textSize = 12;
			int textAlign = Align::Left | Align::CenterY;
			std::string font = GraphicsBase::DefaultFont;
			Menu menu{ false /* Not vertical menu */ };

			void Update() override;
			void Render(CommandCollection& d) const override;
			bool Hitbox(const Vec2<float>& v) const override;

		private:
			std::string* title = nullptr;
		};

		Frame();
		Frame(const WindowData& data);
		Frame(const Frame&) = delete;
		Frame(Frame&&) = delete;
		Frame& operator=(Frame&& other)
		{
			background = std::move(other.background);
			border = std::move(other.border);
			titlebar = std::move(other.titlebar);
			panel = std::move(other.panel);
			return *this;
		}

		Color background{ 13, 13, 13, 255 };
		Color border{ 64, 64, 64 };
		TitleBar titlebar;
		Panel panel;

		void ForwardUpdate() override;
		void ForwardRender(CommandCollection& d) override;

	protected:
		bool BorderHitbox(const Vec2<float>& v) const override;

	private:
		void Init();
	};
}