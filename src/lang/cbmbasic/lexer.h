#ifndef _CBMBASIC_LEXER_H
#define _CBMBASIC_LEXER_H

#include <cstddef>
#include <sstream>
#include <string>
#include <utility>
#include <variant>

#include "../../core/lexer.h"
#include "../../core/scanner.h"

enum class CBMBASICTokenID
{
	EndOfInput,
	Error,

	Asterisk,
	Equal,
	Greater,
	GreaterEqual,
	Less,
	LessEqual,
	Minus,
	ParenthesisLeft,
	ParenthesisRight,
	Plus,
	Semicolon,
	Slash,

	FloatLiteral,
	Identifier,
	IntegerLiteral,
	StringLiteral,

	Abs,
	And,
	Append,
	Asc,
	Atn,
	Auto,
	Background,
	Backup,
	Bank,
	Begin,
	BEnd,
	BLoad,
	Boot,
	Border,
	Box,
	BSave,
	Bump,
	BVerify,
	Catalog,
	Change,
	Char,
	Chr, // CHR$
	Circle,
	Close,
	Clr,
	Cmd,
	Collect,
	Collision,
	Color,
	Concat,
	Cont,
	Copy,
	Cos,
	Cut,
	Data,
	DClear,
	DClose,
	Dec,
	Def,
	Delete,
	Dim,
	Directory,
	Disk,
	Dispose,
	DLoad,
	DMA,
	DMode,
	Do,
	DOpen,
	DPat,
	Draw,
	DS,
	DSave,
	DSVar,
	DVerify,
	Ellipse, // EL or ELLIPSE
	Else,
	End,
	Envelope,
	Erase, // ER or ERASE
	Err, // ERR$
	Esc,
	Exit,
	Exp,
	Fast,
	Fetch,
	Filter,
	Find,
	Fn,
	For,
	Foreground,
	Fre,
	GCopy,
	GenLock,
	Get,
	GetHash, // GET#
	GetKey,
	Go64,
	GoSub,
	GoTo,
	Graphic,
	GShape,
	Header,
	Help,
	Hex, // HEX$
	Highlight,
	If,
	Input,
	InputHash, // INPUT#,
	InStr,
	Int,
	Joy,
	Key,
	Left, // LEFT$
	Len,
	Let,
	Line,
	List,
	Load,
	Locate,
	Log,
	Loop,
	LPen,
	Mid, // MID$
	Monitor,
	Mouse,
	MovSpr,
	New,
	Next,
	Not, // <> or NOT
	Off,
	On,
	Open,
	Or,
	Paint,
	Palette,
	Paste,
	Peek,
	Pen,
	Pic,
	Play,
	Pointer,
	Poke,
	Polygon,
	Popups,
	Pos,
	Pot,
	Print,
	PrintHash, // PRINT#
	PUDef,
	Quit,
	RClr,
	RDot,
	Read,
	ReadHash, // READ#
	Record,
	Rename,
	Renumber,
	Restore,
	Resume,
	Return,
	Rgr,
	Right, // RIGHT$
	RLum,
	Rnd,
	RReg,
	RSpColor,
	RSpPos,
	RSprite,
	Run,
	RWindow,
	Save,
	Scale,
	ScnClr,
	Scratch,
	Screen,
	Set,
	Sgn,
	Sin,
	Sleep,
	Slow,
	Sound,
	Spc,
	SprColor,
	SprDef,
	Sprite,
	SprSav,
	Sqr,
	SShape,
	Stash,
	Status, // ST or STATUS
	Step,
	Stop,
	Str, // STR$
	Strs,
	Swap,
	Sys,
	Tab,
	Tan,
	Tempo,
	Then,
	Time, // TI or TIME
	TimeVar, // TI$ or TIME$
	To,
	Trap,
	TROff,
	TROn,
	Type,
	Until,
	Using,
	Usr,
	Val,
	Verify,
	Viewport,
	Volume, // VOL or VOLUME
	Wait,
	While,
	Width,
	XOr
};

typedef std::variant<std::nullptr_t, double, int, std::string> CBMBASICTokenValue;
typedef Token<CBMBASICTokenID, CBMBASICTokenValue> CBMBASICToken;

class CBMBASICLexer
{
public:
	CBMBASICLexer(const char *);

	template <class ...Args> CBMBASICToken Error(Args ...args)
	{
		std::string code(m_lastState.Ptr(), m_state.Column() - m_lastState.Column());
		std::ostringstream msg;
		(msg << ... << std::forward<Args>(args)) << '\n'
			<< '(' << m_state.Line() << ", " << m_state.Column() << "): " << code;

		m_state.Advance();

		return MakeToken(CBMBASICTokenID::Error, msg.str());
	}

	CBMBASICToken Expect(char, CBMBASICTokenID);
	bool HasMore() const;
	CBMBASICToken NextToken();
private:
	Scanner m_lastState;
	Scanner m_state;

	CBMBASICToken Float();
	CBMBASICToken Identifier();
	void LineComment();
	CBMBASICToken MakeToken(CBMBASICTokenID id, CBMBASICTokenValue value = nullptr) const;
	CBMBASICToken Number(bool);
	CBMBASICToken Simple(CBMBASICTokenID);
	CBMBASICToken String();
};

constexpr bool ValidCBMBASICIdentifier(char);

#endif // _CBMBASIC_LEXER_H
