#include <algorithm>
#include <cctype>
#include <cstdlib>

#include "../core/utility.h"
#include "assembler.h"

static const std::vector<const char *> Ops =
{
	"a11", "shx", "sxa", "xas",
	"aax", "axs", "sax",
	"adc",
	"ahx", "axa", "sha",
	"alr", "asr",
	"anc", "anc2",
	"and",
	"ane", "xaa",
	"arr",
	"asl",
	"aso", "slo",
	"bcc",
	"bcs",
	"beq",
	"bit",
	"bmi",
	"bne",
	"bpl",
	"brk",
	"bvc",
	"bvs",
	"clc",
	"cld",
	"cli",
	"clv",
	"cmp",
	"cpx",
	"cpy",
	"dcm", "dcp",
	"dec",
	"dex",
	"dey",
	"dop", "nop", "top",
	"eor",
	"hlt", "jam", "kil",
	"inc",
	"ins", "isb", "isc",
	"inx",
	"iny",
	"jmp",
	"jsr",
	"lar", "las",
	"lax",
	"lda",
	"ldx",
	"ldy",
	"lse", "sre",
	"lsr",
	"lxa",
	"ora",
	"pha",
	"php",
	"pla",
	"plp",
	"rla",
	"rol",
	"ror",
	"rra",
	"rti",
	"rts",
	"say", "shy", "sya",
	"sbc",
	"sbx",
	"sec",
	"sed",
	"sei",
	"shs", "tas",
	"sta",
	"stx",
	"sty",
	"tax",
	"tay",
	"tsx",
	"txa",
	"txs",
	"tya",
	"usbc"
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
