#include <utility>

#include "assembler.h"
#include "../../core/utility.h"

Assembler6500::Assembler6500(const char *source):
	m_lex(Lexer6500(source))
{
}

Assembler6500::~Assembler6500()
{
}

/*void Assembler6500::CodeGen(std::ostream &logOut)
{
	auto it = m_ast.begin();

	while (it != m_ast.end())
	{
		switch (it->ID)
		{
			case Token6500ID::Error:
				logOut << std::get<std::string>(it->Value) << '\n';
				it++;
				break;

			case Token6500ID::Offset:
				it += 2;
				break;

			case Token6500ID::Byte:
				it++;
				while (it->ID == Token6500ID::IntegerLiteral)
				{
					EmitByte(static_cast<uint8_t>(std::get<uint16_t>(it->Value)));
					it++;
				}
				break;

			case Token6500ID::Text:
				it++
				for (char c : std::get<std::string>(it->Value))
					EmitByte(static_cast<uint8_t>(c));
				it++;
				break;

			case Token6500ID::BCC:
				EmitByte(144);
				it++;
				break;

			case Token6500ID::BCS:
				EmitByte(176);
				it++;
				break;

			case Token6500ID::BEQ:
				EmitByte(240);
				it++;
				break;

			case Token6500ID::BMI:
				EmitByte(48);
				it++;
				break;

			case Token6500ID::BNE:
				EmitByte(208);
				it++;
				break;

			case Token6500ID::BPL:
				EmitByte(16);
				it++;
				break;

			case Token6500ID::BRK:
				EmitByte(0);
				it++;
				break;

			case Token6500ID::BVC:
				EmitByte(80);
				it++;
				break;

			case Token6500ID::BVS:
				EmitByte(112);
				it++;
				break;

			case Token6500ID::CLC:
				EmitByte(24);
				it++;
				break;

			case Token6500ID::CLD:
				EmitByte(216);
				it++;
				break;

			case Token6500ID::CLI:
				EmitByte(88);
				it++;
				break;

			case Token6500ID::CLV:
				EmitByte(184);
				it++;
				break;

			case Token6500ID::DEX:
				EmitByte(202);
				it++;
				break;

			case Token6500ID::DEY:
				EmitByte(136);
				it++;
				break;

			case Token6500ID::INX:
				EmitByte(232);
				it++;
				break;

			case Token6500ID::INY:
				EmitByte(200);
				it++;
				break;

			case Token6500ID::PHA:
				EmitByte(72);
				it++;
				break;

			case Token6500ID::PHP:
				EmitByte(8);
				it++;
				break;

			case Token6500ID::PLA:
				EmitByte(104);
				it++;
				break;

			case Token6500ID::PLP:
				EmitByte(40);
				it++;
				break;

			case Token6500ID::SEC:
				EmitByte(56);
				it++;
				break;

			case Token6500ID::SED:
				EmitByte(248);
				it++;
				break;

			case Token6500ID::SEI:
				EmitByte(120);
				it++;
				break;

			case Token6500ID::TAX:
				EmitByte(170);
				it++;
				break;

			case Token6500ID::TAY:
				EmitByte(168);
				it++;
				break;

			case Token6500ID::TSX:
				EmitByte(186);
				it++;
				break;

			case Token6500ID::TXA:
				EmitByte(138);
				it++;
				break;

			case Token6500ID::TXS:
				EmitByte(154);
				it++;
				break;

			case Token6500ID::TYA:
				EmitByte(152);
				it++;
				break;

			default:
				logOut << it->Line << ',' << it->Column << ": Unsupported syntax or illegal opcode\n";
				it++;
		}
	}
}*/

void Assembler6500::EmitByte(uint8_t data)
{
	m_gen.push_back(data);
}

void Assembler6500::EmitWord(uint16_t data)
{
	EmitByte(data & 255);
	EmitByte(Hi16(data) >> 8);
}

void Assembler6500::Expression(Token6500 token)
{
	static bool inParen = false;

	switch (token.ID)
	{
		case Token6500ID::ParenthesisLeft:
			m_ast.push_back(std::move(token));
			inParen = true;
			Expression(m_lex.NextToken());
			break;

		case Token6500ID::ParenthesisRight:
			if (!inParen)
			{
				m_ast.push_back(m_lex.Error("Unexpected closing parenthesis"));
				return;
			}

			if (m_ast.back().ID == Token6500ID::ParenthesisLeft)
			{
				m_ast.push_back(m_lex.Error("Empty indirect expression"));
				return;
			}

			m_ast.push_back(std::move(token));
			inParen = false;
			break;

		case Token6500ID::Value:
			m_ast.push_back(std::move(token));
			token = m_lex.NextToken();
			if (token.ID == Token6500ID::IntegerLiteral || token.ID == Token6500ID::Identifier)
				m_ast.push_back(std::move(token));
			else
				m_ast.push_back(m_lex.Error("Expected integer literal or identifier, got: ",
					Token6500IDString(token.ID)));

			if (inParen) Expression(m_lex.NextToken());
			break;

		case Token6500ID::Identifier:
		case Token6500ID::IntegerLiteral:
		case Token6500ID::X:
		case Token6500ID::Y:
			m_ast.push_back(std::move(token));
			if (inParen) Expression(m_lex.NextToken());
			break;

		default:
			m_ast.push_back(m_lex.Error("Expected expression, got: ", Token6500IDString(token.ID)));
	}
}

uint16_t Assembler6500::FindLabelOffset(const std::string &label) const
{
	auto it = m_ast.cbegin();

	while (it != m_ast.cend())
	{
		if (it->ID == Token6500ID::Identifier)
		{
			auto ident = it++;
			if (it->ID == Token6500ID::Colon)
				return it->Offset;
		}
	}

	return 0;
}

