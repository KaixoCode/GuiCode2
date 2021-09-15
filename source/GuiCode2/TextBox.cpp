#include "GuiCode2/TextBox.hpp"

namespace GuiCode
{
	TextBox::TextBox()
		: Panel{ {.ratio = 1, .overflow = { Overflow::Hide, Overflow::Hide }} },
		align(displayer.align),
		container(displayer.container),
		font(displayer.font),
		fontSize(displayer.fontSize),
		placeholder(displayer.placeholder),
		textColor(displayer.textColor),
		selectColor(displayer.selectColor)
	{
		panels.push_back(Panel{ {.ratio = 0, .size{ Auto, Auto } }, displayer });
		
		listener += [this](const MousePress& e)
		{
			m_Mouse = e.pos - position;
			UpdateScroll();
			m_Dragging = true;
		};

		listener += [this](const MouseRelease& e)
		{
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

	void TextBox::Update()
	{
		displayer.min = size;
		displayer.height = height;
		displayer.lineHeight = displayer.height;
		displayer.wrap = Wrap::None;

		if (m_Update)
		{
			m_Update--;
			UpdateScroll();
		}

		if (m_Dragging & MouseButton::Left)
		{
			if (m_Mouse.x < width * 0.1)
				scrollbar.x.value += -5 * 0.02 * (width * 0.1 - m_Mouse.x);

			if (m_Mouse.x > width - width * 0.1)
				scrollbar.x.value += 5 * 0.02 * (m_Mouse.x - (width - width * 0.1));

			scrollbar.x.ConstrainValue();
		}
	}

	void TextBox::UpdateScroll()
	{
		auto pos = displayer.IndexToPosition(container.selection.start);
		pos.x -= displayer.x;
		pos.y -= displayer.y;

		if (pos.x > scrollbar.x.value + width - 2)
			scrollbar.x.value = pos.x - width + 2;

		else if (pos.x < scrollbar.x.value + 2)
			scrollbar.x.value = pos.x - 2;

		scrollbar.x.ConstrainValue();
	}
}