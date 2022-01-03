#include "app.h"

wxIMPLEMENT_APP(EmuApp);

bool EmuApp::OnInit()
{
	EmuMainWin *mainWin = new EmuMainWin();
	mainWin->Show(true);
	return true;
}
