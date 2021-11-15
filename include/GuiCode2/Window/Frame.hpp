#pragma once
#include "GuiCode2/Components/Menu.hpp"
#include "GuiCode2/Window/Window.hpp"

namespace GuiCode
{
	class Frame : public Window
	{
	public:
		class Button : public Component
		{
		public:
			Button();
			Button(const Button&) = delete;
			Button(Button&&) = delete;
			Button& operator=(Button&& other);
			Button& operator=(const Button&) = delete;

			std::string type;
			StateColors color;
			std::function<void(void)> callback;
			std::function<void(CommandCollection&)> render;

			void Render(CommandCollection& d) const override { render(d); }
		};

		class TitleBar : public Component
		{
		public:
			friend class Frame;
			TitleBar();
			TitleBar(const TitleBar&) = delete;
			TitleBar(TitleBar&&) = delete;
			TitleBar& operator=(TitleBar&& other);
			TitleBar& operator=(const TitleBar&) = delete;

			Color background{ 26, 26, 26, 255 };
			Button close, maximize, minimize;
			struct Text
			{
				float size = 12;
				Color color{ 255, 255, 255, 255 };
			} text;
			std::string font = GraphicsBase::DefaultFont;
			Menu menu{ {.vertical = false, .background{ {.base{ 0, 0, 0, 0 } } }, .border{ .width = 0 } } };

			void Update() override;
			void Render(CommandCollection& d) const override;
			bool Hitbox(const Vec2<float>& v) const override;

		private:
			std::string* title = nullptr;
		};

		Frame(const Settings& data = {});
		Frame(const Frame&) = delete;
		Frame(Frame&&) = delete;
		Frame& operator=(Frame&& other);
		Frame& operator=(const Frame&) = delete;

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