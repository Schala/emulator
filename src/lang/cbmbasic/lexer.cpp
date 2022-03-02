#include <cctype>
#include <charconv>
#include <cstring>
#include <map>
#include <system_error>

#include "lexer.h"

static const std::map<const char *, CBMBASICTokenID> Keywords =
{
	{ "ABS", CBMBASICTokenID::Abs },
	{ "AND", CBMBASICTokenID::And },
	{ "APPEND", CBMBASICTokenID::Append },
	{ "ASC", CBMBASICTokenID::Asc },
	{ "ATN", CBMBASICTokenID::Atn },
	{ "AUTO", CBMBASICTokenID::Auto },
	{ "BACKGROUND", CBMBASICTokenID::Background },
	{ "BACKUP", CBMBASICTokenID::Backup },
	{ "BANK", CBMBASICTokenID::Bank },
	{ "BEGIN", CBMBASICTokenID::Begin },
	{ "BEND", CBMBASICTokenID::BEnd },
	{ "BLOAD", CBMBASICTokenID::BLoad },
	{ "BOOT", CBMBASICTokenID::Boot },
	{ "BORDER", CBMBASICTokenID::Border },
	{ "BOX", CBMBASICTokenID::Box },
	{ "BSAVE", CBMBASICTokenID::BSave },
	{ "BUMP", CBMBASICTokenID::Bump },
	{ "BVERIFY", CBMBASICTokenID::BVerify },
	{ "CATALOG", CBMBASICTokenID::Catalog },
	{ "CHANGE", CBMBASICTokenID::Change },
	{ "CHAR", CBMBASICTokenID::Char },
	{ "CHR$", CBMBASICTokenID::Chr },
	{ "CIRCLE", CBMBASICTokenID::Circle },
	{ "CLOSE", CBMBASICTokenID::Close },
	{ "CLR", CBMBASICTokenID::Clr },
	{ "CMD", CBMBASICTokenID::Cmd },
	{ "COLLECT", CBMBASICTokenID::Collect },
	{ "COLLISION", CBMBASICTokenID::Collision },
	{ "COLOR", CBMBASICTokenID::Color },
	{ "CONCAT", CBMBASICTokenID::Concat },
	{ "CONT", CBMBASICTokenID::Cont },
	{ "COPY", CBMBASICTokenID::Copy },
	{ "COS", CBMBASICTokenID::Cos },
	{ "CUT", CBMBASICTokenID::Cut },
	{ "DATA", CBMBASICTokenID::Data },
	{ "DCLEAR", CBMBASICTokenID::DClear },
	{ "DCLOSE", CBMBASICTokenID::DClose },
	{ "DEC", CBMBASICTokenID::Dec },
	{ "DEF", CBMBASICTokenID::Def },
	{ "DELETE", CBMBASICTokenID::Delete },
	{ "DIM", CBMBASICTokenID::Dim },
	{ "DIR", CBMBASICTokenID::Directory },
	{ "DIRECTORY", CBMBASICTokenID::Directory },
	{ "DISK", CBMBASICTokenID::Disk },
	{ "DISPOSE", CBMBASICTokenID::Dispose },
	{ "DLOAD", CBMBASICTokenID::DLoad },
	{ "DMA", CBMBASICTokenID::DMA },
	{ "DMODE", CBMBASICTokenID::DMode },
	{ "DO", CBMBASICTokenID::Do },
	{ "DOPEN", CBMBASICTokenID::DOpen },
	{ "DPAT", CBMBASICTokenID::DPat },
	{ "DRAW", CBMBASICTokenID::Draw },
	{ "DS", CBMBASICTokenID::DS },
	{ "DSAVE", CBMBASICTokenID::DSave },
	{ "DS$", CBMBASICTokenID::DSVar },
	{ "DVERIFY", CBMBASICTokenID::DVerify },
	{ "EL", CBMBASICTokenID::Ellipse },
	{ "ELLIPSE", CBMBASICTokenID::Ellipse },
	{ "ELSE", CBMBASICTokenID::Else },
	{ "END", CBMBASICTokenID::End },
	{ "ENVELOPE", CBMBASICTokenID::Envelope },
	{ "ER", CBMBASICTokenID::Erase },
	{ "ERASE", CBMBASICTokenID::Erase },
	{ "ERR$", CBMBASICTokenID::Err },
	{ "ESC", CBMBASICTokenID::Esc },
	{ "EXIT", CBMBASICTokenID::Exit },
	{ "EXP", CBMBASICTokenID::Exp },
	{ "FAST", CBMBASICTokenID::Fast },
	{ "FETCH", CBMBASICTokenID::Fetch },
	{ "FILTER", CBMBASICTokenID::Filter },
	{ "FIND", CBMBASICTokenID::Find },
	{ "FN", CBMBASICTokenID::Fn },
	{ "FOR", CBMBASICTokenID::For },
	{ "FOREGROUND", CBMBASICTokenID::Foreground },
	{ "FRE", CBMBASICTokenID::Fre },
	{ "GCOPY", CBMBASICTokenID::GCopy },
	{ "GENLOCK", CBMBASICTokenID::GenLock },
	{ "GET", CBMBASICTokenID::Get },
	{ "GET#", CBMBASICTokenID::GetHash },
	{ "GETKEY", CBMBASICTokenID::GetKey },
	{ "GO64", CBMBASICTokenID::Go64 },
	{ "GOSUB", CBMBASICTokenID::GoSub },
	{ "GOTO", CBMBASICTokenID::GoTo },
	{ "GRAPHIC", CBMBASICTokenID::Graphic },
	{ "GSHAPE", CBMBASICTokenID::GShape },
	{ "HEADER", CBMBASICTokenID::Header },
	{ "HELP", CBMBASICTokenID::Help },
	{ "HEX$", CBMBASICTokenID::Hex },
	{ "HIGHLIGHT", CBMBASICTokenID::Highlight },
	{ "IF", CBMBASICTokenID::If },
	{ "INPUT", CBMBASICTokenID::Input },
	{ "INPUT#", CBMBASICTokenID::InputHash },
	{ "INSTR", CBMBASICTokenID::InStr },
	{ "INT", CBMBASICTokenID::Int },
	{ "JOY", CBMBASICTokenID::Joy },
	{ "KEY", CBMBASICTokenID::Key },
	{ "LEFT$", CBMBASICTokenID::Left },
	{ "LEN", CBMBASICTokenID::Len },
	{ "LET", CBMBASICTokenID::Let },
	{ "LINE", CBMBASICTokenID::Line },
	{ "LIST", CBMBASICTokenID::List },
	{ "LOAD", CBMBASICTokenID::Load },
	{ "LOCATE", CBMBASICTokenID::Locate },
	{ "LOG", CBMBASICTokenID::Log },
	{ "LOOP", CBMBASICTokenID::Loop },
	{ "LPEN", CBMBASICTokenID::LPen },
	{ "MID$", CBMBASICTokenID::Mid },
	{ "MONITOR", CBMBASICTokenID::Monitor },
	{ "MOUSE", CBMBASICTokenID::Mouse },
	{ "MOVSPR", CBMBASICTokenID::MovSpr },
	{ "NEW", CBMBASICTokenID::New },
	{ "NEXT", CBMBASICTokenID::Next },
	{ "NOT", CBMBASICTokenID::Not },
	{ "OFF", CBMBASICTokenID::Off },
	{ "ON", CBMBASICTokenID::On },
	{ "OPEN", CBMBASICTokenID::Open },
	{ "OR", CBMBASICTokenID::Or },
	{ "PAINT", CBMBASICTokenID::Paint },
	{ "PALETTE", CBMBASICTokenID::Palette },
	{ "PASTE", CBMBASICTokenID::Paste },
	{ "PEEK", CBMBASICTokenID::Peek },
	{ "PEN", CBMBASICTokenID::Pen },
	{ "PIC", CBMBASICTokenID::Pic },
	{ "PLAY", CBMBASICTokenID::Play },
	{ "POINTER", CBMBASICTokenID::Pointer },
	{ "POKE", CBMBASICTokenID::Poke },
	{ "POLYGON", CBMBASICTokenID::Polygon },
	{ "POPUPS", CBMBASICTokenID::Popups },
	{ "POS", CBMBASICTokenID::Pos },
	{ "POT", CBMBASICTokenID::Pot },
	{ "PRINT", CBMBASICTokenID::Print },
	{ "PRINT#", CBMBASICTokenID::PrintHash },
	{ "PUDEF", CBMBASICTokenID::PUDef },
	{ "QUIT", CBMBASICTokenID::Quit },
	{ "RCLR", CBMBASICTokenID::RClr },
	{ "RDOT", CBMBASICTokenID::RDot },
	{ "READ", CBMBASICTokenID::Read },
	{ "READ#", CBMBASICTokenID::ReadHash },
	{ "RECORD", CBMBASICTokenID::Record },
	{ "RENAME", CBMBASICTokenID::Rename },
	{ "RENUMBER", CBMBASICTokenID::Renumber },
	{ "RESTORE", CBMBASICTokenID::Restore },
	{ "RESUME", CBMBASICTokenID::Resume },
	{ "RETURN", CBMBASICTokenID::Return },
	{ "RGR", CBMBASICTokenID::Rgr },
	{ "RIGHT$", CBMBASICTokenID::Right },
	{ "RLUM", CBMBASICTokenID::RLum },
	{ "RND", CBMBASICTokenID::Rnd },
	{ "RREG", CBMBASICTokenID::RReg },
	{ "RSPCOLOR", CBMBASICTokenID::RSpColor },
	{ "RSPPOS", CBMBASICTokenID::RSpPos },
	{ "RSPRITE", CBMBASICTokenID::RSprite },
	{ "RUN", CBMBASICTokenID::Run },
	{ "RWINDOW", CBMBASICTokenID::RWindow },
	{ "SAVE", CBMBASICTokenID::Save },
	{ "SCALE", CBMBASICTokenID::Scale },
	{ "SCNCLR", CBMBASICTokenID::ScnClr },
	{ "SCRATCH", CBMBASICTokenID::Scratch },
	{ "SCREEN", CBMBASICTokenID::Screen },
	{ "SET", CBMBASICTokenID::Set },
	{ "SGN", CBMBASICTokenID::Sgn },
	{ "SIN", CBMBASICTokenID::Sin },
	{ "SLEEP", CBMBASICTokenID::Sleep },
	{ "SLOW", CBMBASICTokenID::Slow },
	{ "SOUND", CBMBASICTokenID::Sound },
	{ "SPC", CBMBASICTokenID::Spc },
	{ "SPRCOLOR", CBMBASICTokenID::SprColor },
	{ "SPRDEF", CBMBASICTokenID::SprDef },
	{ "SPRITE", CBMBASICTokenID::Sprite },
	{ "SPRSAV", CBMBASICTokenID::SprSav },
	{ "SQR", CBMBASICTokenID::Sqr },
	{ "SSHAPE", CBMBASICTokenID::SShape },
	{ "STASH", CBMBASICTokenID::Stash },
	{ "ST", CBMBASICTokenID::Status },
	{ "STATUS", CBMBASICTokenID::Status },
	{ "STEP", CBMBASICTokenID::Step },
	{ "STOP", CBMBASICTokenID::Stop },
	{ "STR$", CBMBASICTokenID::Str },
	{ "STRS", CBMBASICTokenID::Strs },
	{ "SWAP", CBMBASICTokenID::Swap },
	{ "SYS", CBMBASICTokenID::Sys },
	{ "TAB", CBMBASICTokenID::Tab },
	{ "TAN", CBMBASICTokenID::Tan },
	{ "TEMPO", CBMBASICTokenID::Tempo },
	{ "THEN", CBMBASICTokenID::Then },
	{ "TI", CBMBASICTokenID::Time },
	{ "TI$", CBMBASICTokenID::TimeVar },
	{ "TIME", CBMBASICTokenID::Time },
	{ "TIME$", CBMBASICTokenID::TimeVar },
	{ "TO", CBMBASICTokenID::To },
	{ "TRAP", CBMBASICTokenID::Trap },
	{ "TROFF", CBMBASICTokenID::TROff },
	{ "TRON", CBMBASICTokenID::TROn },
	{ "TYPE", CBMBASICTokenID::Type },
	{ "UNTIL", CBMBASICTokenID::Until },
	{ "USING", CBMBASICTokenID::Using },
	{ "USR", CBMBASICTokenID::Usr },
	{ "VAL", CBMBASICTokenID::Val },
	{ "VERIFY", CBMBASICTokenID::Verify },
	{ "VIEWPORT", CBMBASICTokenID::Viewport },
	{ "VOL", CBMBASICTokenID::Volume },
	{ "VOLUME", CBMBASICTokenID::Volume },
	{ "WAIT", CBMBASICTokenID::Wait },
	{ "WHILE", CBMBASICTokenID::While },
	{ "WIDTH", CBMBASICTokenID::Width },
	{ "XOR", CBMBASICTokenID::XOr }
};

