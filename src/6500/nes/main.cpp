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

	NES nes(nullptr);
	nes.LoadROM(argv[1]);

	std::array<char, 2> input;
	while (true)
	{
		std::cin.getline(&input[0], 2, '\n');
		switch (std::toupper(input[0]))
		{
			case 'Q':
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
