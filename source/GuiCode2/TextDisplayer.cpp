#include "GuiCode2/TextDisplayer.hpp"
#include "GuiCode2/Key.hpp"

namespace GuiCode
{
	TextDisplayer::TextDisplayer()
		: container()
	{
		Init();

		min = { 5, 5 };
		size = min;
	}

	TextDisplayer::TextDisplayer(const TextDisplayer& other)
		: container(other.container),
		wrap(other.wrap),
		align(other.align),
		lineHeight(other.lineHeight),
		fontSize(other.fontSize),
		font(other.font),
		placeholder(other.placeholder),
		textColor(other.textColor), 
		selectColor(other.selectColor)
	{
		Init();

		min = other.min;
		size = other.size;
	}

	TextDisplayer::TextDisplayer(TextDisplayer&& other)
		: container(other.container),
		wrap(other.wrap),
		align(other.align),
		lineHeight(other.lineHeight),
		fontSize(other.fontSize),
		font(other.font),
		placeholder(other.placeholder),
		textColor(other.textColor),
		selectColor(other.selectColor)
	{
		Init();

		min = other.min;
		size = other.size;
	}

	void TextDisplayer::Init()
	{
		cursor = Cursor::IBeam;

		listener += [this](const KeyPress& e)
		{
			if (!State(Focused) || e.Handled())
				return;

			m_Timer = 60;

			if (e.keycode == Key::Delete)
				container.Delete(), e.Handle();

			if (e.keycode == Key::Left || e.keycode == Key::Right || e.keycode == Key::Up || e.keycode == Key::Down)
				ChangeIndexActions(e), e.Handle();

			RecalculateLines();
		};

		listener += [this](const KeyType& e)
		{
			if (!State(Focused) || e.Handled())
				return;

			m_Timer = 60;

			if (e.key == Key::Escape)
				return;

			if (e.mod & EventMods::Control)
				CtrlTypeActions(e), e.Handle();
			else if (container.editable)
				KeyTypeActions(e), e.Handle();
		};

		listener += [this](const MouseClick& e)
		{
			if (e.button != MouseButton::Left || (e.mod & EventMods::Shift))
				return;

			if (m_Click > 0 && m_PressPos == Vec2<float>{ e.pos.x, e.pos.y })
				container.selection = { container.CtrlLeft(), container.CtrlRight() };
			container.ConstrainSelection();

			m_PressPos = { e.pos.x, e.pos.y };

			m_Click = 20;
		};

		listener += [this](const MousePress& e)
		{
			if (e.button != MouseButton::Left)
				return;

			m_Timer = 60;

			if (e.mod & EventMods::Shift)
				container.selection = { PositionToIndex({ e.pos.x, e.pos.y }), container.selection.end };
			else
				container.selection = PositionToIndex({ e.pos.x, e.pos.y });
			container.ConstrainSelection();

			UpdateTypeX();

			m_Dragging = true;
		};

		listener += [this](const MouseRelease& e)
		{
			RecalculateLines();
			if (e.button != MouseButton::Left)
				return;

			m_Dragging = false;
		};

		listener += [this](const MouseDrag& e)
		{
			if (~e.buttons & MouseButton::Left)
				return;

			if (m_Dragging)
			{
				container.selection = { PositionToIndex({ e.pos.x, e.pos.y }), container.selection.end };
				container.ConstrainSelection();
				UpdateTypeX();
			}
		};

		listener += [this](const Unfocus& e)
		{
			container.selection = container.selection.start;
			container.ConstrainSelection();
		};

		listener += [this](const Focus& e)
		{
			m_Timer = 60;
			RecalculateLines();
		};
	}

	void TextDisplayer::Update()
	{
		m_Timer--;
		if (!m_Timer)
			m_Timer = 60;

		if (m_Click)
			m_Click--;

		Component::Update();
		height = m_Lines.size() * lineHeight * fontSize + 2;

		if (m_PrevSize != size)
		{
			RecalculateLines();
			m_PrevSize = size;
		}

		if (wrap == Wrap::None)
			width = std::max(m_BiggestX, min.width);
		ConstrainSize();
	}

