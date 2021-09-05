#include "GuiCode2/TextDisplayer.hpp"
#include "GuiCode2/Key.hpp"

namespace GuiCode
{
	TextDisplayer::TextDisplayer()
		: container()
	{
		listener += [this](const KeyRelease& e)
		{
			if (e.keycode == Key::SHIFT)
				m_Shift = false;
		};

		listener += [this](const KeyPress& e)
		{
			if (!State<Focused>())
				return;

			m_Timer = 60;

			if (e.keycode == Key::SHIFT)
				m_Shift = true;

			if (e.keycode == Key::DEL)
				container.Delete();

			if (e.keycode == Key::LEFT || e.keycode == Key::RIGHT || e.keycode == Key::UP || e.keycode == Key::DOWN)
				ChangeIndexActions(e);
			RecalculateLines();
		};

		listener += [this](const KeyType& e)
		{
			if (!State<Focused>())
				return;

			m_Timer = 60;

			if (e.key == Key::TAB)
				return;

			if (e.mod & EventMods::Control)
				CtrlTypeActions(e);
			else if (container.editable)
				KeyTypeActions(e);
			RecalculateLines();
		};

		listener += [this](const MouseClick& e)
		{
			if (e.button != MouseButton::Left || m_Shift)
				return;

			if (m_Click > 0 && m_PressPos == Vec2<float>{ e.pos.x, e.pos.y })
				container.selection = { container.CtrlLeft(), container.CtrlRight() };

			m_PressPos = { e.pos.x, e.pos.y };

			m_Click = 20;
		};

		listener += [this](const MousePress& e)
		{
			if (e.button != MouseButton::Left)
				return;

			m_Timer = 60;

			if (m_Shift)
				container.selection = { PositionToIndex({ e.pos.x, e.pos.y }), container.selection.end };
			else
				container.selection = PositionToIndex({ e.pos.x, e.pos.y });

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
				UpdateTypeX();
			}
		};

		listener += [this](const Unfocus& e)
		{
			container.selection = container.selection.start;
		};

		listener += [this](const Focus& e)
		{
			m_Timer = 60;
			RecalculateLines();
		};

		min = { 50, 50 };
		size = min;

