#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/components/Component.hpp"

// --------------------------------------------------------------------------
// ------------------------- Menu accessories -------------------------------
// --------------------------------------------------------------------------

namespace MenuAccessories
{
	// --------------------------------------------------------------------------
	// ------------------------------ Divider -----------------------------------
	// --------------------------------------------------------------------------

	class Divider : public Component
	{
	public:
		Divider(int size, int stroke, int paddingx, int paddingy)
			: m_Size(size), m_PaddingX(paddingx), m_PaddingY(paddingy), m_Stroke(stroke), Component({ size, stroke + paddingy * 2 })
		{}

		void Render(CommandCollection&) override;
		bool WithinBounds(const Vec2<int>&) const override { return false; }

	private:
		int m_Size,
			m_PaddingX,
			m_PaddingY,
			m_Stroke;
	};
}