#ifndef _EMU_RENDERCTRL_H
#define _EMU_RENDERCTRL_H

#include <SFML/Graphics.hpp>
#include <wx/wx.h>

class RenderCtrl : public wxControl, public sf::RenderWindow
{
public:
	RenderCtrl(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &position = wxDefaultPosition,
		const wxSize &size = wxDefaultSize, long style = 0);
	~RenderCtrl();
private:
	DECLARE_EVENT_TABLE()

	void OnUpdate();
	void OnIdle(wxIdleEvent &);
	void OnPaint(wxPaintEvent &);
	void OnEraseBackground(wxEraseEvent &);
	void OnSize(wxSizeEvent &);
};

#endif // _EMU_RENDERCTRL_H
