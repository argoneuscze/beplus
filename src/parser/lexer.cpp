#include "lexer.h"

Lexer::Lexer(std::istream& reader): reader(reader), LastChar(' '), CurToken(), NumValue(0), BoolValue(false), DtValue(), BinValue() {
}

TokenType Lexer::readNextToken() {
	CurToken = getTok();
	return CurToken;
}

TokenType Lexer::getTok() {
	// skip all whitespaces
	while (isspace(LastChar))
		LastChar = reader.get();

	// check for identifiers and keywords
	if (isalpha(LastChar)) {
		StrValue.clear();
		StrValue += LastChar;
		while (isalnum(LastChar = reader.get()))
			StrValue += LastChar;

		auto LowerCase = StrValue;
		for (size_t i = 0; i < LowerCase.size(); ++i)
			LowerCase[i] = tolower(StrValue[i]);

		if (LowerCase == "def")
			return TokenType::KW_DEF;
		if (LowerCase == "if")
			return TokenType::KW_IF;
		if (LowerCase == "while")
			return TokenType::KW_WHILE;
		if (LowerCase == "for")
			return TokenType::KW_FOR;
		if (LowerCase == "return")
			return TokenType::KW_RETURN;

		if (LowerCase == "void") {
			DtValue = DataType::DT_VOID;
			return TokenType::KW_DATATYPE;
		}
		if (LowerCase == "int") {
			DtValue = DataType::DT_INT;
			return TokenType::KW_DATATYPE;
		}
		if (LowerCase == "char") {
			DtValue = DataType::DT_CHAR;
			return TokenType::KW_DATATYPE;
		}

		if (LowerCase == "true") {
			BoolValue = true;
			return TokenType::BOOL;
		}
		if (LowerCase == "false") {
			BoolValue = false;
			return TokenType::BOOL;
		}

		return TokenType::IDENTIFIER;
	}

	// check for integer value
	if (isdigit(LastChar)) {
		StrValue.clear();
		StrValue += LastChar;
		while (isdigit(LastChar = reader.get()))
			StrValue += LastChar;
		NumValue = stol(StrValue);
		return TokenType::NUMBER;
	}

	// check for comment
	if (LastChar == '#') {
		do {
			LastChar = reader.get();
		}
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

		if (LastChar != EOF)
			return getTok();
	}

	if (LastChar == EOF)
		return TokenType::EOFTOK;

	auto ThisChar = LastChar;
	LastChar = reader.get();
	switch (ThisChar) {
	case '(':
		return TokenType::KW_LEFTBRACKET;
	case ')':
		return TokenType::KW_RIGHTBRACKET;
	case '{':
		return TokenType::KW_LEFTCURLYBRACKET;
	case '}':
		return TokenType::KW_RIGHTCURLYBRACKET;
	case '[':
		return TokenType::KW_LEFTSQUAREBRACKET;
	case ']':
		return TokenType::KW_RIGHTSQUAREBRACKET;
	case ';':
		return TokenType::KW_SEMICOLON;
	case ':':
		return TokenType::KW_COLON;
	case ',':
		return TokenType::KW_COMMA;
	case '+':
		BinValue = BinOp::OP_ADD;
		return TokenType::KW_BINARYOP;
	case '-':
		BinValue = BinOp::OP_SUB;
		return TokenType::KW_BINARYOP;
	case '*':
		BinValue = BinOp::OP_MUL;
		return TokenType::KW_BINARYOP;
	case '/':
		BinValue = BinOp::OP_DIV;
		return TokenType::KW_BINARYOP;
	}

	return TokenType::INVALID_TOK;
}

TokenType Lexer::getCurToken() const {
	return this->CurToken;
}

long Lexer::getNumValue() const {
	return this->NumValue;
}

bool Lexer::getBoolValue() const {
	return this->BoolValue;
}

std::string Lexer::getStrValue() const {
	return this->StrValue;
}

DataType Lexer::getDataType() const {
	return this->DtValue;
}

BinOp Lexer::getBinOp() const {
	return this->BinValue;
}
