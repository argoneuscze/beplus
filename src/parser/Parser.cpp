#include <string> // string concat - error output
#include "Parser.h"

#include "Util.h"

Parser::Parser(std::istream& input) {
    Lexer = std::make_unique<::Lexer>(input);
}

// TODO Change parse() to return a Module
// TODO more possible parsing options? toplevelstatement?
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

    if (Lexer->getCurToken() != TokenType::KW_LEFTCURLYBRACKET) {
        std::string err = "Expected '{', instead got: " + Lexer->getStrValue();
        throw ParserException(err);
    }
    Lexer->readNextToken(); // eat '{'

    // parse the following block
    auto block = parseBlock();

    if (Lexer->getCurToken() != TokenType::KW_RIGHTCURLYBRACKET) {
        std::string err = "Expected '}', instead got: " + Lexer->getStrValue();
        throw ParserException(err);
    }
    Lexer->readNextToken(); // eat '}'

    auto fn = std::make_unique<ASTFunction>(std::move(proto), std::move(block));

    ASTRoot.push_back(std::move(fn));
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

    return std::make_unique<ASTFunctionPrototype>(name, std::move(args), type);
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

// DECL ::= DATATYPE IDENT
std::unique_ptr<ASTStatementDecl> Parser::parseDecl(void) {
    std::string err;
    LogDebug("Parsing a declaration");

    auto type = Lexer->getDataType(); // datatype already checked by caller
    Lexer->readNextToken(); // eat DATATYPE

    if (Lexer->getCurToken() != TokenType::IDENTIFIER) {
        err = "Expected an identifier, instead got: " + Lexer->getStrValue();
        throw ParserException(err);
    } 
    
    std::string ident = Lexer->getStrValue();
    Lexer->readNextToken(); // eat IDENT

    return std::make_unique<ASTStatementDecl>(type, ident.c_str());
}

// CALL ::= ARGS* ')'
// ident passed as an argument due to the parser needing to see the bracket
// to decide whether the statement really is a function call.
// possible TODO - check the module whether the function prototype actually
//                 exists and can be satisfied with this call?
std::unique_ptr<ASTStatementCall> Parser::parseCall(const std::string ident) {
    std::string err;
    LogDebug("Parsing a call");

    std::vector<std::unique_ptr<ASTExpression>> args;
    Lexer->readNextToken(); // eat '('

    if (Lexer->getCurToken() == TokenType::KW_RIGHTBRACKET)
        return std::make_unique<ASTStatementCall>(ident, std::move(args));

    // ARGS ::= EXPR , EXPR , ...
    while(true) {
        auto arg = parseExpression();
        args.push_back(std::move(arg));

        switch(Lexer->getCurToken()) {
        case TokenType::KW_COMMA:
            Lexer->readNextToken(); // eat ',' 
            continue;
        case TokenType::KW_RIGHTBRACKET:
            Lexer->readNextToken(); // eat ')'
            return std::make_unique<ASTStatementCall>(ident, std::move(args));
        default:
            err = "Expected ',' or ')', instead got: " + Lexer->getStrValue();
            throw ParserException(err);
        }
    }

    throw ParserException("parseCall() should never get here.");
}

// TODO implement all possible statements
// statement ::= DECL ';'
// statement ::= ASSIGN ';'
// statement ::= CALL ';'
// statement ::= EXPR ';'
// Not possible to perform this as a switch/case sequence as decl + assignment
// is necessary inside of the if block.
std::unique_ptr<ASTStatement> Parser::parseStatement(void) {
    LogDebug("Parsing a statement");
    std::string err;
    std::unique_ptr<ASTStatement> stmt;

    // DECL ::= DATATYPE IDENT ';'
    if (Lexer->getCurToken() == TokenType::KW_DATATYPE) { 
        stmt = parseDecl();
    }
    // CALL ::= IDENT '('
    // ASSIGN ::= IDENT '='
    // EXPR ::= IDENT BINOP
    else if (Lexer->getCurToken() == TokenType::IDENTIFIER) {
        std::string ident = Lexer->getStrValue();
        Lexer->readNextToken(); // eat IDENT
        if (Lexer->getCurToken() == TokenType::KW_LEFTBRACKET)
            stmt = parseCall(ident);
        // TODO else if other statements starting with ident ...
    }
    // TODO else if other statements ...

    if (Lexer->getCurToken() != TokenType::KW_SEMICOLON) {
        err = "Expected ';' at the end of a statement, instead got: " + \
               Lexer->getStrValue();
        throw ParserException(err);
    }
    Lexer->readNextToken(); // eat ';'

    return stmt;
}

// BLOCK ::= STATEMENT*
// ends with a '}' but eating '}' should be done by the function caller
std::unique_ptr<ASTBlock> Parser::parseBlock() {
    LogDebug("Parsing a block");
    std::vector<std::unique_ptr<ASTNode>> vec;

    while (true) {
        if (Lexer->getCurToken() == TokenType::KW_RIGHTCURLYBRACKET) {
            auto block = std::make_unique<ASTBlock>(std::move(vec));
            return block;
        }
        auto astnode = parseStatement();
        vec.push_back(std::move(astnode));
    }

    throw ParserException("parseBlock() should never get here");
}

BinOpPrecedence Parser::OpPrecedence = BinOpPrecedence();
