#include <fstream>
#include <iostream>

#include "assembler.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Specify an assembly file\n";
		return 1;
	}

	std::ifstream srcFile(argv[1]);
	Assembler6500 as(srcFile);
	as.Preprocess();

	std::ofstream outFile("preprocessed.s");
	outFile << as.GetText();
	return 0;
}
