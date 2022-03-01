#include <cctype>
#include <charconv>
#include <cstring>
#include <map>
#include <system_error>

#include "lexer.h"

static const std::map<const char *, Token6500ID> Ops =
{
	{ "ADC", Token6500ID::ADC },
	{ "AND", Token6500ID::AND },
	{ "ASL", Token6500ID::ASL },
	{ "BCC", Token6500ID::BCC },
	{ "BCS", Token6500ID::BCS },
	{ "BEQ", Token6500ID::BEQ },
	{ "BIT", Token6500ID::BIT },
	{ "BMI", Token6500ID::BMI },
	{ "BNE", Token6500ID::BNE },
	{ "BPL", Token6500ID::BPL },
	{ "BRK", Token6500ID::BRK },
	{ "BVC", Token6500ID::BVC },
	{ "BVS", Token6500ID::BVS },
	{ "CLC", Token6500ID::CLC },
	{ "CLD", Token6500ID::CLD },
	{ "CLI", Token6500ID::CLI },
	{ "CLV", Token6500ID::CLV },
	{ "CMP", Token6500ID::CMP },
	{ "CPX", Token6500ID::CPX },
	{ "CPY", Token6500ID::CPY },
	{ "DEC", Token6500ID::DEC },
	{ "DEX", Token6500ID::DEX },
	{ "DEY", Token6500ID::DEY },
	{ "EOR", Token6500ID::EOR },
	{ "INC", Token6500ID::INC },
	{ "INX", Token6500ID::INX },
	{ "INY", Token6500ID::INY },
	{ "JMP", Token6500ID::JMP },
	{ "JSR", Token6500ID::JSR },
	{ "LDA", Token6500ID::LDA },
	{ "LDX", Token6500ID::LDX },
	{ "LDY", Token6500ID::LDY },
	{ "LSR", Token6500ID::LSR },
	{ "NOP", Token6500ID::NOP },
	{ "ORA", Token6500ID::ORA },
	{ "PHA", Token6500ID::PHA },
	{ "PHP", Token6500ID::PHP },
	{ "PLA", Token6500ID::PLA },
	{ "PLP", Token6500ID::PLP },
	{ "ROL", Token6500ID::ROL },
	{ "ROR", Token6500ID::ROR },
	{ "RTI", Token6500ID::RTI },
	{ "RTS", Token6500ID::RTS },
	{ "SBC", Token6500ID::SBC },
	{ "SEC", Token6500ID::SEC },
	{ "SED", Token6500ID::SED },
	{ "SEI", Token6500ID::SEI },
	{ "STA", Token6500ID::STA },
	{ "STX", Token6500ID::STX },
	{ "STY", Token6500ID::STY },
	{ "TAX", Token6500ID::TAX },
	{ "TAY", Token6500ID::TAY },
	{ "TSX", Token6500ID::TSX },
	{ "TXA", Token6500ID::TXA },
	{ "TXS", Token6500ID::TXS },
	{ "TYA", Token6500ID::TYA }
};

static const char *TokenStrings[] {
	"end of input",
	"ERROR",

	":",
	",",
	"*=",
	"(",
	")",
	"#",

	"identifier",
	"integer",
	"string",

	"BYTE",
	"ASCIIZ",

	"X",
	"Y",

	"ADC",
	"AND",
	"ASL",
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
	"DEC",
	"DEX",
	"DEY",
	"EOR",
	"INC",
	"INX",
	"INY",
	"JMP",
	"JSR",
	"LDA",
	"LDX",
	"LDY",
	"LSR",
	"NOP",
	"ORA",
	"PHA",
	"PHP",
	"PLA",
	"PLP",
	"RLA",
	"ROL",
	"ROR",
	"RTI",
	"RTS",
	"SBC",
	"SEC",
	"SED",
	"SEI",
	"STA",
	"STX",
	"STY",
	"TAX",
	"TAY",
	"TSX",
	"TXA",
	"TXS",
	"TYA"
};

Lexer6500::Lexer6500(const char *s):
	m_lastState(s),
	m_state(s)
{
}

Token6500 Lexer6500::Binary()
{
	m_lastState.Advance(); // eat '%'

	while (m_state.IsBinary())
		m_state.Next();

	if (std::isalnum(m_state.Get()) || m_state.Get() == '_')
		return Error("Binary literal holds invalid characters");

	uint16_t n;
	auto result = std::from_chars(m_lastState.Ptr(), m_state.Ptr(), n, 2);

	if (result.ec == std::errc::result_out_of_range)
		return Error("Max value exceeded");

	return MakeToken(Token6500ID::IntegerLiteral, n);
}

