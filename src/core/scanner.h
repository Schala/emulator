#ifndef _SRC_SCANNER_H
#define _SRC_SCANNER_H

#include <cstddef>

class Scanner
{
public:
	Scanner(const char *s):
		m_line(0),
		m_col(0),
		m_ptr(s)
	{
	}

	void Advance()
	{
		if (!HasMore()) return;

		if (IsNewLine())
		{
			m_line++;
			m_col = 0;
		}
		else
			m_col++;

		m_ptr++;
	}

	inline size_t Column() const
	{
		return m_col;
	}

	inline char Get() const
	{
		return *m_ptr;
	}

	inline bool HasMore() const
	{
		return *m_ptr != 0;
	}

	inline bool IsBinary() const
	{
		if (*m_ptr == '0' || *m_ptr == '1')
			return true;
		return false;
	}

	inline bool IsNewLine() const
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

	inline bool IsOctal() const
	{
		return *m_ptr >= '0' && *m_ptr <= '7';
	}

	inline size_t Line() const
	{
		return m_line;
	}

	inline char Next()
	{
		Advance();
		return Get();
	}

	inline const char * Ptr() const
	{
		return m_ptr;
	}

	void SkipWhitespace()
	{
		while (std::isspace(*m_ptr))
			Advance();
	}
private:
	size_t m_line;
	size_t m_col;
	const char *m_ptr;
};

#endif // _SRC_SCANNER_H
