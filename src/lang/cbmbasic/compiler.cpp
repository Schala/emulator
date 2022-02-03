#include <cctype>
#include <cstdlib>

#include "../core/utility.h"
#include "cbm_basic.h"

CBMBASICState::CBMBASICState():
	m_lastToken(CBMBASICToken::Invalid),
	m_lastChar(' '),
	m_val(0)
{
}


CBMBASICToken CBMBASICState::NextToken()
{
	while (std::isspace(m_lastChar))
		m_lastChar = std::toupper(*(++m_it));

	// identifier, beginning with a letter
	if (std::isalpha(m_lastChar))
	{
		m_lastIdentifier = m_lastChar;

		while (std::isalnum(m_lastChar = std::toupper(*(++m_it))))
			m_lastIdentifier += m_lastChar;

		// line comment?
		if (m_lastIdentifier == "REM")
		{
			while (!IsNewLine(m_lastChar))
				m_lastChar = std::toupper(*(++m_it));
			return NextToken();
		}

		if (m_lastIdentifier == "AND") return CBMBASICToken::And;
		if (m_lastIdentifier == "DEF") return CBMBASICToken::Def;
		if (m_lastIdentifier == "DIM") return CBMBASICToken::Dim;
		if (m_lastIdentifier == "ELSE") return CBMBASICToken::Else;
		if (m_lastIdentifier == "FN") return CBMBASICToken::Fn;
		if (m_lastIdentifier == "FOR") return CBMBASICToken::For;
		if (m_lastIdentifier == "GOSUB") return CBMBASICToken::GoSub;
		if (m_lastIdentifier == "GOTO") return CBMBASICToken::GoTo;
		if (m_lastIdentifier == "IF") return CBMBASICToken::If;
		if (m_lastIdentifier == "LET") return CBMBASICToken::Let;
		if (m_lastIdentifier == "NEXT") return CBMBASICToken::Next;
		if (m_lastIdentifier == "NOT") return CBMBASICToken::Not;
		if (m_lastIdentifier == "ON") return CBMBASICToken::On;
		if (m_lastIdentifier == "OR") return CBMBASICToken::Or;
		if (m_lastIdentifier == "RETURN") return CBMBASICToken::Return;
		if (m_lastIdentifier == "STEP") return CBMBASICToken::Step;
		if (m_lastIdentifier == "THEN") return CBMBASICToken::Then;
		if (m_lastIdentifier == "TO") return CBMBASICToken::To;

		// could be a variable or function
		return CBMBASICToken::Identifier;
	}

	// string
	if (m_lastChar == '\"')
	{
		m_lastIdentifier = "";

		while ((m_lastChar = *(++m_it)) != '\"')
			m_lastIdentifier += m_lastChar;

		return CBMBASICToken::String;
	}

	// number
	if (std::isdigit(m_lastChar))
	{
		m_lastIdentifier = m_lastChar;

		while (std::isdigit(m_lastChar = std::toupper(*(++m_it))))
			m_lastIdentifier += m_lastChar;

		if (m_lastChar == '.')
		{
			m_lastIdentifier += m_lastChar;
			while (std::isdigit(m_lastChar = std::toupper(*(++m_it))))
				m_lastIdentifier += m_lastChar;

			m_dec = std::strtof(m_lastIdentifier.c_str(), nullptr);
			return CBMBASICToken::Float;
		}
		else
		{
			m_val = static_cast<uint16_t>(std::strtoul(m_lastIdentifier.c_str(). nullptr, 10));
			return CBMBASICToken::Integer;
		}
	}

	return CBMBASICToken::Invalid;
}


void CBMBASICState::OpenSourceFile(const std::filesystem::path &path)
{
	if (!m_src.empty()) m_src.clear();

	std::ifstream srcFile(path);
	srcFile.seekg(0, std::ifstream::end);
	size_t size = srcFile.tellg();
	srcFile.seekg(0);

	m_src.resize(size, 0);
	srcFile.read(m_src.data(), size);
	m_it = m_src.begin();
}
