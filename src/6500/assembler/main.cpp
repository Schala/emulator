#include <iostream>
#include <type_traits>
#include <vector>

#include "assembler.h"

int main(int argc, char **argv)
{
	if (argc != 2) return 1;

	Assembler6502State state;
	state.OpenSourceFile(argv[1]);

	size_t num = 1;

	//std::vector<std::vector<Token6502>> lineToks;
	//std::vector<std::vector<std::string_view>> lineIds;

	/*while (!state.EndOfFile())
		lineToks.push_back(state.LineTokens());
	state.Reset();*/
	while (!state.EndOfFile())
	{
		auto li = state.LineIdentifiers();

		std::cout << num++ << ": ";

		for (auto &l : li)
			std::cout << l << ' ';
		std::cout << '\n';
	}
		//lineIds.push_back(state.LineIdentifiers());

	std::cout << "wee" << '\n';
	/*for (int i = 1; i <= lineToks.size(); i++)
	{
		std::cout << i << ": ";

		for (auto &t : lineToks[i-1])
		{
			auto id = lineIds[i-1].begin();
			std::cout << static_cast<std::underlying_type<Token6502>::type>(t);
			if (t == Token6502::Identifier)
				std::cout << '(' << *(id++) << ')';
			std::cout << ' ';
		}

		std::cout << '\n';
	}*/

	return 0;
}
