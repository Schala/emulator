#include <algorithm>
#include <cctype>
#include <cstdlib>

#include "../core/utility.h"
#include "assembler.h"

static const std::vector<const char *> Ops =
{
	"A11", "SHX", "SXA", "XAS",
	"AAX", "AXS", "SAX",
	"ADC",
	"AHX", "AXA", "SHA",
	"ALR", "ASR",
	"ANC", "ANC2",
	"AND",
	"ANE", "XAA",
	"ARR",
	"ASL",
	"ASO", "SLO",
	"BCC",
	"BCS",
	"BEQ",
	"BIT",
	"BMI",
	"BNE",
	"BPL",
	"BRK",
	"BVC",
	"BVS",
	"CLC",
	"CLD",
	"CLI",
	"CLV",
	"CMP",
	"CPX",
	"CPY",
	"DCM", "DCP",
	"DEC",
	"DEX",
	"DEY",
	"DOP", "NOP", "TOP",
	"EOR",
	"HLT", "JAM", "KIL",
	"INC",
	"INS", "ISB", "ISC",
	"INX",
	"INY",
	"JMP",
	"JSR",
	"LAR", "LAS",
	"LAX",
	"LDA",
	"LDX",
	"LDY",
	"LSE", "SRE",
	"LSR",
	"LXA",
	"ORA",
	"PHA",
	"PHP",
	"PLA",
	"PLP",
	"RLA",
	"ROL",
	"ROR",
	"RRA",
	"RTI",
	"RTS",
	"SAY", "SHY", "SYA",
	"SBC",
	"SBX",
	"SEC",
	"SED",
	"SEI",
	"SHS", "TAS",
	"STA",
	"STX",
	"STY",
	"TAX",
	"TAY",
	"TSX",
	"TXA",
	"TXS",
	"TYA",
	"USBC"
};

Assembler6502State::Assembler6502State():
	m_lastToken(Token6502::Invalid),
	m_lastChar(' '),
	m_val(0)
{
}


Assembler6502State::~Assembler6502State()
{
	if (m_src.is_open()) m_src.close();
}


Token6502 Assembler6502State::NextToken()
{
	while (std::isspace(m_lastChar))
		m_lastChar = std::toupper(m_src.get());

	// line comment
	if (m_lastChar == ';')
		while (!IsNewLine(m_lastChar))
			m_lastChar = std::toupper(m_src.get());

	// identifier, beginning with a letter or _
	if (std::isalpha(m_lastChar) || m_lastChar == '_')
	{
		m_lastIdentifier = m_lastChar;
		m_lastChar = std::toupper(m_src.get());

		while (std::isalnum(m_lastChar) || m_lastChar == '_')
		{
			m_lastIdentifier += m_lastChar;
			m_lastChar = std::toupper(m_src.get());
		}

		// operation?
		if (std::find(Ops.begin(), Ops.end(), m_lastIdentifier.c_str()) != Ops.end())
			return Token6502::Operation;

		// label?
		if (m_src.peek() == ':')
		{
			m_lastChar = m_src.get();
			m_labels.push_back(std::string_view(m_lastIdentifier));
			return Token6502::Label;
		}

		// register?
		if (m_lastIdentifier == "X") return Token6502::X;
		if (m_lastIdentifier == "Y") return Token6502::Y;

		// something else
		return Token6502::Identifier;
	}

	// binary
	if (m_lastChar == '%')
	{
		m_lastIdentifier = "";

		while (IsBinary(m_lastChar = std::toupper(m_src.get())))
			m_lastIdentifier += m_lastChar;

		if (m_lastIdentifier.empty())
			return Token6502::Invalid;
		else
		{
			m_val = static_cast<uint16_t>(std::strtoul(m_lastIdentifier.c_str(), nullptr, 2);
			return Token6502::Binary;
		}
	}

	// decimal
	if (m_flags.dec)
		if (std::isdigit(m_lastChar))
		{
			m_lastIdentifier = m_lastChar;

			while (std::isdigit(m_lastChar = std::toupper(m_src.get())))
				m_lastIdentifier += m_lastChar;

			m_val = static_cast<uint16_t>(std::strtoul(m_lastIdentifier.c_str(), nullptr, 10);
			return Token6502::Decimal;
		}

	// hex
	if (std::isxdigit(m_lastChar))
	{
		m_lastIdentifier = m_lastChar;

		while (std::isxdigit(m_lastChar = std::toupper(m_src.get())))
			m_lastIdentifier += m_lastChar;

		m_val = static_cast<uint16_t>(std::strtoul(m_lastIdentifier.c_str(), nullptr, 16);
		return Token6502::Hex;
	}

	if (m_src.eof())
		return Token6502::EndOfFile;

	return Token6502::Invalid;
}


void OpenSourceFile(const std::filesystem::path &path)
{
	if (m_src.is_open()) m_src.close();
	m_src.open(path);
}
