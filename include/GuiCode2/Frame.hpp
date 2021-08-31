#pragma once
#include "GuiCode2/Window.hpp"

namespace GuiCode
{
	class Frame : public Window
	{
		class Button : public Component
		{
		public:
			Button();

			void Render(CommandCollection& d) const override { render(d); }

			std::function<void(void)> callback;
			std::function<void(CommandCollection&)> render;
		};

		class TitleBar : public Panel
		{
		public:
			TitleBar();

			void Update() override;
			void Render(CommandCollection& d) const override;

			Color color{ 26, 26, 26, 255 };
			Button close, maximize, minimize;
			
			bool Hitbox(const Vec2<float>& v) const override;
		};

	public:
		Frame(const WindowData& data);

		void Update() override;
		void Render(CommandCollection& d) const override;

		Color background{ 26, 26, 26, 255 };
		TitleBar titlebar;
		Panel panel;

	protected:
		bool BorderHitbox(const Vec2<float>& v) const override;
	};
}