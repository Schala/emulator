#include "memview.h"

EmuMemView::EmuMemView() : wxFrame(nullptr, wxID_ANY, "Memory View")
{
	wxStaticText *regs = new wxStaticText(this, wxID_ANY);
	regs->SetLabel(regsStr);

	// go to address in RAM
	gotoAddr = new wxTextCtrl(this, wxID_ANY);

	wxStaticText *gotoTxt = new wxStaticText(this, wxID_ANY, "Go to address:", wxDefaultPosition,
		wxDefaultSize, wxALIGN_LEFT);

	wxBoxSizer *gotoSizer = new wxBoxSizer(wxHORIZONTAL);
	gotoSizer->Add(gotoTxt, 1, wxSizerFlags().Expand().Left().Top(), 5);
	gotoSizer->Add(gotoAddr, 1, wxSizerFlags().Expand().Right().Top(), 5);

	// RAM view and disassembly
	wxSplitterWindow *split = new wxSplitterWindow(this, wxID_ANY);
	split->SetSashGravity(1.0); // top view always grows automatically

	view = new wxTextCtrl(split, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP);
	disasm = new wxTextCtrl(split, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP);

	wxBoxSizer *topLevelSizer = new wxBoxSizer(wxVERTICAL);
	topLevelSizer->Add(gotoSizer, 1, wxSizerFlags().Expand().Top(), 0);
	topLevelSizer->Add(split, 1, wxSizerFlags().Expand(), 0);

	SetSizerAndFit(topLevelSizer);
}
