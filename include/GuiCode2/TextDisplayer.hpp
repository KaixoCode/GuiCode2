#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Component.hpp"
#include "GuiCode2/TextContainer.hpp"
#include "GuiCode2/Graphics.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Clipboard.hpp"

namespace GuiCode
{
	enum class Wrap
	{
		Word, Character, None
	};

	class TextDisplayer : public Component
	{
		struct StringView { size_t start, end; };
	public:
		TextDisplayer();
		TextDisplayer(const TextDisplayer&);
		TextDisplayer(TextDisplayer&&);

		void Update() override;
		void Render(CommandCollection& d) const override;

		int PositionToIndex(const Vec2<float>& pos);
		Vec2<float> IndexToPosition(int index);

		void RecalculateLines();

		Wrap wrap = Wrap::Word;
		int align = Align::Left | Align::Top;
		float lineHeight = 1;
		float fontSize = 14;
		std::string_view font;
		TextContainer container;
		std::string placeholder;
		Color textColor{ 255, 255, 255, 255 };
		Color selectColor{ 128, 128, 255, 255 };

	protected:
		float m_BiggestX = 0;
		float m_TypeX = 0;
		float m_Click = 0;
		float m_Timer = 60;
		bool m_Dragging = false;

		Vec2<float> m_PressPos{ 0, 0 };
		Vec2<float> m_PrevSize{ 0, 0 };

		std::vector<std::string_view> m_Lines;
		std::vector<std::pair<StringView, int>> m_Words;
		std::vector<float> m_LineWidths;

		void CalcLinesWordWrap();
		void CalcLinesCharWrap();
		void CalcLinesNoWrap();
		void CalcWords();

		void KeyTypeActions(const KeyType& e);
		void CtrlTypeActions(const KeyType& e);
		void ChangeIndexActions(const KeyPress& e);

		void UpdateTypeX();
		void Init();
	};
}