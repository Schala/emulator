#include "renderctrl.h"

wxBEGIN_EVENT_TABLE(RenderCtrl, wxControl)
	EVT_IDLE(wxID_ANY, RenderCtrl::OnIdle)
	EVT_PAINT(wxID_ANY, RenderCtrl::OnPaint)
	EVT_ERASE_BACKGROUND(wxID_ANY, RenderCtrl::OnEraseBackground)
	EVT_SIZE(wxID_ANY, RenderCtrl::OnSize)
wxEND_EVENT_TABLE()

RenderCtrl::RenderCtrl(wxWindow *parent, wxWindowID id, const wxPoint &position, const wxSize &size,
	long style) : wxControl(parent, id, position, size, style)
{
	sf::RenderWindow::create(GetHandle());
}

RenderCtrl::~RenderCtrl()
{
	close();
}

void RenderCtrl::OnUpdate()
{
}

void RenderCtrl::OnIdle(wxIdleEvent &)
{
	Refresh();
}

void RenderCtrl::OnPaint(wxPaintEvent &)
{
	wxPaintDC dc(this);
	OnUpdate();
	display();
}

void RenderCtrl::OnEraseBackground(wxEraseEvent &)
{
}

void RenderCtrl::OnSize(wxSizeEvent &)
{
}
