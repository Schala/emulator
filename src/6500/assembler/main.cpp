#include <fstream>
#include <iomanip>
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

	for (auto &[id, offs] : as.m_labels)
		std::cout << std::setw(4) << std::hex << std::uppercase << offs << ": " << id << '\n';

	return 0;
}
