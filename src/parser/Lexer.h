#pragma once

#include <string>
#include "../common/Types.h"

enum class TokenType {
    // language keywords
    KW_DEF,
    KW_DATATYPE,
    KW_STRUCT,
    KW_IF,
    KW_ELSE,
    KW_ELSIF,
    KW_FOR,
    KW_WHILE,
    KW_RETURN,
    KW_CONST,

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
    KW_ASSIGNOP,
    KW_SUBASSIGN,
    // -=
    KW_ADDASSIGN,
    // +=
    KW_DOT,

    // user-defined
    IDENTIFIER,
    NUMBER,
    CHAR,
    BOOL,

    // other
    EOFTOK,
    INVALID_TOK
};

enum class BinOp {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_AND,
    OP_OR,
    OP_EQL,
    OP_NEQL,
    OP_LT,
    OP_LTE,
    // less than / equal
    OP_GT,
    OP_GTE // greater than / equal
};

class Lexer {
public:
    explicit Lexer(std::istream& reader);
    TokenType readNextToken();

    TokenType getCurToken() const;
    long getNumValue() const;
    bool getBoolValue() const;
    std::string getStrValue() const;
    std::string getCurSymbol() const;
    DataType getDataType() const;
    BinOp getBinOp() const;
private:
    TokenType getTok();

    std::istream& reader;
    int LastChar;
    TokenType CurToken;
    long NumValue;
    bool BoolValue;
    std::string StrValue;
    std::string CurSymbol;
    DataType DtValue;
    BinOp BinValue;
};