CBMBASICLexer::CBMBASICLexer(const char *s):
	m_lastState(s),
	m_state(s)
{
}

CBMBASICToken CBMBASICLexer::Expect(char expected, CBMBASICTokenID id)
{
	if (m_state.Get() == expected) return Simple(id);
	else
		return Error("Expected '", expected, "', got '", m_state.Get(), '\'');
}

bool CBMBASICLexer::HasMore() const
{
	return m_state.HasMore();
}

CBMBASICToken CBMBASICLexer::Identifier()
{
	std::string ident(1, std::toupper(m_state.Get()));

	while (ValidCBMBASICIdentifier(m_state.Next()))
		ident += std::toupper(m_state.Get());

	// suffix?
	if (m_state.Get() == '#' || m_state.Get() == '$' || m_state.Get() == '%')
	{
		ident += m_state.Get();
		m_state.Advance();
	}

	// keyword
	for (auto &[k, v] : Keywords)
		if (std::strcmp(ident.c_str(), k) == 0)
			return Simple(v);

	// constants
	if (ident == "PI") || ident == "π") return MakeToken(CBMBASICTokenID::FloatLiteral, 3.14159265);

	// line comment
	if (ident == "REM")
	{
		LineComment();
		NextToken();
	}

	return MakeToken(CBMBASICTokenID::Identifier, std::move(ident));
}

