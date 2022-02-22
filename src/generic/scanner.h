#ifndef _SRC_SCANNER_H
#define _SRC_SCANNER_H

#include <cstddef>

class Scanner
{
public:
	Scanner(const char *);
	void Advance();
	size_t Column() const;
	char Get() const;
	bool HasMore() const;
	bool IsBinary() const;
	bool IsNewLine() const;
	size_t Line() const;
	char Next();
	const char * Ptr() const;
	void SkipWhitespace();
private:
	size_t m_line;
	size_t m_col;
	const char *m_ptr;
};

#endif // _SRC_SCANNER_H
