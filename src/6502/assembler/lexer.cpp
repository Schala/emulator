#include <algorithm>
#include <cctype>
#include <cstdlib>

#include "../core/utility.h"
#include "lexer.h"
#include "assembler.h"

using namespace std::literals::string_view_literals;

static const std::vector<std::string_view> Ops =
{
	"A11"sv, "SHX"sv, "SXA"sv, "XAS"sv,
	"AAX"sv, "AXS"sv, "SAX"sv,
	"ADC"sv,
	"AHX"sv, "AXA"sv, "SHA"sv,
	"ALR"sv, "ASR"sv,
	"ANC"sv, "ANC2"sv,
	"AND"sv,
	"ANE"sv, "XAA"sv,
	"ARR"sv,
	"ASL"sv,
	"ASO"sv, "SLO"sv,
	"BCC"sv,
	"BCS"sv,
	"BEQ"sv,
	"BIT"sv,
	"BMI"sv,
	"BNE"sv,
	"BPL"sv,
	"BRK"sv,
	"BVC"sv,
	"BVS"sv,
	"CLC"sv,
	"CLD"sv,
	"CLI"sv,
	"CLV"sv,
	"CMP"sv,
	"CPX"sv,
	"CPY"sv,
	"DCM"sv, "DCP"sv,
	"DEC"sv,
	"DEX"sv,
	"DEY"sv,
	"DOP"sv, "NOP"sv, "TOP"sv,
	"EOR"sv,
	"HLT"sv, "JAM"sv, "KIL"sv,
	"INC"sv,
	"INS"sv, "ISB"sv, "ISC"sv,
	"INX"sv,
	"INY"sv,
	"JMP"sv,
	"JSR"sv,
	"LAR"sv, "LAS"sv,
	"LAX"sv,
	"LDA"sv,
	"LDX"sv,
	"LDY"sv,
	"LSE"sv, "SRE"sv,
	"LSR"sv,
	"LXA"sv,
	"ORA"sv,
	"PHA"sv,
	"PHP"sv,
	"PLA"sv,
	"PLP"sv,
	"RLA"sv,
	"ROL"sv,
	"ROR"sv,
	"RRA"sv,
	"RTI"sv,
	"RTS"sv,
	"SAY"sv, "SHY"sv, "SYA"sv,
	"SBC"sv,
	"SBX"sv,
	"SEC"sv,
	"SED"sv,
	"SEI"sv,
	"SHS"sv, "TAS"sv,
	"STA"sv,
	"STX"sv,
	"STY"sv,
	"TAX"sv,
	"TAY"sv,
	"TSX"sv,
	"TXA"sv,
	"TXS"sv,
	"TYA"sv,
	"USBC"sv
};

Lexer6502::Lexer6502(Assembler6502 &assembler):
	m_lastChar(' '),
	m_lastVal(0),
	m_src(assembler.GetStream())
{
}


Lexer6502::~Lexer6502()
{
	if (m_src.is_open()) m_src.close();
}

std::string & Lexer6502::LastIdentifier()
{
	return m_lastIdentifier;
}


uint16_t Lexer6502::LastValue() const
{
	return m_lastVal;
}

