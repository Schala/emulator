#include <utility>

#include "assembler.h"

Assembler6500::Assembler6500(const char *source):
	m_lex(Lexer6500(source))
{
}

Assembler6500::~Assembler6500()
{
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
			m_ast.push_back(std::move(token));
			if (inParen) Expression(m_lex.NextToken());
			break;

		default:
			m_ast.push_back(m_lex.Error("Expected expression, got: ", Token6500IDString(token.ID)));
	}
}

void Assembler6500::Statement(Token6500 token)
{
	switch (token.ID)
	{
		case Token6500ID::EndOfInput:
		case Token6500ID::Error:
			break;

		// offset ::= '*=' integer literal | identifier
		case Token6500ID::Offset:
			token = m_lex.NextToken();
			if (token.ID == Token6500ID::IntegerLiteral || token.ID == Token6500ID::Identifier)
				m_ast.push_back(std::move(token));
			else
				m_lex.Error("Offset specified without a valid value");
			break;

		// byte ::= ['.'] ('BY' | 'BYTE' | 'DB')
		case Token6500ID::Byte:
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

void Assembler6500::Parse()
{
	do
	{
		Statement(m_lex.NextToken());
	} while (m_lex.HasMore());
}
