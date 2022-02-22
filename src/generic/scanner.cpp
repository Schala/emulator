#include <cctype>

#include "scanner.h"

Scanner::Scanner(const char *s):
	m_line(1),
	m_col(1),
	m_ptr(s)
{
}

void Scanner::Advance()
{
	if (!HasMore()) return;

	if (IsNewLine())
	{
		m_line++;
		m_col = 1;
	}
	else
		m_col++;

	m_ptr++;
}

size_t Scanner::Column() const
{
	return m_col;
}

char Scanner::Get() const
{
	return *m_ptr;
}

bool Scanner::HasMore() const
{
	return *m_ptr != 0;
}

bool Scanner::IsBinary() const
{
	if (*m_ptr == '0' || *m_ptr == '1')
		return true;
	return false;
}

bool Scanner::IsNewLine() const
{
	switch (*m_ptr)
	{
		case '\n':
		case '\r':
			return true;
		default:
			return false;
	}
}

size_t Scanner::Line() const
{
	return m_line;
}

char Scanner::Next()
{
	Advance();
	return Get();
}

const char * Scanner::Ptr() const
{
	return m_ptr;
}

void Scanner::SkipWhitespace()
{
	while (std::isspace(*m_ptr))
		Advance();
}
