#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

enum class TokenType
{
	// Single-character tokens.
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,
	Comma,
	Dot,
	Minus,
	Plus,
	Semicolon,
	Slash,
	Star,
	Colon,

	// One or two character tokens.
	Bang,
	BangEqual,
	Equal,
	EqualEqual,
	Greater,
	GreaterEqual,
	Less,
	LessEqual,

	// Literals.
	Identifier,
	String,
	Number,

	// Keywords.
	And,
	Class,
	Else,
	False,
	Fn,
	For,
	If,
	Null,
	Or,
	Print,
	Return,
	Super,
	This,
	True,
	Let,
	While,

	TokenEOF
};

#endif // TOKEN_TYPE_H