		//m_Cursor = GLFW_IBEAM_CURSOR;
	}

	void TextDisplayer::Update()
	{
		m_Timer--;
		if (!m_Timer)
			m_Timer = 60;

		if (m_Click)
			m_Click--;

		Component::Update();
		height = m_Lines.size() * lineHeight + 2;


		if (m_PrevSize != size)
		{
			RecalculateLines();
			m_PrevSize = size;
		}

		if (wrap == Wrap::None)
			width = std::max(m_BiggestX, min.width);
	}

	void TextDisplayer::Render(CommandCollection& d) const
	{
		if (font == "")
			return;
		d.TextAlign(Align::Left | Align::CenterY);
		d.Font(font);
		d.TextSize(fontSize);
		int ypos = 0;
		int beginindex = 0;
		auto sel = container.selection;
		d.Fill(textColor);
		int index = 0;
		if (m_Lines.size() == 1 && m_Lines[0].length() == 0 && !State<Focused>())
		{
			int nx = 0;
			if (align == Align::CenterX)
				nx = width / 2 - m_LineWidths[index] / 2;
			else if (align == Align::Right)
				nx = width - m_LineWidths[index];

			d.Fill(Color{ textColor.r, textColor.g, textColor.b, (float)(textColor.a * 0.5) });
			d.Text(placeholder, { x + nx, y + ypos + lineHeight / 2 });
		}

		for (auto& i : m_Lines)
		{
			//if (y + Y() + lineheight < m_Viewport.y || y + Y() > m_Viewport.y + m_Viewport.height)
			//{
			//	y -= lineheight;
			//	beginindex += i.length();
			//	index++;
			//	continue;
			//}
			bool low = sel.Lowest() <= beginindex + i.length() && sel.Lowest() > beginindex - 1;
			bool high = sel.Highest() <= beginindex + i.length() && sel.Highest() > beginindex - 1;
			bool between = beginindex > sel.Lowest() && beginindex + i.length() < sel.Highest();
			bool pos = State<Focused>() && m_Timer > 30 && container.editable &&
				(sel.start < beginindex + i.length() && sel.start > beginindex - 1 ||
					index == m_Lines.size() - 1 && sel.start == container.content.size());

			int nx = 0;
			if (align == Align::CenterX)
				nx = width / 2 - m_LineWidths[index] / 2;
			else if (align == Align::Right)
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

			d.Text(i, { x + nx, y + ypos + lineHeight / 2 });
			ypos += lineHeight;
			beginindex += i.length();
			index++;
		}
	}


	int TextDisplayer::PositionToIndex(const Vec2<float>& pos)
	{
		// Get the line index by dividing the y position minus the padding by the text
		// height. Also contrain the index to make sure no IndexOutOfBounds is thrown
		float textheight = lineHeight;
		int line = (pos.y - y) / textheight;
		line = constrain(line, 0, m_Lines.size() - 1);

		// Increment the index up to the found line, do +1 because we split it on "\n"
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

		// Now calculate the index in the line by checking each character's x-position
		// until we passes the given x-position.
		auto thisLine = m_Lines[line];
		for (int i = 0; i < thisLine.length(); i++)
		{
			// Width of the current character
			float w = GraphicsBase::CharWidth(thisLine[i], font, fontSize);

			// If the current width of the line + half the width of the current character +
			// paddingX is bigger than the x, we've found our index.
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
		int textheight = lineHeight;

		int line = 0;
		int in = index;
		while (in >= 0 && line != m_Lines.size())
			in -= m_Lines[line].length(), line++;

		line--;
		in += m_Lines[line].length();

		// Calculate the position using the index and the font metrics
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
		case Wrap::Word: CalcLinesNoWrap(); CalcLinesWordWrap(); break;
		}
	}

	void TextDisplayer::CalcLinesWordWrap()
	{
		// Big magic, good luck.

		int size = m_Lines.size();
		int index = 0;
		for (int i = 0; i < size; i++)
		{
			float w = 0;
			int lineindex = index;
			int camt = m_Lines[i].size();
			while (index - lineindex < camt)
			{
				int ctrl = container.CtrlRight(index);
				int j = index - lineindex;
				int len = ctrl - lineindex - j;

				if (j + len > m_Lines[i].length() || j < 0 || j > m_Lines[i].length())
					break;

				float _width = GraphicsBase::StringWidth(m_Lines[i].substr(j, len), font, fontSize);
				if (_width > width)
				{
					int b = j;
					for (int k = j; k < camt; k++)
					{
						float nw = GraphicsBase::CharWidth(m_Lines[i][k], font, fontSize);
						w += nw;
						if (w > width)
						{
							auto left = m_Lines[i].substr(0, k);
							auto right = m_Lines[i].substr(k);
							m_Lines[i] = left;
							m_Lines.emplace(m_Lines.begin() + i + 1, right);
							m_LineWidths[i] = w - nw;
							m_LineWidths.emplace(m_LineWidths.begin() + i + 1, GraphicsBase::StringWidth(m_Lines[i + 1], font, fontSize));
							size++;
							b = k;
							break;
						}
						index++;
					}
					//if (index == b)
					//	index++;
					//else
					//	index = b;

					break;
				}

				w += _width;
				if (w > width)
				{
					auto left = m_Lines[i].substr(0, j);
					auto right = m_Lines[i].substr(j);
					m_Lines[i] = left;
					m_Lines.emplace(m_Lines.begin() + i + 1, right);
					m_LineWidths[i] = w - _width;
					m_LineWidths.emplace(m_LineWidths.begin() + i + 1, GraphicsBase::StringWidth(m_Lines[i + 1], font, fontSize));
					size++;
					break;
				}

				if (index == ctrl)
					index++;
				else
					index = ctrl;
			}
		}
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
	}

	void TextDisplayer::KeyTypeActions(const KeyType& e)
	{
		switch (e.key)
		{
		case Key::BACKSPACE: container.Backspace(); break;
		case Key::TAB: container.Insert("    "); break;
		case Key::ENTER: container.Insert("\n"); break;
		default: container.Insert(e.key);
		}
		container.ConstrainSelection();
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
		}
		else if (key == 1)
		{
			container.selection = { container.Length(), 0 };
		}
		else if ((int)key == 3)
		{
			//Clipboard::Get().Copy(std::string(container.SelectionString()));
		}
		else if ((int)key == 24 && container.editable)
		{
			//Clipboard::Get().Copy(std::string(container.SelectionString()));

			container.RemoveSelection();
		}
		else if ((int)key == 22 && container.editable)
		{
			//container.Insert(Clipboard::Get().Paste());
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
			if (e.keycode == Key::LEFT)
				container.selection = container.selection.Lowest();
			else if (e.keycode == Key::RIGHT)
				container.selection = container.selection.Highest();
			else if (e.keycode == Key::UP)
				container.selection = PositionToIndex({ m_TypeX, -lineHeight + IndexToPosition(container.selection.Lowest()).y });
			else if (e.keycode == Key::DOWN)
				container.selection = PositionToIndex({ m_TypeX, lineHeight + IndexToPosition(container.selection.Highest()).y });
			
			container.ConstrainSelection();
			return;
		}

		// If ctrl is down move the index an entire 'word'
		if (e.mod & EventMods::Control)
		{
			if (e.keycode == Key::LEFT)
				index = container.CtrlLeft();
			else if (e.keycode == Key::RIGHT)
				index = container.CtrlRight();
			else if (e.keycode == Key::UP)
				index = PositionToIndex({ m_TypeX, -lineHeight + IndexToPosition(container.selection.start).y });
			else if (e.keycode == Key::DOWN)
				index = PositionToIndex({ m_TypeX, lineHeight + IndexToPosition(container.selection.start).y });
		}

		// Otherwise just normal index adjusting
		else
		{
			if (e.keycode == Key::LEFT)
				index--;
			else if (e.keycode == Key::RIGHT)
				index++;
			else if (e.keycode == Key::UP)
				index = PositionToIndex({ m_TypeX, -lineHeight + IndexToPosition(container.selection.start).y });
			else if (e.keycode == Key::DOWN)
				index = PositionToIndex({ m_TypeX, lineHeight + IndexToPosition(container.selection.start).y });
		}

		if (!(e.mod & EventMods::Shift))
			container.selection = index;
		else
			container.selection = { index, container.selection.end };

		container.ConstrainSelection();
		if (e.keycode == Key::LEFT || e.keycode == Key::RIGHT)
			UpdateTypeX();
	}

	void TextDisplayer::UpdateTypeX()
	{
		m_TypeX = IndexToPosition(container.selection.start).x;
	}
}