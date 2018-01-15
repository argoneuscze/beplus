#pragma once

#include <string>

enum class TokenType {
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
    EOFTOK,
    INVALID_TOK
};

enum class DataType {
    DT_VOID,
    DT_INT,
    DT_CHAR
};

enum class BinOp {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
};

class Lexer {
public:
    explicit Lexer(std::istream& reader);
    TokenType readNextToken();

    TokenType getCurToken() const;
    long getNumValue() const;
    bool getBoolValue() const;
    std::string getStrValue() const;
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
    DataType DtValue;
    BinOp BinValue;
};
