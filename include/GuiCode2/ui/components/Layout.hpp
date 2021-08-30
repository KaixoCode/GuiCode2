#pragma once

// --------------------------------------------------------------------------
// ------------------------------- Layout -----------------------------------
// --------------------------------------------------------------------------

struct Layout 
{
	enum Type 
	{
		Grid, Stack, Border, Free
	};

	enum class Hint
	{
		None, North, South, East, West, Center
	};

	Layout(Type type)
		: type(type)
	{}
	
	Layout(Type type, int p)
		: type(type), border({ p })
	{}

	Layout(Type type, int c, int r, int h, int v)
		: type(type), grid({ c, r, h, v })
	{}

	~Layout() {}

	Type type;
	
	operator Type() const { return type; }

	struct GridLayout // Grid Layout
	{
		int cols, rows, hgap, vgap;
	};

	struct BorderLayout // Grid Layout
	{
		int padding;
	};

	struct StackLayout // Grid Layout
	{
		int padding;
	};

	union
	{
		GridLayout grid;
		BorderLayout border;
		StackLayout stack;
	};
};
