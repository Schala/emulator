#include <cctype>
#include <cstring>
#include <regex>

#include "assembler.h"
#include "../../core/utility.h"

static const std::vector<const char *> RESERVED =
{
	// instructions
	"ADC", "AND", "ASL",
	"BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL", "BRK", "BVC", "BVS",
	"CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY",
	"DEC", "DEX", "DEY",
	"EOR",
	"INC", "INX", "INY",
	"JMP", "JSR",
	"LDA", "LDX", "LDY", "LSR",
	"NOP",
	"ORA",
	"PHA", "PHP", "PLA", "PLP",
	"ROL", "ROR", "RTI", "RTS",
	"SBC", "SEC", "SED", "SEI", "STA", "STX", "STY",
	"TAX", "TAY", "TSX", "TXA", "TXS", "TYA",

	// registers
	"A", "X", "Y",

	// directives
	"BY", "BYT", "BYTE", "DB",
	"ASCII", "ASCIIZ", "AZ", "TX",
	"INCLUDE",
	"ORG",
	"WORD"
};

static constexpr bool ValidIdentifier(char c)
{
	return std::isalnum(c) || c == '_';
}

Assembler6500::Assembler6500(std::istream &s):
	m_state(s)
{
}

std::string Assembler6500::GetText() const
{
	return m_state.GetAll();
}

void Assembler6500::ParseDirectives()
{

}

void Assembler6500::ParseLabels()
{
	Scanner newState;
	const std::regex pattern("^\\.?([A-Za-z_][A-Za-z0-9_]*)\\:?");
	size_t offset = 0;

	// collect labels with their offsets
	for (size_t i = 0; i < m_state.LineCount(); i++)
	{
		std::string line = *(m_state.LineIterator());
		std::smatch matches;

		if (std::regex_match(line, matches, pattern))
		{
			for (const char *s : RESERVED)
				if (matches[1] == s)
				{
					newState.AddLine(line);
					offset++;
					m_state.NextLine();
					continue;
				}

			m_labels[matches[1]] = static_cast<uint16_t>(offset);
			m_state.NextLine();
		}
		else
		{
			newState.AddLine(line);
			offset++;
			m_state.NextLine();
		}
	}

	newState.Rewind();
	m_state = std::move(newState);

	// Replace label references with offsets
	for (size_t i = 0; i < m_state.LineCount(); i++)
	{
		std::string line = *(m_state.LineIterator());

		for (auto &[id, offset] : m_labels)
			line = std::regex_replace(line, std::regex(id), std::to_string(offset));

		newState.AddLine(line);
		m_state.NextLine();
	}

	newState.Rewind();
	m_state = std::move(newState);
}

void Assembler6500::Preprocess()
{
	StripComments();
	ParseLabels();
	ParseDirectives();
}

void Assembler6500::StripComments()
{
	Scanner newState;

	for (size_t i = 0; i < m_state.LineCount(); i++)
	{
		std::string line = *(m_state.LineIterator());

		size_t pos = line.find_first_of(';');
		if (pos != std::string::npos)
			line = line.substr(0, pos);

		// if whole line was not a comment, empty, or whitespace
		if (line.empty() || IsBlankLine(line))
			m_state.NextLine();
		else
		{
			newState.AddLine(line);
			m_state.NextLine();
		}
	}

	newState.Rewind();
	m_state = std::move(newState);
}
