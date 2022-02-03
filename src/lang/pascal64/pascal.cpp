#include <cctype>
#include <cstdlib>

#include "../core/utility.h"
#include "pascal.h"

PascalState::PascalState():
	m_lastToken(PascalToken::Invalid),
	m_lastChar(' '),
{
}


PascalState::~PascalState()
{
	if (m_src.is_open()) m_src.close();
}


PascalToken PascalState::NextToken()
{
	while (std::isspace(m_lastChar))
		m_lastChar = std::toupper(m_src.get());

	// identifier, beginning with a letter
	if (std::isalpha(m_lastChar))
	{
		m_lastIdentifier = m_lastChar;

		while (std::isalnum(m_lastChar = std::toupper(*(++m_it))))
			m_lastIdentifier += m_lastChar;

		if (m_lastIdentifier == "AND") return PascalToken::And;
		if (m_lastIdentifier == "DEF") return PascalToken::Def;
		if (m_lastIdentifier == "DIM") return PascalToken::Dim;
		if (m_lastIdentifier == "ELSE") return PascalToken::Else;
		if (m_lastIdentifier == "FN") return PascalToken::Fn;
		if (m_lastIdentifier == "FOR") return PascalToken::For;
		if (m_lastIdentifier == "GOSUB") return PascalToken::GoSub;
		if (m_lastIdentifier == "GOTO") return PascalToken::GoTo;
		if (m_lastIdentifier == "IF") return PascalToken::If;
		if (m_lastIdentifier == "LET") return PascalToken::Let;
		if (m_lastIdentifier == "NEXT") return PascalToken::Next;
		if (m_lastIdentifier == "NOT") return PascalToken::Not;
		if (m_lastIdentifier == "ON") return PascalToken::On;
		if (m_lastIdentifier == "OR") return PascalToken::Or;
		if (m_lastIdentifier == "RETURN") return PascalToken::Return;
		if (m_lastIdentifier == "STEP") return PascalToken::Step;
		if (m_lastIdentifier == "THEN") return PascalToken::Then;
		if (m_lastIdentifier == "TO") return PascalToken::To;

		// could be a variable or function
		return PascalToken::Identifier;
	}

	// string
	if (m_lastChar == '\"')
	{
		m_lastIdentifier = "";

		while ((m_lastChar = *(++m_it)) != '\"')
			m_lastIdentifier += m_lastChar;

		return PascalToken::String;
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
			return PascalToken::Float;
		}
		else
		{
			m_val = static_cast<uint16_t>(std::strtoul(m_lastIdentifier.c_str(). nullptr, 10));
			return PascalToken::Integer;
		}
	}

	return PascalToken::Invalid;
}


void PascalState::OpenSourceFile(const std::filesystem::path &path)
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
