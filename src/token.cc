#include "token.h"

Token::Token(TokenType type, std::string lexeme, int line) : Token(type, lexeme, std::any{}, line)
{
}

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
{
	this->m_type = type;
	this->m_lexeme = std::move(lexeme);
	this->m_literal = literal;
	this->m_line = line;
}

std::string Token::to_string() const
{
	return std::to_string(static_cast<int>(m_type)) + ", lexeme: '" + m_lexeme + "' , literal: '" +
		   literal_to_string() + "'";
}

std::string Token::literal_to_string() const
{
	switch (m_type)
	{
	case TokenType::String:
		return std::any_cast<std::string>(m_literal);
	case TokenType::Number:
		return std::to_string(std::any_cast<double>(m_literal));
	default:
		return "";
	}
}