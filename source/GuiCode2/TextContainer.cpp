#include "GuiCode2/TextContainer.hpp"

namespace GuiCode
{
	void TextContainer::Insert(const std::string& add)
	{
		RemoveSelection();
		auto _start = content.begin() + selection.Lowest();
		content.insert(_start, add.begin(), add.end());
		selection = selection.Lowest() + add.length();
	}

	void TextContainer::Insert(const char& add)
	{
		RemoveSelection();
		auto _start = content.begin() + selection.Lowest();
		content.insert(_start, add);
		selection = selection.Lowest() + 1;
	}

	void TextContainer::Delete()
	{
		if (selection.Selected())
			RemoveSelection();

		else if (selection.Highest() < Length())
		{
			auto _start = content.begin() + selection.Lowest();
			content.erase(_start, _start + 1);
			selection = selection.Lowest();
		}
	}

	void TextContainer::Backspace()
	{
		if (selection.Selected())
			RemoveSelection();

		else if (selection.Lowest() > 0)
		{
			auto _start = content.begin() + selection.Lowest();
			content.erase(_start - 1, _start);
			selection = selection.Lowest() - 1;
		}
	}

	void TextContainer::RemoveSelection()
	{
		Remove(selection);
		selection = selection.Lowest();
		ConstrainSelection();
	}

	void TextContainer::Remove(const Selection& s)
	{
		auto _start = content.begin() + s.Lowest();
		auto _end = content.begin() + s.Highest();
		content.erase(_start, _end);
		ConstrainSelection();
	}

	int TextContainer::CtrlLeft() const
	{
		return CtrlLeft(selection.start);
	}

	int TextContainer::CtrlLeft(int start) const
	{
		if (start > Length())
			return Length();

		if (start < 0)
			return 0;

		// Get the text up until the type index
		std::string_view text = content;
		text = text.substr(0, start);

		// If there is no text, keep the same
		if (text.length() == 0)
			return start;

		// Get the type of character at the end of the string
		bool letter = text[text.length() - 1] < 0 ? false : std::isalpha(text[text.length() - 1]);

		// Loop through each index from back to front
		for (int index = start - 1; index >= 0; index--)
		{
			// If there's a change in character type, return the index + 1, because we want
			// the index after the change in character type.
			bool alpha = text[index] < 0 ? false : std::isalpha(text[index]);
			if (letter != alpha)
				return index + 1;
		}

		// Otherwise if it went through the entire loop, it's at the start of the text,
		// so index will be 0.
		return 0;
	}

	int TextContainer::CtrlRight() const
	{
		return CtrlRight(selection.start);
	}

	int TextContainer::CtrlRight(int start) const
	{
		if (start > Length())
			return Length();

		if (start < 0)
			return 0;

		// Get the text from index to end
		std::string_view text = content;
		text = text.substr(start);

		// If there is no text, keep the same
		if (text.length() == 0)
			return start;

		// Get the type of character at the start of the string
		bool letter = text[0] < 0 ? false : std::isalpha(text[0]);

		// Loop through each index from front to back
		for (int index = 0; index < text.length(); index++)
		{
			// If there's a change in character type, return the index - 1 + start, because
			// we want the index after the change in character type.
			bool alpha = text[index] < 0 ? false : std::isalpha(text[index]);
			if (letter != alpha)
				return index + start;
		}
		// Otherwise if it went through the entire loop, it's at the end of the text,
		// so index will be length.
		return Length();
	}
}