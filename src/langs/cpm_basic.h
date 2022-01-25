#ifndef _CPM_BASIC_H
#define _CPM_BASIC_H

#include <cstdint>

enum class CPMBASICToken : uint8_t
{
	Invalid = 0,
	EndOfFile,
	And,
	Divide,
	Equal,
	Exp,
	Goto,
	GreaterEqual,
	GreaterThan,
	If,
	Integer,
	IntVar,
	LessEqual,
	LessThan,
	Minus,
	Multiply,
	Not,
	Or,
	Plus,
	String,
	StrVar,
};

class CPMBASICState
{
private:
	CPMBASICToken m_lastToken;
	char m_last
};

#endif // _CPM_BASIC_H
