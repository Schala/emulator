#ifndef _CBMBASIC_COMPILER_H
#define _CBMBASIC_COMPILER_H

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <variant>

enum class CBMBASICToken : uint8_t
{
	Invalid,
	EndOfFile,
	And, // AND
	Def, // DEF
	Dim, // DIM
	Divide, // /
	Else, // ELSE
	Equal, // =
	Float, // 1.23
	Fn, // FN
	For, // FOR
	GoSub, // GOSUB
	GoTo, // GOTO
	GreaterEqual, // >=
	GreaterThan, // >
	Identifier,
	If, // IF
	Integer, // 123
	LessEqual, // <=
	LessThan, // <
	Let, // LET
	Minus, // -
	Multiply, // *
	Next, // NEXT
	Not, // NOT
	On, // ON
	Or, // OR
	Plus, // +
	Return, // RETURN
	Step, // STEP
	String, // ""
	Then, // THEN
	To // TO
};

struct CBMBASICVariable
{
	std::string_view Name;
	std::variant<std::string, uint16_t> Value;
};

class CBMBASICState
{
public:
	CBMBASICState();
	void OpenSourceFile(const std::filesystem::path &);
private:
	CBMBASICToken m_lastToken;
	char m_lastChar;
	uint16_t m_val;
	float m_dec;
	std::string::iterator m_it;
	std::string m_src;
	std::string m_lastIdentifier;

	CBMBASICToken NextToken();
};

#endif // _CBMBASIC_COMPILER_H
