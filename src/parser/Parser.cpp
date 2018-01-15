#include <string> // string concat - error output
#include "Parser.h"

#include "Util.h"

Parser::Parser(std::istream& input) {
    Lexer = std::make_unique<::Lexer>(input);
}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
    std::string err; // have to declare before switch
    LogDebug("Parsing started");

    // read first token
    Lexer->readNextToken();

    try {
        while (true) {
            switch (Lexer->getCurToken()) {
                // TODO more tokens
            case TokenType::KW_SEMICOLON:
                LogDebug("Found stray ';', skipping");
                Lexer->readNextToken();
                break;
            case TokenType::KW_DEF:
                LogDebug("Function definition found");
                handleFunction();
                break;
            case TokenType::EOFTOK:
                LogDebug("EOF encountered");
                return std::move(ASTRoot);
            case TokenType::INVALID_TOK:
                // LogDebug("Invalid token found"); // no need to logdebug
                err = "Invalid token found: " + Lexer->getStrValue();
                throw ParserException(err);
            default:
                LogDebug("Parsing top level expression");
                handleTopLevelExpression();
                break;
            }
        }
    }
    catch (ParserException& ex) {
        LogParsingError(ex.what());
    }

    return std::move(ASTRoot);
}

void Parser::handleTopLevelExpression() {
    auto expr = parseExpression();

    LogDebug(expr.get());

    if (Lexer->getCurToken() != TokenType::KW_SEMICOLON) {
        std::string err = "Expected a ';' at the end of an expression, \
instead got " + Lexer->getStrValue();
        throw ParserException(err);
    }

    Lexer->readNextToken(); // eat ';'

    ASTRoot.push_back(std::move(expr));
}

int Parser::getTokPrecedence() const {
    if (Lexer->getCurToken() != TokenType::KW_BINARYOP)
        return -1;
    return OpPrecedence.get(Lexer->getBinOp());
}

std::unique_ptr<ASTExpression> Parser::parseExpression() {
    LogDebug("Parsing expression");

    auto primary = parsePrimary();

    return parseBinOpRHS(0, std::move(primary));
}

std::unique_ptr<ASTExpression> Parser::parsePrimary() {
    switch (Lexer->getCurToken()) {
    case TokenType::NUMBER:
        return parseNumberExpression();
    case TokenType::KW_LEFTBRACKET:
        return parseParenthesisExpression();
        // TODO more token types
    default:
        std::string err = "Unknown token " + Lexer->getStrValue() + " found \
when parsing expression.";
        throw ParserException(err);
    }
}

std::unique_ptr<ASTExpression> Parser::parseBinOpRHS(const int prec, std::unique_ptr<ASTExpression> LHS) {
    LogDebug("Parsing binary operation");

    while (true) {
        auto tokPrec = getTokPrecedence();

        if (tokPrec < prec)
            return LHS;

        auto binOp = Lexer->getBinOp();
        Lexer->readNextToken(); // eat BinOp

        auto RHS = parsePrimary();

        auto nextPrec = getTokPrecedence();
        if (tokPrec < nextPrec)
            RHS = parseBinOpRHS(tokPrec + 1, std::move(RHS));

        LHS = std::make_unique<ASTExpressionBinOp>(std::move(LHS), std::move(RHS), binOp);
    }
}

std::unique_ptr<ASTExpressionNumber> Parser::parseNumberExpression() {
    LogDebug("Parsing number");

    auto num = std::make_unique<ASTExpressionNumber>(Lexer->getNumValue());
    Lexer->readNextToken(); // eat number
    return num;
}

std::unique_ptr<ASTExpression> Parser::parseParenthesisExpression() {
    LogDebug("Parsing a parenthesis expression");

    Lexer->readNextToken(); // eat '('

    auto expr = parseExpression();

    if (Lexer->getCurToken() != TokenType::KW_RIGHTBRACKET) {
        std::string err = "Expected ')', instead got: " + Lexer->getStrValue();
        throw ParserException(err);
    }
    Lexer->readNextToken(); // eat ')'

    return expr;
}

// function ::= 'def' prototype '{' statement* '}'
void Parser::handleFunction() {
    Lexer->readNextToken(); // eat def

    auto proto = parsePrototype();

    // parse the following block
    auto block = parseBlock();
}

// prototype ::= identifier '(' arguments ')' ':' datatype
std::unique_ptr<ASTFunctionPrototype> Parser::parsePrototype() {
    LogDebug("Parsing function prototype");

    auto name = Lexer->getStrValue();
    Lexer->readNextToken(); // eat identifier

    if (Lexer->getCurToken() != TokenType::KW_LEFTBRACKET) {
        std::string err = "Expected '(', instead got: " + Lexer->getStrValue();
        throw ParserException(err);
    }
    Lexer->readNextToken(); // eat '('

    auto args = parseArguments();
    Lexer->readNextToken(); // eat ')'

    if (Lexer->getCurToken() != TokenType::KW_COLON) {
        std::string err = "Expected ':', instead got: " + Lexer->getStrValue();
        throw ParserException(err);
    }
    Lexer->readNextToken(); // eat ':'

    if (Lexer->getCurToken() != TokenType::KW_DATATYPE) {
        std::string err = "Expected datatype, instead got: " + Lexer->getStrValue();
        throw ParserException(err);
    }
    auto type = Lexer->getDataType();
    Lexer->readNextToken(); // eat datatype

    throw ParserException("Not implemented yet");
    // TODO make arguments
    //return std::make_unique<ASTFunctionPrototype>()
}

// arguments ::= datatype identifier ',' | ')'
std::vector<std::unique_ptr<ASTArgument>> Parser::parseArguments() {
    std::string err;
    LogDebug("Parsing prototype arguments");

    std::vector<std::unique_ptr<ASTArgument>> args;

    if (Lexer->getCurToken() == TokenType::KW_RIGHTBRACKET)
        return args;

    while (true) {
        if (Lexer->getCurToken() != TokenType::KW_DATATYPE)
            throw ParserException("Expected datatype");
        auto type = Lexer->getDataType();
        Lexer->readNextToken(); // eat datatype

        if (Lexer->getCurToken() != TokenType::IDENTIFIER)
            throw ParserException("Expected identifier");
        auto ident = Lexer->getStrValue();
        Lexer->readNextToken(); // eat identifier

        auto arg = std::make_unique<ASTArgument>(type, ident);
        LogDebug(arg.get());
        args.push_back(std::move(arg));

        switch (Lexer->getCurToken()) {
        case TokenType::KW_COMMA:
            Lexer->readNextToken(); // eat ','
            continue;
        case TokenType::KW_RIGHTBRACKET:
            return args;
        default:
            err = "Expected ',' or ')', instead got: " + Lexer->getStrValue();
            throw ParserException(err);
        }
    }
}

std::unique_ptr<ASTBlock>> Parser::parseBlock() {
  throw ParserException("NOT IMPLEMENTED");
  return nullptr;
}

BinOpPrecedence Parser::OpPrecedence = BinOpPrecedence();
