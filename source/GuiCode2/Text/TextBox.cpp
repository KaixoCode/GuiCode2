#include "GuiCode2/Text/TextBox.hpp"
#include "GuiCode2/Events/Key.hpp"

namespace GuiCode
{
	TextBox::TextBox()
		: Panel{ {.ratio = 0, .overflow = { Overflow::Hide, Overflow::Hide }} }
	{
		Init();
	}

	TextBox::TextBox(TextBox&& other)
		: Panel{ {.ratio = 0, .overflow = { Overflow::Hide, Overflow::Hide }} },
		displayer(other.displayer)
	{
		Init();
	}

	TextBox::TextBox(const TextBox& other)
		: Panel{ {.ratio = 0, .overflow = { Overflow::Hide, Overflow::Hide }} },
		displayer(other.displayer)
	{
		Init();
	}

	void TextBox::Init()
	{
		panels.push_back(new Panel{ {.ratio = 0, .size{ Auto, Auto } }, displayer });
		cursor = Cursor::IBeam;

		displayer += [this](const KeyPress& e)
		{
			if (e.keycode == Key::Enter)
			{
				State(Focused) = false;
				HandleEvent(Unfocus{});
				e.Handle();
			}
		};

		*this += [this](const MousePress& e)
		{
			m_Mouse = e.pos - position;
			UpdateScroll();
			m_Dragging = true;
		};

		*this += [this](const MouseRelease& e)
		{
			m_Dragging = false;
		};

		*this += [this](const MouseDrag& e)
		{
			m_Mouse = e.pos - position;
		};

		*this += [this](const KeyPress& e)
		{
			if (e.Handled())
				m_Update = 2;
		};

		*this += [this](const KeyType& e)
		{
			if (e.Handled())
				m_Update = 2;
		};
	}

	void TextBox::Update()
	{
		displayer.min = size;
		displayer.height = height;
		displayer.lineHeight = height / fontSize;
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