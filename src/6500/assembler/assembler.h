#ifndef _6500_ASSEMBLER_H
#define _6500_ASSEMBLER_H

#include <cstdint>
#include <map>

#include "../../core/scanner.h"

class Assembler6500
{
public:
	Assembler6500(std::istream &);
	void Compile();
	std::string GetText() const;
	void Preprocess();
private:
	std::string::iterator m_lastCol;
	std::vector<std::string>::iterator m_lastLine;
	Scanner m_state;
	std::vector<uint8_t> m_code;
	std::map<std::string, uint16_t> m_labels;

	void ParseDirectives();
	void ParseLabels();
	void StripComments();
};

#endif // _6500_ASSEMBLER_H
