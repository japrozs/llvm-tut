#include "scanner.h"

Scanner::Scanner(std::string source) : m_source(std::move(source))
{
	/* clang-format off */
    m_keywords = {
        {"and", TokenType::And},
        {"class", TokenType::Class},
        {"else", TokenType::Else},
        {"false", TokenType::False},
        {"for", TokenType::For},
        {"fn", TokenType::Fn},
        {"if", TokenType::If},
        {"null", TokenType::Null},
        {"or", TokenType::Or},
        {"print", TokenType::Print},
        {"return", TokenType::Return},
        {"super", TokenType::Super},
        {"this", TokenType::This},
        {"true", TokenType::True},
        {"let", TokenType::Let},
        {"while", TokenType::While},
    };
}

std::vector<Token> Scanner::scan_tokens()
{
	while (!is_at_end())
	{
		m_start = m_current;
		scan_token();
	}
	m_tokens.emplace_back(TokenType::TokenEOF, "", m_line);
	return m_tokens;
}

bool Scanner::is_at_end() const
{
	return m_current >= static_cast<int>(m_source.size());
}

char Scanner::advance()
{
	++m_current;
	return m_source.at(m_current - 1);
}

bool Scanner::match(char expected)
{
	if (is_at_end())
	{
		return false;
	}
	if (m_source.at(m_current) != expected)
	{
		return false;
	}

	++m_current;
	return true;
}

char Scanner::peek() const
{
	if (is_at_end())
	{
		return '\0';
	}
	return m_source.at(m_current);
}

char Scanner::peek_next() const
{
	if (m_current + 1 >= static_cast<int>(m_source.size()))
	{
		return '\0';
	}
	return m_source.at(m_current + 1);
}

void Scanner::add_token(TokenType type, std::any literal)
{
	m_tokens.emplace_back(type, m_source.substr(m_start, m_current - m_start), literal, m_line);
}

void Scanner::add_token(TokenType type)
{
	m_tokens.emplace_back(type, m_source.substr(m_start, m_current - m_start), m_line);
}

void Scanner::comment()
{
	// A comment goes until the end of the line.
	while (peek() != '\n' && !is_at_end())
	{
		advance();
	}
}

void Scanner::string()
{
	while (peek() != '"' && !is_at_end())
	{
		if (peek() == '\n')
		{
			++m_line;
		}
		advance();
	}

	// Unterminated string
	if (is_at_end())
	{
		std::cout << "line[" << m_line << "] : Unterminated string \n";
	}

	advance(); // the closing '"'

	// Trim the surrounding quotes
	add_token(TokenType::String, m_source.substr(m_start + 1, m_current - m_start - 2));
}

void Scanner::number()
{
	while (std::isdigit(peek()))
	{
		advance();
	}

	// Look for a fractional part.
	if (peek() == '.' && std::isdigit(peek_next()))
	{
		// Consume the "."
		advance();

		while (std::isdigit(peek()))
		{
			advance();
		}
	}

	add_token(TokenType::Number, std::stod(m_source.substr(m_start, m_current - m_start)));
}

void Scanner::identifier()
{
	while (std::isdigit(peek()) || std::isalpha(peek()) || peek() == '_')
	{
		advance();
	}

	const auto text = m_source.substr(m_start, m_current - m_start);
	if (const auto it = m_keywords.find(text); it != m_keywords.end())
	{
		add_token(it->second);
	}
	else
	{
		add_token(TokenType::Identifier);
	}
}

void Scanner::scan_token()
{
	auto c = advance();
	switch (c)
	{
	case '(':
		add_token(TokenType::LeftParen);
		break;
	case ')':
		add_token(TokenType::RightParen);
		break;
	case '{':
		add_token(TokenType::LeftBrace);
		break;
	case '}':
		add_token(TokenType::RightBrace);
		break;
	case ',':
		add_token(TokenType::Comma);
		break;
	case '.':
		add_token(TokenType::Dot);
		break;
	case '-':
		add_token(TokenType::Minus);
		break;
	case '+':
		add_token(TokenType::Plus);
		break;
	case ';':
		add_token(TokenType::Semicolon);
		break;
	case ':':
		add_token(TokenType::Colon);
		break;
	case '*':
		add_token(TokenType::Star);
		break;
	case '!':
		add_token(match('=') ? TokenType::BangEqual : TokenType::Bang);
		break;
	case '=':
		add_token(match('=') ? TokenType::EqualEqual : TokenType::Equal);
		break;
	case '<':
		add_token(match('=') ? TokenType::LessEqual : TokenType::Less);
		break;
	case '>':
		add_token(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
		break;
	case '/':
		if (match('/'))
		{
			comment();
		}
		else
		{
			add_token(TokenType::Slash);
		}
		break;
	case ' ':
	case '\r':
	case '\t':
		// Ignore whitespace.
		break;
	case '\n':
		m_line++;
		break;
	case '"':
		string();
		break;

	default:
		if (std::isdigit(c))
		{
			number();
		}
		else if (std::isalpha(c) || c == '_')
		{
			identifier();
		}
		else
		{
			std::cout << "line[" << m_line << "] : Unexpected chracter '" << c << "'\n";
		}
		break;
	}
}