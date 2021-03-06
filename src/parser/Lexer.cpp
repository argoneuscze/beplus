#include "Lexer.h"
#include <iostream>

Lexer::Lexer(std::istream& reader): reader(reader), LastChar(' '), CurToken(), NumValue(0), BoolValue(false), DtValue(),
                                    BinValue() {
}

TokenType Lexer::readNextToken() {
    CurToken = getTok();
    return CurToken;
}

TokenType Lexer::getTok() {
    bool isNeg = false; // unary minus TODO

    // skip all whitespaces
    while (isspace(LastChar))
        LastChar = reader.get();

    // check for identifiers and keywords
    if (isalpha(LastChar)) {
        StrValue.clear();
        StrValue += LastChar;
        while (isalnum(LastChar = reader.get()))
            StrValue += LastChar;

        CurSymbol = StrValue;
        auto LowerCase = StrValue;
        for (size_t i = 0; i < LowerCase.size(); ++i)
            LowerCase[i] = tolower(StrValue[i]);

        if (LowerCase == "or") {
            BinValue = BinOp::OP_OR;
            return TokenType::KW_BINARYOP;
        }
        if (LowerCase == "and") {
            BinValue = BinOp::OP_AND;
            return TokenType::KW_BINARYOP;
        }
        if (LowerCase == "else")
            return TokenType::KW_ELSE;
        if (LowerCase == "elsif")
            return TokenType::KW_ELSIF;
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
        if (LowerCase == "const")
            return TokenType::KW_CONST;
        if (LowerCase == "struct") {
            DtValue = DataType::DT_STRUCT;
            return TokenType::KW_DATATYPE;
        }
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
        if (LowerCase == "bool") {
            DtValue = DataType::DT_BOOL;
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
        CurSymbol = StrValue;
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

    if (LastChar == EOF) {
        CurSymbol = "EOF"; // set to "EOF" just for better error messages
        return TokenType::EOFTOK;
    }

    auto ThisChar = LastChar;
    CurSymbol = ThisChar;
    LastChar = reader.get();
    switch (ThisChar) {
    case '!':
        if (LastChar == '=') {
            // '!=' instead of a unary NOT
            LastChar = reader.get();
            BinValue = BinOp::OP_NEQL;
            return TokenType::KW_BINARYOP;
        }
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
    case '.':
        return TokenType::KW_DOT;
    case '=':
        if (LastChar == '=') {
            // '==' instead of '='
            LastChar = reader.get();
            BinValue = BinOp::OP_EQL;
            return TokenType::KW_BINARYOP;
        }
        return TokenType::KW_ASSIGNOP;
    case '<':
        if (LastChar == '=') {
            // '<= instead of '<'
            LastChar = reader.get();
            BinValue = BinOp::OP_LTE;
            return TokenType::KW_BINARYOP;
        }
        BinValue = BinOp::OP_LT;
        return TokenType::KW_BINARYOP;
    case '>':
        if (LastChar == '=') {
            // '>=' instead of '<'
            LastChar = reader.get();
            BinValue = BinOp::OP_GTE;
            return TokenType::KW_BINARYOP;
        }
        BinValue = BinOp::OP_GT;
        return TokenType::KW_BINARYOP;
    case '+':
        if (LastChar == '=') {
            // '+='
            LastChar = reader.get();
            return TokenType::KW_ADDASSIGN;
        }
        BinValue = BinOp::OP_ADD;
        return TokenType::KW_BINARYOP;
    case '-':
        if (LastChar == '=') {
            // '-='
            LastChar = reader.get();
            return TokenType::KW_SUBASSIGN;
        }
        BinValue = BinOp::OP_SUB;
        return TokenType::KW_BINARYOP;
    case '*':
        BinValue = BinOp::OP_MUL;
        return TokenType::KW_BINARYOP;
    case '/':
        BinValue = BinOp::OP_DIV;
        return TokenType::KW_BINARYOP;
    default:
        break;
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

std::string Lexer::getCurSymbol() const {
    return this->CurSymbol;
}

DataType Lexer::getDataType() const {
    return this->DtValue;
}

BinOp Lexer::getBinOp() const {
    return this->BinValue;
}