void Assembler6500::Parse()
{
	do
	{
		Statement(m_lex.NextToken());
	} while (m_lex.HasMore());
}

void Assembler6500::ResolveSymbols()
{
	auto it = m_ast.begin();
	uint16_t globalOffset = 0;
	uint16_t relOffset = 0;

	while (it != m_ast.end())
	{
		switch (it->ID)
		{
			case Token6500ID::Identifier:
			{
				it->Offset = globalOffset + relOffset;
				relOffset++;
				auto ident = it++;
				if (it->ID == Token6500ID::Colon)
					m_syms[std::get<std::string>(ident->Value)] = globalOffset + relOffset;
				break;
			}

			case Token6500ID::Offset:
				it->Offset = globalOffset + relOffset;
				relOffset++;
				it++;
				switch (it->ID)
				{
					case Token6500ID::IntegerLiteral:
						globalOffset = std::get<uint16_t>(it->Value);
						break;

					case Token6500ID::Identifier:
						if (m_syms.contains(std::get<std::string>(it->Value)))
							globalOffset = m_syms[std::get<std::string>(it->Value)];
						else
							globalOffset = FindLabelOffset(std::get<std::string>(it->Value));

					default: ;
				}
				relOffset = 0;
				break;

			default: ;
		}

		it->Offset = globalOffset + relOffset;
		relOffset++;
		it++;
	}
}

void Assembler6500::Statement(Token6500 token)
{
	switch (token.ID)
	{
		case Token6500ID::EndOfInput:
			break;

		case Token6500ID::Error:
			m_ast.push_back(std::move(token));
			break;

		// offset ::= '*=' integer literal | identifier
		case Token6500ID::Offset:
			m_ast.push_back(std::move(token));
			token = m_lex.NextToken();
			switch (token.ID)
			{
				case Token6500ID::IntegerLiteral:
					m_ast.push_back(std::move(token));
					break;
				case Token6500ID::Identifier:
					m_ast.push_back(std::move(token));
					break;
				default:
					m_lex.Error("Offset specified without a valid value");
					m_ast.push_back(std::move(token));
			}
			break;

		case Token6500ID::Identifier:
			m_ast.push_back(std::move(token));
			m_ast.push_back(m_lex.Expect(':', Token6500ID::Colon));
			break;

		// byte ::= ['.'] ('BY' | 'BYTE' | 'DB')
		case Token6500ID::Byte:
			m_ast.push_back(std::move(token));
			token = m_lex.NextToken();
			if (token.ID == Token6500ID::IntegerLiteral || token.ID == Token6500ID::Identifier)
			{
				while (token.ID == Token6500ID::IntegerLiteral || token.ID == Token6500ID::Identifier)
				{
					m_ast.push_back(std::move(token));

					token = m_lex.Expect(',', Token6500ID::Comma);
					if (token.ID == Token6500ID::Error)
						break;

					token = m_lex.NextToken();
				}
			}
			else
				m_ast.push_back(m_lex.Error("Expected byte literal or identifier, got: ",
					Token6500IDString(token.ID)));
			break;

		// text ::= ['.'] ('ASCIIZ', 'AZ', 'TX') string literal
		case Token6500ID::Text:
			m_ast.push_back(std::move(token));
			token = m_lex.NextToken();
			if (token.ID == Token6500ID::StringLiteral)
				m_ast.push_back(std::move(token));
			else
				m_ast.push_back(m_lex.Error("Expected string literal, got: ", Token6500IDString(token.ID)));
			break;

		// implied instructions
		case Token6500ID::BRK:
		case Token6500ID::CLC:
		case Token6500ID::CLD:
		case Token6500ID::CLI:
		case Token6500ID::CLV:
		case Token6500ID::DEX:
		case Token6500ID::DEY:
		case Token6500ID::INX:
		case Token6500ID::INY:
		case Token6500ID::PHA:
		case Token6500ID::PHP:
		case Token6500ID::PLA:
		case Token6500ID::PLP:
		case Token6500ID::RTI:
		case Token6500ID::RTS:
		case Token6500ID::SEC:
		case Token6500ID::SED:
		case Token6500ID::SEI:
		case Token6500ID::TAX:
		case Token6500ID::TAY:
		case Token6500ID::TSX:
		case Token6500ID::TXA:
		case Token6500ID::TXS:
		case Token6500ID::TYA:
			m_ast.push_back(std::move(token));
			break;

		// instructions with operands
		case Token6500ID::ADC:
		case Token6500ID::AND:
		case Token6500ID::ASL:
		case Token6500ID::BCC:
		case Token6500ID::BCS:
		case Token6500ID::BEQ:
		case Token6500ID::BIT:
		case Token6500ID::BMI:
		case Token6500ID::BNE:
		case Token6500ID::BPL:
		case Token6500ID::BVC:
		case Token6500ID::BVS:
		case Token6500ID::CMP:
		case Token6500ID::CPX:
		case Token6500ID::CPY:
		case Token6500ID::DEC:
		case Token6500ID::EOR:
		case Token6500ID::INC:
		case Token6500ID::JMP:
		case Token6500ID::JSR:
		case Token6500ID::LDA:
		case Token6500ID::LDX:
		case Token6500ID::LDY:
		case Token6500ID::LSR:
		case Token6500ID::ORA:
		case Token6500ID::ROL:
		case Token6500ID::ROR:
		case Token6500ID::SBC:
		case Token6500ID::STA:
		case Token6500ID::STX:
		case Token6500ID::STY:
			m_ast.push_back(std::move(token));
			Expression(m_lex.NextToken());
			break;
	}
}