	void TextDisplayer::Render(CommandCollection& d) const
	{
		if (font == "")
			return;
		d.TextAlign(Align::Left | Align::Middle);
		d.Font(font);
		d.FontSize(fontSize);
		int ypos = 0;
		int beginindex = 0;
		float lineHeight = this->lineHeight * fontSize;
		auto sel = container.selection;
		d.Fill(textColor);
		int index = 0;
		if (m_Lines.size() == 1 && m_Lines[0].length() == 0 && !State(Focused))
		{
			int nx = 0;
			if (align & Align::CenterX)
				nx = width / 2 - m_LineWidths[index] / 2;
			else if (align & Align::Right)
				nx = width - m_LineWidths[index];

			d.Fill(Color{ textColor.r, textColor.g, textColor.b, (float)(textColor.a * 0.5) });
			d.Text(placeholder, { x + nx, y + ypos + lineHeight / 2 });
		}

		for (auto& i : m_Lines)
		{
			if (ypos + y + lineHeight < d.viewport.y || ypos + y > d.viewport.y + d.viewport.height)
			{
				ypos += lineHeight;
				beginindex += i.length();
				index++;
				continue;
			}
			bool low = sel.Lowest() <= beginindex + i.length() && sel.Lowest() > beginindex - 1;
			bool high = sel.Highest() <= beginindex + i.length() && sel.Highest() > beginindex - 1;
			bool between = beginindex > sel.Lowest() && beginindex + i.length() < sel.Highest();
			bool pos = State(Focused) && m_Timer > 30 && container.editable &&
				(sel.start < beginindex + i.length() && sel.start > beginindex - 1 ||
					index == m_Lines.size() - 1 && sel.start == container.content.size());

			int nx = 0;
			if (align & Align::CenterX)
				nx = width / 2 - m_LineWidths[index] / 2;
			else if (align & Align::Right)
				nx = width - m_LineWidths[index];

			/* Selection highlight */
			if (low && !high)
			{
				std::string_view sub1 = i.substr(0, sel.Lowest() - beginindex);
				std::string_view sub2 = i.substr(sel.Lowest() - beginindex);
				float w1 = GraphicsBase::StringWidth(sub1, font, fontSize);
				float w2 = GraphicsBase::StringWidth(sub2, font, fontSize);
				d.Fill(selectColor);
				d.Quad({ x + w1 + nx, y + ypos, w2, lineHeight });
				d.Fill(textColor);
			}

			if (low && high)
			{
				std::string_view sub1 = i.substr(0, sel.Lowest() - beginindex);
				std::string_view sub2 = i.substr(0, sel.Highest() - beginindex);
				float w1 = GraphicsBase::StringWidth(sub1, font, fontSize);
				float w2 = GraphicsBase::StringWidth(sub2, font, fontSize);
				d.Fill(selectColor);
				d.Quad({ x + w1 + nx, y + ypos, w2 - w1, lineHeight });
				d.Fill(textColor);
			}

			if (between)
			{
				float w1 = GraphicsBase::StringWidth(i, font, fontSize);
				d.Fill(selectColor);
				d.Quad({ x + nx, y + ypos, w1, lineHeight });
				d.Fill(textColor);
			}

			if (!low && high)
			{
				std::string_view sub1 = i.substr(0, sel.Highest() - beginindex);
				float w1 = GraphicsBase::StringWidth(sub1, font, fontSize);
				d.Fill(selectColor);
				d.Quad({ x + nx, y + ypos, w1, lineHeight });
				d.Fill(textColor);
			}

			if (pos)
			{
				std::string_view sub1 = i.substr(0, sel.start - beginindex);
				float w1 = GraphicsBase::StringWidth(sub1, font, fontSize);
				d.Fill(textColor);
				d.Quad({ x + nx + w1, y + ypos, 1, lineHeight });
			}
			ypos += lineHeight;
			beginindex += i.length();
			index++;
		}

		ypos = 0;
		index = 0;
		d.Fill(textColor);
		for (auto& i : m_Lines)
		{
			if (ypos + y + lineHeight < d.viewport.y || ypos + y > d.viewport.y + d.viewport.height)
			{
				ypos += lineHeight;
				index++;
				continue;
			}

			int nx = 0;
			if (align & Align::CenterX)
				nx = width / 2 - m_LineWidths[index] / 2;
			else if (align & Align::Right)
				nx = width - m_LineWidths[index];

			d.Text(i, { x + nx, y + ypos + lineHeight / 2 });
			ypos += lineHeight;
			index++;
		}
	}


