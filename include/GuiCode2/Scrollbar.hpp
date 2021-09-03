#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/StateColors.hpp"

namespace GuiCode
{
	/**
	 * Very simple scrollbar class for Panel's overflow: Scroll.
	 */
	class Scrollbar : public Component
	{
	public:
		enum Axis { Vertical, Horizontal };

		Scrollbar();

		/**
		 * @return true when scrollbar is necessary (content overflows)
		 */
		bool Necessary() const;

		float visibleRange; // Visible range on the screen, used to calculate bar width
		float value = 0;    // value
		Vec2<float> range;
		Axis axis = Horizontal;

		Color background{ 96, 96, 96 };
		StateColors bar{ *this };

		void Update() override;
		void Render(CommandCollection& d) const override;
		bool Hitbox(const Vec2<float>& pos) const override;

	private:
		float m_ValueWhenPressed = 0; // value when mousepressed
		bool m_IsDragging = false;    // true when mouse was pressed on bar, until mouse release
	};
}