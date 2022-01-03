#ifndef _EMU_MAIN_WIN_H
#define _EMU_MAIN_WIN_H

#include <wx/wx.h>

#include "../6502/devices.h"
#include "memview.h"
#include "renderctrl.h"

enum
{
	ID_MEM_VIEW = 1
	ID_MODE_6502,
};

class EmuMainWin : public wxFrame
{
public:
	EmuMainWin();
	~EmuMainWin();
private:
	DECLARE_EVENT_TABLE()

	EmuMemView *memView;
	RenderCtrl *screen;

	BUS_6502 *bus;
	CPU_6502 *cpu;

	wxString ram, disasm, regs;

	void OnExit(wxCommandEvent &);
	void OnAbout(wxCommandEvent &);
	void OnMemoryView(wxCommandEvent &);
	void OnMode6502(wxCommandEvent &);

	void Deinit6502();
};

#endif // _EMU_MAIN_WIN_H