	int TextDisplayer::PositionToIndex(const Vec2<float>& pos)
	{
		// Get the line _index by dividing the y position minus the padding by the text
		// height. Also contrain the _index to make sure no IndexOutOfBounds is thrown
		float textheight = lineHeight * fontSize;
		int line = (pos.y - y) / textheight;
		line = constrain(line, 0, m_Lines.size() - 1);

		// Increment the _index up to the found line, do +1 because we split it on "\n"
		// so that isn't part of the lines anymore.
		float index = 0;
		for (int i = 0; i < line; i++)
			index += m_Lines[i].length();

		// Calculate the horizontal offset given the text alignment
		float nx = 0;
		if (align & Align::CenterX)
			nx = width / 2 - m_LineWidths[line] / 2;
		else if (align & Align::Right)
			nx = width - m_LineWidths[line];

		// Now calculate the _index in the line by checking each character's x-position
		// until we passes the given x-position.
		auto thisLine = m_Lines[line];
		for (int i = 0; i < thisLine.length(); i++)
		{
			// Width of the current character
			float w = GraphicsBase::CharWidth(thisLine[i], font, fontSize);

			// If the current width of the line + half the width of the current character +
			// paddingX is bigger than the x, we've found our _index.
			if (nx + w / 2 > pos.x - x)
				return index + i;

			// Increament the width of this line by the character width
			nx += w;
		}

		// If the given x-position is even bigger, it means it's after the end of the
		// line, so add the entire length (-1 because of the '\n', except on last line )
		// of the line and return.
		if (line == m_Lines.size() - 1)
			return index + m_Lines[line].length();
		else
			return index + m_Lines[line].length() - 1;
	}

	Vec2<float> TextDisplayer::IndexToPosition(int index)
	{
		// Get the font metrics of the Graphics2D
		int textheight = lineHeight * fontSize;

		int line = 0;
		int in = index;
		while (in >= 0 && line != m_Lines.size())
			in -= m_Lines[line].length(), line++;

		line--;
		in += m_Lines[line].length();

		// Calculate the position using the _index and the font metrics
		float ex = GraphicsBase::StringWidth(m_Lines[line].substr(0, in), font, fontSize);
		float ey = line * textheight;
		if (align & Align::CenterX)
			ex += width / 2 - m_LineWidths[line] / 2;
		else if (align == Align::Right)
			ex += width - m_LineWidths[line];
		return { x + ex, y + ey };
	}

	void TextDisplayer::RecalculateLines()
	{
		switch (wrap)
		{
		case Wrap::None: CalcLinesNoWrap(); break;
		case Wrap::Character: CalcLinesNoWrap(); CalcLinesCharWrap(); break;
		case Wrap::Word: CalcWords(); CalcLinesWordWrap(); break;
		}
	}

	void TextDisplayer::CalcLinesWordWrap()
	{
		enum Type { None, Alnum, Whitespace, Character, Newline };

		m_Lines.clear(), m_LineWidths.clear();

		std::string_view _content = container.content;
		StringView _currentLine = { 0, 0 };
		float _currentWidth = 0;
		for (int i = 0; i < m_Words.size(); i++)
		{
			auto& [_word, _type] = m_Words[i];
			float _wordWidth = GraphicsBase::StringWidth(_content.substr(_word.start, _word.end - _word.start), font, fontSize);

			switch (_type)
			{
			case Character:
			case Alnum:
				if (_wordWidth + _currentWidth > width)
				{
					m_Lines.push_back({ _content.begin() + _currentLine.start, _content.begin() + _currentLine.end });
					m_LineWidths.push_back(_currentWidth);

					_currentLine.start = _currentLine.end, _currentWidth = 0;
				}
			case Whitespace:
				_currentLine.end = _word.end, _currentWidth += _wordWidth;
				break;
			case Newline:
				m_Lines.push_back({ _content.begin() + _currentLine.start, _content.begin() + _word.end });
				m_LineWidths.push_back(_currentWidth);

				_currentLine = { _word.end, _word.end }, _currentWidth = 0;
				break;
			}
		}

		if (_currentLine.end != _currentLine.start)
		{
			m_Lines.push_back({ _content.begin() + _currentLine.start, _content.begin() + _currentLine.end });
			m_LineWidths.push_back(_currentWidth);
		}

		if (m_Lines.size() == 0)
			m_Lines.push_back(_content), m_LineWidths.push_back(0);

		if (_content.size() > 0 && *(_content.end() - 1) == '\n')
			m_Lines.push_back("\n"), m_LineWidths.push_back(0);
	}

