#ifndef _EMU_MEMVIEW_H
#define _EMU_MEMVIEW_H

#include <wx/wx.h>

class EmuMemView : public wxFrame
{
	friend class EmuMainWin;
public:
	EmuMemView();
private:
	wxTextCtrl *view, *gotoAddr, *disasm;
};

#endif // _EMU_MEMVIEW_H
