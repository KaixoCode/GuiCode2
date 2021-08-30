//#pragma once
//#include "GuiCode/ui/Events.hpp"
//#include "GuiCode/ui/Components/ComponentBase.hpp"
//#include "GuiCode/ui/Components/Menu.hpp"
//#include "GuiCode/ui/ZoomArea.hpp"
//#include "GuiCode/ui/Views/View.hpp"
//
//// --------------------------------------------------------------------------
//// ------------------------------ Track -------------------------------------
//// --------------------------------------------------------------------------
//
//class Track : public ComponentBase, public EventDistributor
//{
//public:
//	Track()
//		: ComponentBase(0, 0, 1, 100)
//	{}
//
//	void MousePressed(MouseEvent e)
//	{
//		LOG("eee");
//	}
//
//	void Draw() override;
//};
//
//// --------------------------------------------------------------------------
//// ------------------------ Arrangement View --------------------------------
//// --------------------------------------------------------------------------
//
//class Arrangement : public ViewBase
//{
//public:
//
//	Arrangement();
//
//	void MousePressed(MouseEvent) override;
//
//	void AddTrack();
//	void Draw() override;
//
//private:
//	double m_Unit = 32;
//
//	int m_CheckerBoardSize = 16;
//
//	bool m_RightClick = false,
//		m_ScrollDragging = false;
//
//	ZoomArea m_ZoomArea;
//
//	Button<ButtonGraphics::Normal<>> m_Btton{ []() {}, "BUttottnt", 100, 100 };
//
//	std::vector<std::unique_ptr<Track>> m_Tracks;
//	Menu<MenuGraphics::Vertical, MenuType::Normal> m_RightClickMenu;
//};
