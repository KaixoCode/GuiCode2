#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/Component.hpp"

// --------------------------------------------------------------------------
// -------------------------- Scrollbar Base --------------------------------
// --------------------------------------------------------------------------

class ScrollbarBase : public Component
{
public:
	ScrollbarBase();

	auto Range(const Vec2<int>& r) -> void const { m_Range = r; ConstrainValue(); }
	auto Scroll(int s)			   -> void const { m_Value = constrain(s + m_Value, m_Range.start, m_Range.end - m_VisibleRange); }
	auto Value(int v)			   -> void const { m_Value = constrain(v, m_Range.start, m_Range.end - m_VisibleRange); }
	auto VisibleRange(int v)       -> void const { m_VisibleRange = v; ConstrainValue(); }

	int       MinSize()      const { return 40; }
	int       Value()        const { return m_Value; }
	int       Mouse()        const { return m_Mouse; }
	int       VisibleRange() const { return m_VisibleRange; }
	Vec2<int> Range()        const { return m_Range; }
	bool      NotNecessary() const { return m_VisibleRange >= m_Range.end - m_Range.start; }
	virtual bool Hovering()  const { return m_Hovering; }

	void Update(const Vec4<int>& viewport) override;

protected:
	Vec2<int> m_Range{0, 100};
	
	int m_VisibleRange = 20,
		m_Value = 0,
		m_Mouse = 0,
		m_PressMouse = 0,
		m_PressValue = 0;

	bool m_Hovering = false;

	void ConstrainValue() { m_Value = constrain(m_Value, m_Range.start, m_Range.end - m_VisibleRange); }
};

// --------------------------------------------------------------------------
// ----------------------------- Scrollbar ----------------------------------
// --------------------------------------------------------------------------

template<typename ScrollbarGraphics, typename ScrollbarType = ScrollbarBase>
class Scrollbar : public ScrollbarType
{
	using ScrollbarType::ScrollbarType;

	void Render(CommandCollection& d) override
	{
		ScrollbarType::Render(d);
		ScrollbarGraphics::Render(*this, d);
	};
};