Token6500 Lexer6500::Decimal()
{
	while (std::isdigit(m_state.Next())) ;

	if (std::isalpha(m_state.Get()) || m_state.Get() == '_')
		return Error("Decimal literal holds invalid characters");

	uint16_t n;
	auto result = std::from_chars(m_lastState.Ptr(), m_state.Ptr(), n);

	if (result.ec == std::errc::result_out_of_range)
		return Error("Max value exceeded");

	return MakeToken(Token6500ID::IntegerLiteral, n);
}

Token6500 Lexer6500::Expect(char expected, Token6500ID id)
{
	if (m_state.Get() == expected) return Simple(id);
	else
		return Error("Expected '", expected, "', got '", m_state.Get(), '\'');
}

Token6500 Lexer6500::Follow(char expected, Token6500ID ifYes, Token6500ID ifNo)
{
	if (m_state.Next() == expected) return Simple(ifYes);
	else return Simple(ifNo);
}

bool Lexer6500::HasMore() const
{
	return m_state.HasMore();
}

Token6500 Lexer6500::Hex()
{
	m_lastState.Advance(); // eat '$'

	while (std::isxdigit(m_state.Next())) ;

	if (std::isalpha(m_state.Get()) || m_state.Get() == '_')
		return Error("Hexadecimal literal holds invalid characters");

	uint16_t n;
	auto result = std::from_chars(m_lastState.Ptr(), m_state.Ptr(), n, 16);

	if (result.ec == std::errc::result_out_of_range)
		return Error("Max value exceeded");

	return MakeToken(Token6500ID::IntegerLiteral, n);
}

Token6500 Lexer6500::Identifier(bool prefixed)
{
	std::string ident(1, prefixed ? m_state.Next() : std::toupper(m_state.Get()));

	while (Valid6500Identifier(m_state.Next()))
		ident += std::toupper(m_state.Get());

	for (auto &[k, v] : Ops)
		if (std::strcmp(ident.c_str(), k) == 0)
			return Simple(v);

	// meta commands
	if (ident == "BY" || ident == "BYTE" || ident == "DB")
		return Simple(Token6500ID::Byte);
	if (ident == "AZ" || ident == "ASCIIZ" || ident == "TX")
		return Simple(Token6500ID::Text);

	// registers
	if (ident == "X") return Simple(Token6500ID::X);
	if (ident == "Y") return Simple(Token6500ID::Y);

	return MakeToken(Token6500ID::Identifier, std::move(ident));
}

void Lexer6500::LineComment()
{
	while (!m_state.IsNewLine())
		m_state.Advance();
}

Token6500 Lexer6500::MakeToken(Token6500ID id, Token6500Value value) const
{
	return { id, m_state.Line(), m_state.Column(), m_state.Line(), std::move(value) };
}

Token6500 Lexer6500::NextToken()
{
	m_state.SkipWhitespace();

	m_lastState = m_state;
	char c = std::toupper(m_state.Get());

	switch (c)
	{
		case 0: return MakeToken(Token6500ID::EndOfInput);
		case '%': return Binary();
		case ':': return Simple(Token6500ID::Colon);
		case ',': return Simple(Token6500ID::Comma);
		case '$': return Hex();
		case '.': return Identifier(true);
		case ';': LineComment(); return NextToken();
		case '(': return Simple(Token6500ID::ParenthesisLeft);
		case ')': return Simple(Token6500ID::ParenthesisRight);
		case '"': return String();
		case '#': return Simple(Token6500ID::Value);
		default:
			if (std::isalpha(c) || c == '_') return Identifier(false);
			if (std::isdigit(c)) return Decimal();
			return Error("Unexpected character: '", c, '\'');
	}
}

Token6500 Lexer6500::Offset()
{
	if (m_state.Next() == '=')
		return Simple(Token6500ID::Offset);
	return Error("Expected '=' for offset, got '", m_state.Get(), '\'');
}

std::string SanitizeString(std::string text)
{
	for (size_t i = 0; i < text.size(); i++)
		switch (text[i])
		{
			case '\n':
				text.insert(i, "\\n");
				break;
			case '\t':
				text.insert(i, "\\t");
				break;
			case '\\':
				text.insert(i, "\\\\");
				break;
			default:
				if (std::iscntrl(text[i]))
				{
					text.insert(i, "\\x");
					i += 2;
					text.insert(i++, 1, (text[i] & 240) >> 4);
					text.insert(i++, 1, text[i] & 15);
				}
		}

	return std::move(text);
}

Token6500 Lexer6500::Simple(Token6500ID id)
{
	m_state.Advance();
	return MakeToken(id);
}

Token6500 Lexer6500::String()
{
	std::string text = "";

	while (m_state.Next() != '"')
		text += m_state.Get();
	m_state.Advance();

	return MakeToken(Token6500ID::StringLiteral, text);
}

const char * Token6500IDString(Token6500ID id)
{
	return TokenStrings[static_cast<size_t>(id)];
}

constexpr bool Valid6500Identifier(char c)
{
	return std::isalnum(c) || c == '_';
}
