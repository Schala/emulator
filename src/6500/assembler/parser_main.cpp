#include <fstream>
#include <iomanip>
#include <iostream>

#include "assembler.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "No file specified\n";
		return 1;
	}

	std::ifstream srcFile(argv[1]);
	srcFile.seekg(0, std::ios::end);
	size_t srcSize = srcFile.tellg();
	srcFile.seekg(0);
	std::string src(srcSize, 0);
	srcFile.read(&src[0], srcSize);

	Assembler6500 parser(src.c_str());
	parser.Parse();

	for (auto &t : parser.m_ast)
	{
		std::cout << std::setw(4) << t.Line << ", " << std::setw(3) <<
			t.Column << ":\t" << Token6500IDString(t.ID);

		switch (t.ID)
		{
			case Token6500ID::Error:
			case Token6500ID::Identifier:
			case Token6500ID::StringLiteral:
				std::cout << '(' << std::get<std::string>(t.Value) << ')';
				break;
			case Token6500ID::IntegerLiteral:
				std::cout << '(' << std::get<uint16_t>(t.Value) << ')';
				break;
			default: ;
		}

		std::cout << '\n';
	}

	return 0;
}
