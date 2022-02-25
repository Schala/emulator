#include <fstream>
#include <iomanip>
#include <iostream>

#include "lexer.h"

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

	Lexer6500 lex(src.c_str());

	while (lex.HasMore())
	{
		Token6500 t = lex.NextToken();

		std::cout << std::setw(4) << t.Line << ", " << std::setw(3) <<
			t.Column << ":\t" << TokenIDString(t.ID);

		switch (t.ID)
		{
			case Token6500ID::Error:
			case Token6500ID::Identifier:
			case Token6500ID::String:
				std::cout << '(' << std::get<std::string>(t.Value) << ')';
				break;
			case Token6500ID::Integer:
				std::cout << '(' << std::get<uint16_t>(t.Value) << ')';
				break;
			default: ;
		}

		std::cout << '\n';
	}

	return 0;
}
