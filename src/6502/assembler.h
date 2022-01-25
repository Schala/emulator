#ifndef _6502_ASSEMBLER_H
#define _6502_ASSEMBLER_H

#include <cstdint>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

enum class AddressMode6502 : uint8_t
{
	None, // jam
	Absolute,
	AbsoluteX,
	AbsoluteY,
	Immediate,
	Implied,
	Indirect,
	IndirectX,
	IndirectY,
	Relative,
	ZeroPage,
	ZeroPageX,
	ZeroPageY,
};

enum class Token6502 : uint8_t
{
	Invalid,
	EndOfFile,
	Binary,
	Decimal,
	Hex,
	Identifier,
	Offset,
	Operation,
	X,
	Y
};

struct Operation6502
{
	AddressMode6502 Mode;
	std::string_view Label;
};

class Assembler6502State
{
public:
	Assembler6502State();
	Token6502 NextToken();
private:
	Token6502 m_lastToken;
	char m_lastChar;
	uint16_t m_val;
	std::ifstream m_src;
	std::string m_lastIdentifier;
	std::vector<std::string_view> m_labels;
};

#endif // _6502_ASSEMBLER_H
