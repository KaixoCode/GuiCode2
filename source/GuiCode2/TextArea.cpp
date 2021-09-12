#include "GuiCode2/TextArea.hpp"

namespace GuiCode
{
	TextArea::TextArea()
		: Panel{ {.ratio = 1, .overflow = Overflow::Scroll } },
		align(displayer.align),
		container(displayer.container),
		font(displayer.font),
		fontSize(displayer.fontSize),
		lineHeight(displayer.lineHeight),
		placeholder(displayer.placeholder),
		textColor(displayer.textColor),
		selectColor(displayer.selectColor),
		wrap(displayer.wrap)
	{
		panels.Emplace({ {.ratio = 0, .size{ Auto, Auto } }, displayer });
		
		listener += [this](const MousePress& e)
		{
			if (e.button != MouseButton::Left || scrollbar.x.State<Hovering>() || scrollbar.y.State<Hovering>())
				return;

			m_Mouse = e.pos - position;
			UpdateScroll();
			m_Dragging = true;
		};

		listener += [this](const MouseRelease& e)
		{			
			if (e.button != MouseButton::Left)
				return;

			m_Dragging = false;
		};

		listener += [this](const MouseDrag& e)
		{
			m_Mouse = e.pos - position;
		};

		listener += [this](const KeyPress& e)
		{
			if (e.Handled())
				m_Update = 2;
		};

		listener += [this](const KeyType& e)
		{
			if (e.Handled())
				m_Update = 2;
		};
	}

	void TextArea::Update() 
	{
		displayer.min = size - Vec2<float>{ scrollbar.y.width* scrollbar.y.Necessary(), scrollbar.x.height* scrollbar.x.Necessary() };
		if (displayer.wrap != Wrap::None)
			displayer.width = width - scrollbar.y.width * scrollbar.y.Necessary();

		if (m_Update)
		{
			m_Update--;
			UpdateScroll();
		}

		if (m_Dragging & MouseButton::Left)
		{
			if (m_Mouse.y < height * 0.1)
				scrollbar.y.value += -5 * 0.02 * (height * 0.1 - m_Mouse.y);

			if (m_Mouse.y > height - height * 0.1)
				scrollbar.y.value += 5 * 0.02 * (m_Mouse.y - (height - height * 0.1));

			if (m_Mouse.x < width * 0.1)
				scrollbar.x.value += -5 * 0.02 * (width * 0.1 - m_Mouse.x);

			if (m_Mouse.x > width - width * 0.1)
				scrollbar.x.value += 5 * 0.02 * (m_Mouse.x - (width - width * 0.1));

			scrollbar.x.ConstrainValue();
			scrollbar.y.ConstrainValue();
		}
	}

	void TextArea::UpdateScroll()
	{
		auto pos = displayer.IndexToPosition(container.selection.start);
		pos.x -= displayer.x;
		pos.y -= displayer.y;

		if (pos.y > scrollbar.y.value + height - displayer.lineHeight - scrollbar.x.height)
			scrollbar.y.value = pos.y - height + displayer.lineHeight + scrollbar.x.height;

		else if (pos.y < scrollbar.y.value)
			scrollbar.y.value = pos.y;

		if (pos.x > scrollbar.x.value + width - 2 - scrollbar.y.width)
			scrollbar.x.value = pos.x - width + 2 + scrollbar.y.width;

		else if (pos.x < scrollbar.x.value + 2)
			scrollbar.x.value = pos.x - 2;

		scrollbar.x.ConstrainValue();
		scrollbar.y.ConstrainValue();
	}
}