void CBMBASICLexer::LineComment()
{
	while (!m_state.IsNewLine())
		m_state.Advance();
}

CBMBASICToken CBMBASICLexer::MakeToken(CBMBASICTokenID id, CBMBASICTokenValue value) const
{
	return { id, m_state.Line(), m_state.Column(), m_state.Line(), std::move(value) };
}

CBMBASICToken CBMBASICLexer::NextToken()
{
	m_state.SkipWhitespace();

	m_lastState = m_state;
	char c = std::toupper(m_state.Get());

	switch (c)
	{
		case 0: return MakeToken(CBMBASICTokenID::EndOfInput);
		case '*': return Simple(CBMBASICTokenID::Asterisk);
		case '=': return Simple(CBMBASICTokenID::Equal);
		case '>':
			switch (m_state.Next())
			{
				case '=': return Simple(CBMBASICTokenID::GreaterEqual);
				default: return Simple(CBMBASICTokenID::Greater);
			}
		case '<':
			switch (m_state.Next())
			{
				case '=': return Simple(CBMBASICTokenID::GreaterEqual);
				case '>': return Simple(CBMBASICTokenID::Not);
				default: return Simple(CBMBASICTokenID::Greater);
			}
		case '-':
			if (std::isdigit(m_state.Next()))
				return Number(true);
			return Simple(CBMBASICTokenID::Minus);
		case '(': return Simple(CBMBASICTokenID::ParenthesisLeft);
		case ')': return Simple(CBMBASICTokenID::ParenthesisRight);
		case '+':
			if (std::isdigit(m_state.Next()))
				return Number(false);
			return Simple(CBMBASICTokenID::Plus);
		case ';': return Simple(CBMBASICTokenID::Semicolon);
		case '/': return Simple(CBMBASICTokenID::Slash);
		default:
			if (std::isalpha(c) || c == '_') return Identifier(false);
			if (std::isdigit(c)) return Number(false);
			return Error("Unexpected character: '", c, '\'');
	}
}

CBMBASICToken CBMBASICLexer::Number(bool negative)
{
	bool fp = false;

	while (std::isdigit(m_state.Next())) ;

	// float?
	if (m_state.Get() == '.')
	{
		fp = true;
		while (std::isdigit(m_state.Next())) ;
	}

	if (std::isalpha(m_state.Get()) || m_state.Get() == '_')
		return Error("Number literal holds invalid characters");

	if (fp)
	{
		double f;
		auto result = std::from_chars(m_lastState.Ptr(), m_state.Ptr(), f);

		if (result.ec == std::errc::result_out_of_range)
			return Error("Max value exceeded");

		return MakeToken(CBMBASICTokenID::FloatLiteral, negative ? -f : f);
	}
	else
	{
		int n;

		auto result = std::from_chars(m_lastState.Ptr(), m_state.Ptr(), n);

		if (result.ec == std::errc::result_out_of_range)
			return Error("Max value exceeded");

		return MakeToken(CBMBASICTokenID::IntegerLiteral, negative ? -n : n);
	}
}

constexpr bool ValidCBMBASICIdentifier(char c)
{
	return std::isalnum(c) || c == '_';
}