	void TextDisplayer::CalcLinesCharWrap()
	{
		int size = m_Lines.size();
		for (int i = 0; i < size; i++)
		{
			int w = 0;
			int camt = m_Lines[i].size();
			for (int j = 0; j < camt; j++)
			{
				float nw = GraphicsBase::CharWidth(m_Lines[i][j], font, fontSize);
				w += nw;
				if (w > width)
				{
					auto left = m_Lines[i].substr(0, j);
					auto right = m_Lines[i].substr(j);
					m_Lines[i] = left;
					m_Lines.emplace(m_Lines.begin() + i + 1, right);
					m_LineWidths[i] = w - nw;
					m_LineWidths.emplace(m_LineWidths.begin() + i + 1, GraphicsBase::StringWidth(m_Lines[i + 1], font, fontSize));
					size++;
					break;
				}
			}
		}
	}

	void TextDisplayer::CalcLinesNoWrap()
	{
		m_BiggestX = 0;
		m_Lines.clear();
		m_LineWidths.clear();
		std::string_view str = container.content;
		std::string delims = "\n";
		for (auto first = str.data(), second = str.data(), last = first + str.size(); second != last && first != last; first = second + 1) {
			second = std::find_first_of(first, last, delims.begin(), delims.end());

			std::string_view* now;
			if (first != second)
				if (*(second) == '\n')
					now = &m_Lines.emplace_back(first, second - first + 1);
				else
					now = &m_Lines.emplace_back(first, second - first);
			else
				if (*(second) == '\n')
					now = &m_Lines.emplace_back("\n");
				else
					now = &m_Lines.emplace_back("");

			float w = GraphicsBase::StringWidth(*now, font, fontSize);
			m_LineWidths.push_back(w);
			if (w > m_BiggestX)
				m_BiggestX = w;
		}

		if (m_Lines.size() == 0)
		{
			m_Lines.emplace_back(str),
				m_BiggestX = 0;
			m_LineWidths.push_back(0);
		}
		if (str.size() > 0 && *(str.end() - 1) == '\n')
		{
			m_Lines.emplace_back("\n");
			m_LineWidths.push_back(0);
		}
		m_BiggestX += 2;
	}

	void TextDisplayer::CalcWords()
	{
		m_Words.clear();
		std::string_view _view = container.content;
		enum Type { None, Alnum, Whitespace, Character, Newline } type = None;
		size_t _prev = 0;
		for (size_t i = 0; i < _view.size() + 1; i++)
		{
			char c = '\0';
			if (i < _view.size())
				c = _view[i];
			
			if (std::isalnum(c, std::locale()))
			{
				if (type == Whitespace)
					m_Words.push_back({ { _prev, i }, type });

				if (type != Alnum)
					_prev = i;

				type = Alnum;
			}
			else if (c == '\n')
			{
				if (type != Character && type != None && type != Newline)
					m_Words.push_back({ { _prev, i }, type });

				if (c != 0)
					m_Words.push_back({ { i, i + 1} , Newline });

				_prev = i;
				type = Newline;
			}
			else if (std::isspace(c, std::locale()))
			{
				if (type == Alnum)
					m_Words.push_back({ { _prev, i }, type });

				if (type != Whitespace)
					_prev = i;
				
				type = Whitespace;
			} 
			else 
			{
				if (type != Character && type != None && type != Newline)
					m_Words.push_back({ { _prev, i }, type });

				if (c != 0)
					m_Words.push_back({ { i, i + 1} , Character });

				_prev = i;
				type = Character;
			}
		}
	}

	void TextDisplayer::KeyTypeActions(const KeyType& e)
	{
		switch (e.key)
		{
		case Key::Backspace: container.Backspace(); break;
		case Key::Tab: container.Insert("    "); break;
		case Key::Enter: container.Insert("\n"); break;
		default: container.Insert(e.key);
		}
		container.ConstrainSelection();
		RecalculateLines();
	}

	void TextDisplayer::Cut() 
	{
		Clipboard::Get().Copy(std::string(container.SelectionString()));
		if (!container.editable)
			return;

		container.RemoveSelection();
		RecalculateLines();
	}

