#pragma once

#include <string>

enum class token_type {
	// language keywords
	KW_DEF,
	KW_DATATYPE,
	KW_IF,
	KW_FOR,
	KW_WHILE,
	KW_RETURN,

	// syntax
	KW_LEFTBRACKET,
	KW_RIGHTBRACKET,
	KW_LEFTCURLYBRACKET,
	KW_RIGHTCURLYBRACKET,
	KW_LEFTSQUAREBRACKET,
	KW_RIGHTSQUAREBRACKET,
	KW_SEMICOLON,
	KW_COLON,
	KW_COMMA,
	KW_BINARYOP,

	// user-defined
	IDENTIFIER,
	NUMBER,
	CHAR,
	BOOL,

	// other
	EOFTOK
};

enum class data_type {
	DT_VOID,
	DT_INT,
	DT_CHAR
};

enum class bin_op {
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV
};

class Lexer {
public:
private:
	long num_value;
	bool bool_value;
	std::string str_value;
};
