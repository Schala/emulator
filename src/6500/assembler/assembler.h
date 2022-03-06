#ifndef _6500_ASSEMBLER_H
#define _6500_ASSEMBLER_H

#include <cstdint>
#include <filesystem>
#include <map>
#include <vector>

#include "../../core/scanner.h"

class Assembler6500
{
public:
	Assembler6500(std::istream &);
	void Compile();
	std::string GetText() const;
	void Preprocess();

	std::map<std::string, uint16_t> m_labels;
private:
	Scanner m_state;
	std::vector<uint8_t> m_code;
	std::map<uint16_t, uint16_t> m_jumps;
	std::map<uint16_t, std::vector<uint8_t>> m_bytes;
	std::map<uint16_t, std::vector<uint16_t>> m_words;
	std::map<uint16_t, std::string> m_strings;
	std::map<uint16_t, std::filesystem::path> m_includes;

	void ParseDirectives();
	void ParseLabels();
	void StripComments();
};

#endif // _6500_ASSEMBLER_H
