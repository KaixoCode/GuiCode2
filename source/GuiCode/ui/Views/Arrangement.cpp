//#include "GuiCode/ui/Views/Arrangement.hpp"
//#include "GuiCode/ui/RightClickMenu.hpp"
//
//// --------------------------------------------------------------------------
//// ------------------------------ Track -------------------------------------
//// --------------------------------------------------------------------------
//
//void Track::Draw() 
//{
//	Graphics::SetColor(Theme::Arrangement::track_background);
//	Graphics::Quad({ m_X + m_Width - 200, m_Y, 200, m_Height });
//
//	Graphics::SetColor(Theme::Arrangement::line);
//	Graphics::Quad({ m_X, m_Y - 2, m_Width, 2 });
//
//	Graphics::SetColor(Theme::Arrangement::line);
//	Graphics::LinedQuad({ m_X + m_Width - 200, m_Y - 2, 200, m_Height + 2 }, 2);
//
//};
//
//// --------------------------------------------------------------------------
//// ------------------------ Arrangement View --------------------------------
//// --------------------------------------------------------------------------
//
//Arrangement::Arrangement()
//{
//	using MenuButton = Button<ButtonGraphics::Menu, ButtonType::Normal>;
//	m_RightClickMenu.Emplace<MenuButton>([this]() { this->AddTrack(); }, "Add Track", 100, 20);
//	Display(m_RightClickMenu);
//	AddTrack();
//	AddTrack();
//	AddTrack();
//	AddTrack();
//}
//
//void Arrangement::Draw() 
//{
//	ViewBase::Draw();
//
//	// Zoom area
//	m_ZoomArea.SetSize(m_Width - 16, m_Height - 16);
//	m_ZoomArea.SetPosition(m_X + 8, m_Y + 8);
//	m_ZoomArea.StartZoomArea();
//	{
//		// Calculate the x position to start drawing at
//		int _startX = m_CheckerBoardSize * m_Unit * std::floor((m_ZoomArea.Translate().x / m_ZoomArea.Zoom()) / ((float)m_CheckerBoardSize * m_Unit));
//
//		// Calculate how many bars are visible on the screen, given the current zoom.
//		int _barAmount = 16 + std::ceil((m_Width / m_ZoomArea.Zoom()) / m_Unit / m_CheckerBoardSize) * m_CheckerBoardSize;
//
//		// Draw the checkerboard background
//		for (int i = 0; i < _barAmount; i += m_CheckerBoardSize / 2)
//		{
//			float _x = _startX + i * m_Unit;
//			float _w = m_CheckerBoardSize / 2 * m_Unit;
//
//			Graphics::SetColor(i % m_CheckerBoardSize == 0 ? Theme::Arrangement::background_light : Theme::Arrangement::background_dark);
//			Graphics::Quad({ _x, -m_ZoomArea.Translate().y, _w, m_Height });
//		}
//
//		// Display bars at certain intervals depending on the zoom level.
//		float _barInterval = 1.0 / pow(2, ceil(log(m_ZoomArea.Zoom()) / log(2)));
//		int _maxZoomDepth = 64;
//		float _stroke = min(std::pow(m_ZoomArea.Zoom(), 0.25), 1.0) * 0.5;
//		for (int i = 0; i < _barAmount * _maxZoomDepth; i += _barInterval * _maxZoomDepth)
//		{
//			float _x = _startX + (i / (float)_maxZoomDepth) * m_Unit;
//
//			// Some optimizing to make sure we only draw lines on screen
//			if (_x * m_ZoomArea.Zoom() - m_ZoomArea.Translate().x > Width())
//				break;
//
//			if (_x - m_ZoomArea.Translate().x / m_ZoomArea.Zoom() < 0)
//				continue;
//
//			// Calculate the stroke width
//			float _cStroke = _stroke / m_ZoomArea.Zoom();
//			if (i % (2 * _maxZoomDepth) == 0)
//				_cStroke *= 4;
//
//			else if (i % (_maxZoomDepth / 2) == 0)
//				_cStroke *= 2.5;
//
//			Graphics::SetColor(Theme::Arrangement::line);
//			Graphics::Quad({ _x, -m_ZoomArea.Translate().y, _cStroke, m_Height });
//		}
//
//		// Test rectangle
//		Graphics::SetColor({ 1, 0, 0, 1 });
//		Graphics::Quad({ 900, 20, 100, 100 });
//
//		//Graphics::SetColor({ 1, 0, 0, 1 });
//		//Graphics::Quad({ m_ZoomArea.Translate(), 20, 100, 100 });
//
//		// Display all tracks (on the ZoomArea)
//		int _x = m_ZoomArea.Translate().x / m_ZoomArea.Zoom(), _y = Height();
//		for (auto& _t : m_Tracks)
//		{
//			_y -= _t->Height();
//			_t->SetSize(Width() / m_ZoomArea.Zoom(), _t->Height());
//			m_ZoomArea.Display(*_t, _x, _y);
//		}
//
//		// Test button
//		m_ZoomArea.Display(m_Btton, 100, 100);
//	}
//	m_ZoomArea.FinishZoomArea(this);
//
//	if (m_RightClick || m_RightClickMenu.active && Graphics::WindowFocused())
//	{
//		SetFocus(&m_RightClickMenu);
//		Display(m_RightClickMenu);
//		m_RightClick = false;
//	}
//
//	// Outline
//	int _padding = 8;
//	Graphics::SetColor({ 0.05, 0.05, 0.05, 1.00 });
//	Graphics::LinedQuad({ m_X + _padding, m_Y + _padding, m_Width - _padding * 2, m_Height - _padding * 2 }, 3);
//
//
//	//if (!Graphics::WindowFocused() || !m_RightClickMenu.active)
//	//	m_RightClick = false;
//}
//
//void Arrangement::AddTrack()
//{
//	auto& _track = m_Tracks.emplace_back(std::make_unique<Track>());
//	// TODO: Also add track to audio part of application
//}
//
//void Arrangement::MousePressed(MouseEvent event)
//{
//	if (event.button == MouseEvent::RIGHT)
//	{
//		/*m_RightClickMenu.SetPosition(event.x, event.y - m_RightClickMenu.Height());
//		m_RightClick = true;*/
//
//		//RightClickMenu::Open(*this);
//	}
//}