	void TextDisplayer::Copy() 
	{
		Clipboard::Get().Copy(std::string(container.SelectionString()));
	}

	void TextDisplayer::Paste() 
	{
		if (!container.editable)
			return;

		container.Insert(Clipboard::Get().Paste());
		RecalculateLines();
	}

	void TextDisplayer::CtrlTypeActions(const KeyType& e)
	{
		char key = e.key;
		if (key == 127 && container.editable)
		{
			if (container.selection.Selected())
				container.RemoveSelection();
			else
			{
				int ctrl = container.CtrlLeft();
				container.Remove({ ctrl, container.selection.start });
				container.selection = ctrl;
			}
			RecalculateLines();
		}
		else if (key == 1)
		{
			container.selection = { container.Length(), 0 };
		}
		else if ((int)key == 3)
		{
			Copy();
		}
		else if ((int)key == 24)
		{
			Cut();
		}
		else if ((int)key == 22)
		{
			Paste();
		}
		container.ConstrainSelection();
	}

	void TextDisplayer::ChangeIndexActions(const KeyPress& e)
	{
		int index = container.selection.start;

		// If theres a selection and shift is not pressed, go to the end of start of the
		// selection depending on the key.
		if (container.selection.Selected() && !(e.mod & EventMods::Shift))
		{
			if (e.keycode == Key::Left)
				container.selection = container.selection.Lowest();
			else if (e.keycode == Key::Right)
				container.selection = container.selection.Highest();
			else if (e.keycode == Key::Up)
				container.selection = PositionToIndex({ m_TypeX, -fontSize * lineHeight + IndexToPosition(container.selection.Lowest()).y });
			else if (e.keycode == Key::Down)
				container.selection = PositionToIndex({ m_TypeX, fontSize * lineHeight + IndexToPosition(container.selection.Highest()).y });
			
			container.ConstrainSelection();
			return;
		}

		// If _ctrlRightIndex is down move the _index an entire 'word'
		if (e.mod & EventMods::Control)
		{
			if (e.keycode == Key::Left)
				index = container.CtrlLeft();
			else if (e.keycode == Key::Right)
				index = container.CtrlRight();
			else if (e.keycode == Key::Up)
				index = PositionToIndex({ m_TypeX, -fontSize * lineHeight + IndexToPosition(container.selection.start).y });
			else if (e.keycode == Key::Down)
				index = PositionToIndex({ m_TypeX, fontSize * lineHeight + IndexToPosition(container.selection.start).y });
		}

		// Otherwise just normal _index adjusting
		else
		{
			if (e.keycode == Key::Left)
				index--;
			else if (e.keycode == Key::Right)
				index++;
			else if (e.keycode == Key::Up)
				index = PositionToIndex({ m_TypeX, -fontSize * lineHeight + IndexToPosition(container.selection.start).y });
			else if (e.keycode == Key::Down)
				index = PositionToIndex({ m_TypeX, fontSize * lineHeight + IndexToPosition(container.selection.start).y });
		}

		if (!(e.mod & EventMods::Shift))
			container.selection = index;
		else
			container.selection = { index, container.selection.end };

		container.ConstrainSelection();
		if (e.keycode == Key::Left || e.keycode == Key::Right)
			UpdateTypeX();
	}

	void TextDisplayer::UpdateTypeX()
	{
		m_TypeX = IndexToPosition(container.selection.start).x;
	}

	/**
	 * Parser
	 */

	TextDisplayerParser::TextDisplayerParser()
	{
		settings.name = "text-displayer";
		Attribute("wrap", &TextDisplayer::wrap);
		Attribute("align", &TextDisplayer::align);
		Attribute("line-height", &TextDisplayer::lineHeight);
		Attribute("font-size", &TextDisplayer::fontSize);
		Attribute("font", &TextDisplayer::font);
		Attribute("placeholder", &TextDisplayer::placeholder);
		Attribute("text-color", &TextDisplayer::textColor);
		Attribute("select-color", &TextDisplayer::selectColor);
		Attribute("editable", &TextDisplayer::m_Editable);
		Attribute("content", &TextDisplayer::m_Content);
		enumMap["word"] = (int)Wrap::Word;
		enumMap["character"] = (int)Wrap::Character;
	}

	Pointer<Component> TextDisplayerParser::Create()
	{
		return new TextDisplayer{};
	}
}