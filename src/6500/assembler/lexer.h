#ifndef _6500_LEXER_H
#define _6500_LEXER_H

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <utility>
#include <variant>

#include "../../core/lexer.h"
#include "../../generic/scanner.h"

enum class Token6500ID
{
	EndOfInput,
	Error,

	Colon,
	Comma,
	Offset,
	ParenthesisLeft,
	ParenthesisRight,
	Value,

	Identifier,
	IntegerLiteral,
	StringLiteral,

	X,
	Y,

	Byte,
	Text,

	ADC,
	AND,
	ASL,
	BCC,
	BCS,
	BEQ,
	BIT,
	BMI,
	BNE,
	BPL,
	BRK,
	BVC,
	BVS,
	CLC,
	CLD,
	CLI,
	CLV,
	CMP,
	CPX,
	CPY,
	DEC,
	DEX,
	DEY,
	EOR,
	INC,
	INX,
	INY,
	JMP,
	JSR,
	LDA,
	LDX,
	LDY,
	LSR,
	NOP,
	ORA,
	PHA,
	PHP,
	PLA,
	PLP,
	ROL,
	ROR,
	RTI,
	RTS,
	SBC,
	SEC,
	SED,
	SEI,
	STA,
	STX,
	STY,
	TAX,
	TAY,
	TSX,
	TXA,
	TXS,
	TYA
};

typedef std::variant<std::nullptr_t, std::string, uint16_t> Token6500Value;
typedef Token<Token6500ID, Token6500Value> Token6500;

class Lexer6500
{
public:
	Lexer6500(const char *);

	template <class ...Args> Token6500 Error(Args ...args)
	{
		std::string code(m_lastState.Ptr(), m_state.Column() - m_lastState.Column());
		std::ostringstream msg;
		(msg << ... << std::forward<Args>(args)) << '\n'
			<< '(' << m_state.Line() << ", " << m_state.Column() << "): " << code;

		m_state.Advance();

		return MakeToken(Token6500ID::Error, msg.str());
	}

	Token6500 Expect(char, Token6500ID);
	bool HasMore() const;
	Token6500 NextToken();
private:
	Scanner m_lastState;
	Scanner m_state;

	Token6500 Binary();
	Token6500 Decimal();
	Token6500 Follow(char, Token6500ID, Token6500ID);
	Token6500 Hex();
	Token6500 Identifier(bool);
	void LineComment();
	Token6500 MakeToken(Token6500ID id, Token6500Value value = nullptr) const;
	Token6500 Offset();
	Token6500 Simple(Token6500ID);
	Token6500 String();
};

std::string SanitizeString(std::string);
const char * Token6500IDString(Token6500ID);
constexpr bool Valid6500Identifier(char);

#endif // _6500_LEXER_H
