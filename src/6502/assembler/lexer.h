#ifndef _6502_LEXER_H
#define _6502_LEXER_H

#include <cstdint>
#include <istream>
#include <string>
#include <string_view>
#include <vector>

class Assembler6502;;

enum class Token6502
{
	Invalid,
	EndOfFile,
	NewLine,
	Align, // ALIGN
	And, // AND or &&
	AsciiZ, // ASCIIZ, AS or AZ
	Assert, // ASSERT
	AT, // AT
	BankByte, // BANKBYTE or ^
	BankBytes, // BANKBYTES
	BitAnd, // BITAND or &
	BitNot, // BITNOT or ~
	BitOr, // BITOR or |
	BitXOr, // BITXOR or ^
	BSS, // BSS
	Byte, // BYT, BYTE or DB
	CharMap, // CHARMAP
	CloseParenthesis, // )
	Code, // CODE
	ConDes, // CONDES
	Constructor, // CONSTRUCTOR
	CR, // CR
	Data, // DA or DATA
	DByt, // DBYT
	DebugInfo, // DEBUGINFO
	Define, // DEFINE
	DelMac, // DELMAC
	Divide, // /
	DSB, // DSB
	DWord, // DWORD
	Else, // ELSE
	ElseIf, // ELSEIF
	En, // EN
	End, // END
	EndEnum, // ENDE or ENDENUM
	EndIf, // ENDIF
	EndMacro, // ENDM, ENDMAC or ENDMACRO
	EndProc, // ENDPROC
	EndRepeat, // ENDREP or ENDREPEAT
	EndScope, // ENDSCOPE
	EndStruct, // ENDSTRUCT
	EndUnion, // ENDUNION
	Enum, // ENUM
	Equal, // EQU or =
	Error, // ERROR
	ExitMacro, // EXITMAC or EXITMACRO
	Export, // EXPORT
	ExportZP, // EXPORTZP
	Fatal, // FATAL
	Filename, // foo.txt
	FileOpt, // FOPT or FILEOPT
	ForceImport, // FORCEIMPORT
	Global, // GLOBAL
	GlobalZP, // GLOBALZP
	GreaterEqual, // >=
	GreaterThan, // >
	HiByte, // HIBYTE or >
	HiBytes, // HIBYTES
	Ident, // IDENT
	Identifier,
	If, // IF
	IfBlank, // IFBLANK
	IfConst, // IFCONST
	IfDef, // IFDEF
	IfNDef, // IFNDEF
	IfNRef, // IFNREF
	IfRef, // IFREF
	Import, // IMPORT
	ImportZP, // IMPORTZP
	IncBin, // INCBIN
	Include, // INCLUDE
	Integer, // $0 or 0 or %0
	Interruptor, // INTERRUPTOR
	Label, // :
	LabelAssign, // :=
	LessEqual, // <=
	LessThan, // <
	LF, // LF
	Literal, // LITERAL
	LoByte, // LOBYTE or <
	LoBytes, // LOBYTES
	Local, // LOCAL
	Macro, // MAC or MACRO
	Minus, // -
	Mod, // MOD
	Multiply, // *
	Not, // NOT or !
	NotEqual, // <>
	OpenParenthesis, // (
	Operation, // LDX, SED, ADD, etc.
	Or, // OR or ||
	Org, // ORG, or *=$8000
	Out, // OUT
	PageLength, // PAGELEN, PAGELENGTH
	ParamCount, // PARAMCOUNT
	Plus, // +
	PopCharMap, // POPCHARMAP
	PopCPU, // POPCPU
	PopSeg, // POPSEG
	Proc, // PROC
	PushCharMap, // PUSHCHARMAP
	PushCPU, // PUSHCPU
	PushSeg, // PUSHSEG
	ReferTo, // REFTO or REFERTO
	Reloc, // RELOC
	Repeat, // REPEAT
	Res, // RES
	ROData, // RODATA
	RS, // RS
	RSSet, // RSSET
	Scope, // SCOPE
	ScopeAccess, // ::
	Segment, // SEGMENT
	Set, // SET
	SetCPU, // SETCPU
	ShL, // SHL or <<
	ShR, // SHR or >>
	String, // ""
	Struct, // STRUCT
	Tag, // TAG
	TF, // TF
	Time, // TIME
	Undefine, // UNDEF or UNDEFINE
	Union, // UNION
	Unlimited, // UNLIMITED
	Value, // #
	Warning, // WARNING
	Word, // ADDR, WORD or DW
	X, // X
	XOr, // XOR
	Y, // Y
	ZeroPage // ZEROPAGE
};

class Lexer6502
{
public:
	Lexer6502(Assembler6502 &);
	~Lexer6502();
	std::string & LastIdentifier();
	uint16_t LastValue() const;
	Token6502 NextToken();
	void ResetLineCount();
private:
	char m_lastChar;
	uint16_t m_lastVal;
	std::istream &m_src;
	std::string m_lastIdentifier;
};

#endif // _6502_LEXER_H
