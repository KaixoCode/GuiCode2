#pragma once
#include "GuiCode2/Window.hpp"

namespace GuiCode
{
	class Frame : public Window
	{
		/**
		 * Simple button class for the close/minimize/maximize 
		 * buttons on the frame.
		 */
		class Button : public Component
		{
		public:
			Button();

			Color hover;
			Color press;
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
			TitleBar();

			Color color{ 26, 26, 26, 255 };
			Button close, maximize, minimize;
			
			void Update() override;
			void Render(CommandCollection& d) const override;
			bool Hitbox(const Vec2<float>& v) const override;
		};

	public:
		Frame(const WindowData& data);

		Color background{ 26, 26, 26, 255 };
		TitleBar titlebar;
		Panel panel;

		void ForwardUpdate() override;
		void ForwardRender(CommandCollection& d) override;

	protected:
		bool BorderHitbox(const Vec2<float>& v) const override;
	};
}