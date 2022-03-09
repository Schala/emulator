#include <array>
#include <cctype>
#include <cstdlib>
#include <iostream>

#include "cheater.h"
#include "nes.h"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Specify a ROM\n";
		std::exit(EXIT_FAILURE);
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialise video: %s", SDL_GetError());
		std::exit(EXIT_FAILURE);
	}

	SDL_Renderer *renderer;
	SDL_Window *window;

	if (SDL_CreateWindowAndRenderer(100, 100, SDL_WINDOW_RESIZABLE, &window, &renderer))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialise window or renderer: %s", SDL_GetError());
		std::exit(EXIT_FAILURE);
	}

	NES nes(renderer);
	nes.LoadROM(argv[1]);
	int npal = 0;

	std::array<char, 2> input;
	while (true)
	{
		std::cin.getline(&input[0], 2, '\n');
		switch (std::toupper(input[0]))
		{
			case 'P':
				(++npal) %= 8;
				nes.GetPPU()->GetPatternTable(0, npal).Draw(renderer, 10, 10);
				nes.GetPPU()->GetPatternTable(1, npal).Draw(renderer, 10, 50);
				SDL_RenderPresent(renderer);
				break;
			case 'Q':
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				std::exit(EXIT_SUCCESS);
				break;
			default:
				do nes.GetCPU()->Clock();
				while (nes.GetCPU()->Cycles() > 0);
				std::cout << nes.GetCPU()->FrameInfo() << '\n';
		}
	}

	return 0;
}
