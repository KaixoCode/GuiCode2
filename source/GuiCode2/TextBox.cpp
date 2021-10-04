#include "GuiCode2/TextBox.hpp"
#include "GuiCode2/Key.hpp"

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

		displayer.listener += [this](const KeyPress& e)
		{
			if (e.keycode == Key::Enter)
			{
				State<Focused>(false);
				listener(Unfocus{});
				e.Handle();
			}
		};

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

	void Text::Update()
	{
		TextBox::Update();
		Panel::settings.min.width = GraphicsBase::StringWidth(content, font, fontSize) + 4;
		editable = false;
	}

	TextBoxParser::TextBoxParser()
	{
		settings.name = "textbox";
		Attribute("text-align", &TextBox::m_Align);
		Attribute("font-size", &TextBox::m_FontSize);
		Attribute("font", &TextBox::m_Font);
		Attribute("placeholder", &TextBox::m_Placeholder);
		Attribute("text-color", &TextBox::m_TextColor);
		Attribute("select-color", &TextBox::m_SelectColor);
		Attribute("editable", &TextBox::m_Editable);
		Attribute("content", &TextBox::m_Content);
	}

	Pointer<Component> TextBoxParser::Create()
	{
		return new TextBox{};
	}

	TextParser::TextParser()
	{
		settings.name = "text";
	}

	Pointer<Component> TextParser::Create()
	{
		return new Text{};
	}
}