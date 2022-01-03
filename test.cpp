#include <SDL2/SDL.h>
#include <wx/wx.h>
#include <iostream>

class TestFrame : public wxFrame
{
public:
	TestFrame() : wxFrame(nullptr, wxID_ANY, "Test")
	{
		wxMenu *fileMenu = new wxMenu();
		fileMenu->Append(wxID_OPEN);
		fileMenu->Append(wxID_EXIT);

		wxMenuBar *menuBar = new wxMenuBar();
		menuBar->Append(fileMenu, "&File");

		SetMenuBar(menuBar);
		CreateStatusBar();
		SetStatusText("Not running");

		win = SDL_CreateWindow("nope", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, 0);
		ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

		Bind(wxEVT_PAINT, &TestFrame::OnPaint, this, wxID_ANY);

		std::cerr << SDL_GetError() << std::endl;
	}

	~TestFrame()
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
	}

	void OnPaint(wxPaintEvent &)
	{
		SDL_Rect rect {5, 5, 300, 300};
		SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(ren, &rect);
		SDL_RenderPresent(ren);
	}

	void OnExit(wxCommandEvent &)
	{
		Close();
	}
private:
	SDL_Window *win;
	SDL_Renderer *ren;
};

class TestApp : public wxApp
{
public:
	bool OnInit()
	{
		TestFrame *frame = new TestFrame();
		frame->Show(true);
		return true;
	}

	int OnExit()
	{
		SDL_Quit();
		return 0;
	}
};

wxIMPLEMENT_APP_NO_MAIN(TestApp);

int main(int argc, char **argv)
{
	int initcode = SDL_Init(SDL_INIT_VIDEO);
	if (initcode == 0)
		return wxEntry(argc, argv);
	else
		return initcode;
}
