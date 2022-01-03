#include "main_win.h"

wxBEGIN_EVENT_TABLE(EmuMainWin, wxFrame)
	EVT_MENU(wxID_ABOUT, EmuMainWin::OnAbout)
	EVT_MENU(wxID_EXIT, EmuMainWin::OnExit)
	EVT_MENU(ID_MODE_6502, EmuMainWin::OnMode6502)
	EVT_MENU(ID_MEM_VIEW, EmuMainWin::OnMemoryView)
wxEND_EVENT_TABLE()

EmuMainWin::EmuMainWin() : wxFrame(nullptr, wxID_ANY, "Emulator")
{
	wxMenu *fileMenu = new wxMenu();
	fileMenu->Append(wxID_OPEN);
	fileMenu->Append(wxID_EXIT);

	wxMenu *modeMenu = new wxMenu();
	modeMenu->Append(ID_MODE_6502, "MOS 6502", "Set the emulation mode to 6502");

	wxMenu *emuMenu = new wxMenu();
	emuMenu->AppendSubMenu(modeMenu, "Mode", "Emulation modes");

	wxMenu *toolsMenu = new wxMenu();
	toolsMenu->Append(ID_MEM_VIEW, "Memory View", "Displays the contents in emulator RAM");

	wxMenu *helpMenu = new wxMenu();
	helpMenu->Append(wxID_INFO);
	helpMenu->Append(wxID_ABOUT);

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(emuMenu, "Emulation");
	menuBar->Append(toolsMenu, "Tools");
	menuBar->Append(helpMenu, "&Help");

	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Not running");

	memView = new EmuMemView();
	screen = new RenderCtrl(this);
}

EmuMainWin::~EmuMainWin()
{
	Deinit6502();
}

void EmuMainWin::OnAbout(wxCommandEvent &)
{
	wxMessageBox("Test emulator's GUI", "About this emulator", wxOK);
}

void EmuMainWin::OnExit(wxCommandEvent &)
{
	Close(true);
}

void EmuMainWin::OnMemoryView(wxCommandEvent &)
{
	memView->Show(true);
}

void EmuMainWin::OnMode6502(wxCommandEvent &)
{
	// barebones for now

	bus = bus6502_alloc(0xFFFF);
	cpu = cpu6502_alloc(bus, 0x6000, 0x7FFF);
}

void EmuMainWin::Deinit6502()
{
	if (cpu) cpu6502_free(cpu);
	if (bus) bus6502_free(bus);
}
