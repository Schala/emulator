#ifndef _SRC_SCANNER_H
#define _SRC_SCANNER_H

#include <cctype>
#include <istream>
#include <string>
#include <utility>
#include <vector>

class Scanner
{
public:
	Scanner():
		m_col(1),
		m_line(1)
	{
	}

	Scanner(std::istream &s):
		m_col(1),
		m_line(1)
	{
		Set(s);
	}

	void Advance()
	{
		if (!HasMore()) return;

		if (IsNewLine())
			NextLine();
		else
		{
			m_col++;
			m_colIt++;
		}
	}

	inline void AddLine(std::string s)
	{
		m_src.push_back(std::move(s));
	}

	inline size_t Column() const
	{
		return m_col;
	}

	inline std::string::iterator ColumnIterator() const
	{
		return m_colIt;
	}

	inline char Get() const
	{
		return *m_colIt;
	}

	inline std::string GetAll() const
	{
		std::string s;

		for (auto &line : m_src)
		{
			s += line;
			s += '\n';
		}

		return std::move(s);
	}

	inline bool HasMore() const
	{
		return m_lineIt != m_src.end();
	}

	inline bool IsBinary() const
	{
		if (*m_colIt == '0' || *m_colIt == '1')
			return true;
		return false;
	}

	inline bool IsNewLine() const
	{
		switch (*m_colIt)
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
		return *m_colIt >= '0' && *m_colIt <= '7';
	}

	inline size_t Line() const
	{
		return m_line;
	}

	inline size_t LineCount() const
	{
		return m_src.size();
	}

	inline std::vector<std::string>::iterator LineIterator() const
	{
		return m_lineIt;
	}

	inline char Next()
	{
		Advance();
		return Get();
	}

	inline void NextLine()
	{
		m_line++;
		m_lineIt++;
		m_col = 1;
		m_colIt = m_lineIt->begin();
	}

	inline void Rewind()
	{
		m_line = m_col = 1;
		m_lineIt = m_src.begin();
		m_colIt = m_lineIt->begin();
	}

	void Set(std::istream &s)
	{
		m_src.clear();

		while (!s.eof())
		{
			std::string line;

			std::getline(s, line);
			m_src.push_back(std::move(line));
		}

		m_lineIt = m_src.begin();
		m_colIt = m_lineIt->begin();
	}

	inline void SkipWhitespace()
	{
		while (std::isspace(*m_colIt))
			Advance();
	}
private:
	size_t m_col;
	size_t m_line;
	std::string::iterator m_colIt;
	std::vector<std::string>::iterator m_lineIt;
	std::vector<std::string> m_src;
};

#endif // _SRC_SCANNER_H