Token6502 Lexer6502::NextToken()
{
	if (IsNewLine(m_lastChar))
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::NewLine;
	}

	while (std::isspace(m_lastChar))
		m_lastChar = std::toupper(m_src.get());

	// line comment
	if (m_lastChar == ';')
	{
		while (!IsNewLine(m_lastChar))
			m_lastChar = std::toupper(m_src.get());
		return NextToken();
	}

	// value
	if (m_lastChar == '#')
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::Value;
	}

	// C comment or divide
	if (m_lastChar == '/')
		if (m_src.peek() == '*')
		{
			m_src.get();

			while (m_lastChar != '*' && m_src.peek() != '/')
				m_lastChar = m_src.get();
			m_lastChar = std::toupper(m_src.get());

			return NextToken();
		}
		else
		{
			m_lastChar = std::toupper(m_src.get());
			return Token6502::Divide;
		}

	// org or multiply
	if (m_lastChar == '*')
		if (m_src.peek() == '=')
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::Org;
		}
		else
		{
			m_lastChar = std::toupper(m_src.get());
			return Token6502::Multiply;
		}

	// lo byte, less than, not, shift left, or <=
	if (m_lastChar == '<')
	{
		char next = m_src.peek();

		if (next == '$' || next == '%' || std::isdigit(next))
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::LoByte;
		}
		else if (next == '>')
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::NotEqual;
		}
		else if (next == '<')
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::ShL;
		}
		else if (next == '=')
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::LessEqual;
		}
		else
		{
			m_lastChar = std::toupper(m_src.get());
			return Token6502::LessThan;
		}
	}

	// hi byte, greater than, shift right, or >=
	if (m_lastChar == '>')
	{
		char next = m_src.peek();

		if (next == '$' || next == '%' || std::isdigit(next))
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::HiByte;
		}
		else if (next == '>')
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::ShR;
		}
		else if (next == '=')
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::GreaterEqual;
		}
		else
		{
			m_lastChar = std::toupper(m_src.get());
			return Token6502::GreaterThan;
		}
	}

	// string
	if (m_lastChar == '\"')
	{
		m_lastIdentifier = "";
		m_lastChar = m_src.get();

		while (m_lastChar != '\"')
		{
			m_lastIdentifier += m_lastChar;
			m_lastChar = m_src.get();
		}

		m_lastChar = std::toupper(m_src.get());

		return Token6502::String;
	}

	// label, label assignment, or scope access
	if (m_lastChar == ':')
		switch (m_src.peek())
		{
			case '=':
				m_src.get();
				m_lastChar = std::toupper(m_src.get());
				return Token6502::LabelAssign;
			case ':':
				m_src.get();
				m_lastChar = std::toupper(m_src.get());
				return Token6502::ScopeAccess;
			default:
				m_lastChar = std::toupper(m_src.get());
				return Token6502::Label;
		}

	// bit and or logical and
	if (m_lastChar == '&')
	{
		if (m_src.peek() == '&')
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::And;
		}
		else
		{
			m_lastChar = std::toupper(m_src.get());
			return Token6502::BitAnd;
		}
	}

	// bit or or logical or
	if (m_lastChar == '|')
	{
		if (m_src.peek() == '|')
		{
			m_src.get();
			m_lastChar = std::toupper(m_src.get());
			return Token6502::Or;
		}
		else
		{
			m_lastChar = std::toupper(m_src.get());
			return Token6502::Or;
		}
	}

	// ignore alignment symbol
	if (m_lastChar == '.')
	{
		m_lastChar = std::toupper(m_src.get());
		return NextToken();
	}

	// assignment, equality
	if (m_lastChar == '=')
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::Equal;
	}

	// negative, subtraction
	if (m_lastChar == '-')
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::Minus;
	}

	// positive, addition
	if (m_lastChar == '+')
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::Plus;
	}

	// bitwise not
	if (m_lastChar == '~')
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::BitNot;
	}

	// logical not
	if (m_lastChar == '!')
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::Not;
	}

	if (m_lastChar == '(')
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::OpenParenthesis;
	}

	if (m_lastChar == ')')
	{
		m_lastChar = std::toupper(m_src.get());
		return Token6502::CloseParenthesis;
	}

	// identifier
	if (std::isalpha(m_lastChar) || m_lastChar == '_' || m_lastChar == '@')
	{
		if (m_lastChar == '@')
			m_lastIdentifier = "";
		else
			m_lastIdentifier = m_lastChar;

		m_lastChar = std::toupper(m_src.get());

		while (std::isalnum(m_lastChar) || m_lastChar == '_' || m_lastChar == '.')
		{
			m_lastIdentifier += m_lastChar;
			m_lastChar = std::toupper(m_src.get());
		}

		// operation?
		if (std::find(Ops.begin(), Ops.end(), m_lastIdentifier) != Ops.end())
			return Token6502::Operation;

		// keywords
		if (m_lastIdentifier == "ADDR") return Token6502::Word;
		if (m_lastIdentifier == "ALIGN") return Token6502::Align;
		if (m_lastIdentifier == "AND") return Token6502::And;
		if (m_lastIdentifier == "AS") return Token6502::AsciiZ;
		if (m_lastIdentifier == "ASCIIZ") return Token6502::AsciiZ;
		if (m_lastIdentifier == "ASSERT") return Token6502::Assert;
		if (m_lastIdentifier == "AT") return Token6502::AT;
		if (m_lastIdentifier == "AZ") return Token6502::AsciiZ;
		if (m_lastIdentifier == "BANKBYTE") return Token6502::BankByte;
		if (m_lastIdentifier == "BANKBYTES") return Token6502::BankBytes;
		if (m_lastIdentifier == "BITAND") return Token6502::BitAnd;
		if (m_lastIdentifier == "BITNOT") return Token6502::BitNot;
		if (m_lastIdentifier == "BITOR") return Token6502::BitOr;
		if (m_lastIdentifier == "BITXOR") return Token6502::BitXOr;
		if (m_lastIdentifier == "BSS") return Token6502::BSS;
		if (m_lastIdentifier == "BYT") return Token6502::Byte;
		if (m_lastIdentifier == "BYTE") return Token6502::Byte;
		if (m_lastIdentifier == "CHARMAP") return Token6502::CharMap;
		if (m_lastIdentifier == "CR") return Token6502::CR;
		if (m_lastIdentifier == "DA") return Token6502::Data;
		if (m_lastIdentifier == "DATA") return Token6502::Data;
		if (m_lastIdentifier == "DB") return Token6502::Byte;
		if (m_lastIdentifier == "DBYT") return Token6502::DByt;
		if (m_lastIdentifier == "DEBUGINFO") return Token6502::DebugInfo;
		if (m_lastIdentifier == "DEFINE") return Token6502::Define;
		if (m_lastIdentifier == "DELMAC") return Token6502::DelMac;
		if (m_lastIdentifier == "DSB") return Token6502::DSB;
		if (m_lastIdentifier == "DW") return Token6502::Word;
		if (m_lastIdentifier == "DWORD") return Token6502::DWord;
		if (m_lastIdentifier == "ELSE") return Token6502::Else;
		if (m_lastIdentifier == "ELSEIF") return Token6502::ElseIf;
		if (m_lastIdentifier == "EN") return Token6502::En;
		if (m_lastIdentifier == "END") return Token6502::End;
		if (m_lastIdentifier == "ENDE") return Token6502::EndEnum;
		if (m_lastIdentifier == "ENDENUM") return Token6502::EndEnum;
		if (m_lastIdentifier == "ENDIF") return Token6502::EndIf;
		if (m_lastIdentifier == "ENDM") return Token6502::EndMacro;
		if (m_lastIdentifier == "ENDMACRO") return Token6502::EndMacro;
		if (m_lastIdentifier == "ENDPROC") return Token6502::EndProc;
		if (m_lastIdentifier == "ENDREPEAT") return Token6502::EndRepeat;
		if (m_lastIdentifier == "ENDSCOPE") return Token6502::EndScope;
		if (m_lastIdentifier == "ENDSTRUCT") return Token6502::EndStruct;
		if (m_lastIdentifier == "ENDUNION") return Token6502::EndUnion;
		if (m_lastIdentifier == "ENUM") return Token6502::Enum;
		if (m_lastIdentifier == "EQU") return Token6502::Equal;
		if (m_lastIdentifier == "ERROR") return Token6502::Error;
		if (m_lastIdentifier == "EXITMACRO") return Token6502::ExitMacro;
		if (m_lastIdentifier == "EXPORT") return Token6502::Export;
		if (m_lastIdentifier == "EXPORTZP") return Token6502::ExportZP;
		if (m_lastIdentifier == "FATAL") return Token6502::Fatal;
		if (m_lastIdentifier == "FILEOPT") return Token6502::FileOpt;
		if (m_lastIdentifier == "FOPT") return Token6502::FileOpt;
		if (m_lastIdentifier == "FORCEIMPORT") return Token6502::ForceImport;
		if (m_lastIdentifier == "GLOBAL") return Token6502::Global;
		if (m_lastIdentifier == "GLOBALZP") return Token6502::GlobalZP;
		if (m_lastIdentifier == "HIBYTE") return Token6502::HiByte;
		if (m_lastIdentifier == "HIBYTES") return Token6502::HiBytes;
		if (m_lastIdentifier == "IDENT") return Token6502::Ident;
		if (m_lastIdentifier == "IF") return Token6502::If;
		if (m_lastIdentifier == "IFBLANK") return Token6502::IfBlank;
		if (m_lastIdentifier == "IFCONST") return Token6502::IfConst;
		if (m_lastIdentifier == "IFDEF") return Token6502::IfDef;
		if (m_lastIdentifier == "IFNDEF") return Token6502::IfNDef;
		if (m_lastIdentifier == "IFNREF") return Token6502::IfNRef;
		if (m_lastIdentifier == "IFREF") return Token6502::IfRef;
		if (m_lastIdentifier == "IMPORT") return Token6502::Import;
		if (m_lastIdentifier == "IMPORTZP") return Token6502::ImportZP;
		if (m_lastIdentifier == "INCBIN") return Token6502::IncBin;
		if (m_lastIdentifier == "INCLUDE") return Token6502::Include;
		if (m_lastIdentifier == "INTERRUPTOR") return Token6502::Interruptor;
		if (m_lastIdentifier == "LF") return Token6502::LF;
		if (m_lastIdentifier == "LITERAL") return Token6502::Literal;
		if (m_lastIdentifier == "LOBYTE") return Token6502::LoByte;
		if (m_lastIdentifier == "LOBYTES") return Token6502::LoBytes;
		if (m_lastIdentifier == "LOCAL") return Token6502::Local;
		if (m_lastIdentifier == "MAC") return Token6502::Macro;
		if (m_lastIdentifier == "MACRO") return Token6502::Macro;
		if (m_lastIdentifier == "MOD") return Token6502::Mod;
		if (m_lastIdentifier == "NOT") return Token6502::Not;
		if (m_lastIdentifier == "OR") return Token6502::Or;
		if (m_lastIdentifier == "ORG") return Token6502::Org;
		if (m_lastIdentifier == "OUT") return Token6502::Out;
		if (m_lastIdentifier == "PAGELEN") return Token6502::PageLength;
		if (m_lastIdentifier == "PAGELENGTH") return Token6502::PageLength;
		if (m_lastIdentifier == "POPCHARMAP") return Token6502::PopCharMap;
		if (m_lastIdentifier == "POPCPU") return Token6502::PopCPU;
		if (m_lastIdentifier == "POPSEG") return Token6502::PopSeg;
		if (m_lastIdentifier == "PROC") return Token6502::Proc;
		if (m_lastIdentifier == "PUSHCHARMAP") return Token6502::PushCharMap;
		if (m_lastIdentifier == "PUSHCPU") return Token6502::PushCPU;
		if (m_lastIdentifier == "PUSHSEG") return Token6502::PushSeg;
		if (m_lastIdentifier == "REFERTO") return Token6502::ReferTo;
		if (m_lastIdentifier == "REFTO") return Token6502::ReferTo;
		if (m_lastIdentifier == "RES") return Token6502::Res;
		if (m_lastIdentifier == "RODATA") return Token6502::ROData;
		if (m_lastIdentifier == "RS") return Token6502::RS;
		if (m_lastIdentifier == "RSSET") return Token6502::RSSet;
		if (m_lastIdentifier == "SCOPE") return Token6502::Scope;
		if (m_lastIdentifier == "SEGMENT") return Token6502::Segment;
		if (m_lastIdentifier == "SET") return Token6502::Set;
		if (m_lastIdentifier == "SETCPU") return Token6502::SetCPU;
		if (m_lastIdentifier == "SHL") return Token6502::ShL;
		if (m_lastIdentifier == "SHR") return Token6502::ShR;
		if (m_lastIdentifier == "STRUCT") return Token6502::Struct;
		if (m_lastIdentifier == "TAG") return Token6502::Tag;
		if (m_lastIdentifier == "TF") return Token6502::TF;
		if (m_lastIdentifier == "TIME") return Token6502::ReferTo;
		if (m_lastIdentifier == "UNDEF") return Token6502::Undefine;
		if (m_lastIdentifier == "UNDEFINE") return Token6502::Undefine;
		if (m_lastIdentifier == "UNION") return Token6502::Union;
		if (m_lastIdentifier == "UNLIMITED") return Token6502::Unlimited;
		if (m_lastIdentifier == "WARNING") return Token6502::Warning;
		if (m_lastIdentifier == "WORD") return Token6502::Word;
		if (m_lastIdentifier == "X") return Token6502::X;
		if (m_lastIdentifier == "XOR") return Token6502::XOr;
		if (m_lastIdentifier == "Y") return Token6502::Y;
		if (m_lastIdentifier == "ZEROPAGE") return Token6502::ZeroPage;

		// something else
		return Token6502::Identifier;
	}

	// binary
	if (m_lastChar == '%')
	{
		m_lastIdentifier = "";

		while (IsBinary(m_lastChar = std::toupper(m_src.get())))
			m_lastIdentifier += m_lastChar;

		if (m_lastIdentifier.empty())
			return Token6502::Invalid;
		else
		{
			m_lastVal = static_cast<uint16_t>(std::strtoul(m_lastIdentifier.c_str(), nullptr, 2));
			return Token6502::Integer;
		}
	}

	// hex
	if (m_lastChar == '$')
	{
		m_lastIdentifier = "";

		while (std::isxdigit(m_lastChar = std::toupper(m_src.get())))
			m_lastIdentifier += m_lastChar;

		if (m_lastIdentifier.empty())
			return Token6502::Invalid;
		else
		{
			m_lastVal = static_cast<uint16_t>(std::strtoul(m_lastIdentifier.c_str(), nullptr, 16));
			return Token6502::Integer;
		}
	}

	// decimal
	if (std::isdigit(m_lastChar))
	{
		m_lastIdentifier = m_lastChar;

		while (std::isdigit(m_lastChar = std::toupper(m_src.get())))
			m_lastIdentifier += m_lastChar;

		m_lastVal = static_cast<uint16_t>(std::strtoul(m_lastIdentifier.c_str(), nullptr, 10));
		return Token6502::Integer;
	}

	if (m_src.eof())
		return Token6502::EndOfFile;

	m_lastChar = std::toupper(m_src.get());
	return Token6502::Invalid;
}
