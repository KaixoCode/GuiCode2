#pragma once
#include "GuiCode2/pch.hpp"

namespace GuiCode
{
	/**
	 * Simple selection class, marks the start and end of a selection.
	 */
	struct Selection
	{
		int start = 0;
		int end = 0;

		bool Selected() const { return start != end; }
		int Lowest() const { return start < end ? start : end; }
		int Highest() const { return start < end ? end : start; }

		Selection& operator=(int a) { start = end = a; return *this; }
	};

	/**
	 * 
	 */
	class TextContainer
	{
	public:
		TextContainer();

		auto SelectionString() -> std::string_view { return std::string_view(content).substr(selection.Lowest(), (size_t)selection.Highest() - selection.Lowest()); }

		void Insert(const std::string& add);
		void Insert(const char& add);

		void Delete();
		void Backspace();

		void RemoveSelection();
		void Remove(const Selection& s);

		int CtrlLeft();
		int CtrlLeft(int start);
		int CtrlRight();
		int CtrlRight(int start);

		int Length() { return content.length(); }

		bool editable = true;
		std::string content;
		Selection selection{ 0, 0 };

		void ConstrainSelection() { selection = { constrain(selection.start, 0, Length()), constrain(selection.end, 0, Length()) }; }
	};
}