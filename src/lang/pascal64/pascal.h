#ifndef _PASCAL64_H
#define _PASCAL64_H

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <variant>

enum class Pascal64Token : uint8_t
{
	Invalid,
	EndOfFile,
	And, // AND
	Array, // ARRAY
	Begin, // BEGIN
	Case, // CASE
	Const, // CONST
	Div, // DIV
	Do, // DO
	Else, // ELSE
	End, // END
	Equal, // =
	External, // EXTERNAL
	File, // FILE
	Float, // 3.14
	For, // FOR
	Function, // FUNCTION
	GoTo, // GOTO
	Graphic, // GRAPHIC
	GreaterEqual, // >=
	GreaterThan, // >
	Identifier,
	If, // IF
	In, // IN
	Integer, // INTEGER
	IntegerLiteral, // 123
	Interrupt, // INTERRUPT
	LessEqual, // <=
	LessThan, // <
	Minus, // -
	Mod, // MOD
	Multiply, // *
	Nil, // NIL
	NotEqual, // <>
	Of, // OF
	Off, // OFF
	On, // ON
	Or, // OR
	Packed, // PACKED
	Plus, // +
	Pointer, // ^
	Poke, // POKE
	Procedure, // PROCEDURE
	Program, // PROGRAM
	Range, // [1..5]
	Real, // REAL
	Record, // RECORD
	Repeat, // REPEAT
	Set, // SET
	Sprite, // SPRITE
	String, // ""
	Subscript, // [1]
	Sys, // SYS
	Then, // THEN
	To, // TO
	Type, // TYPE
	Until, // UNTIL
	Var, // VAR
	While, // WHILE
	With // WITH
};

struct Pascal64Variable
{
	std::string_view Name;
	std::variant<std::string, uint16_t> Value;
};

class Pascal64State
{
public:
	Pascal64State();
	~Pascal64State();
	void OpenSourceFile(const std::filesystem::path &);
private:
	Pascal64Token m_lastToken;
	char m_lastChar;
	std::ifstream m_src;
	std::string m_lastIdentifier;

	Pascal64Token NextToken();
};

#endif // _PASCAL64_H
