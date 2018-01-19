#include <string> // string concat - error output
#include <cassert>

#include "Parser.h"

#include "Util.h"

Parser::Parser(std::istream& input) {
    Lexer = std::make_unique<::Lexer>(input);
}

// TODO more possible parsing options? toplevelstatement?
std::unique_ptr<Module> Parser::parse() {
    LogDebug("Parsing started");

    // read first token
    Lexer->readNextToken();

    try {
        while (true) {
            switch (Lexer->getCurToken()) {
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
                return std::make_unique<Module>(std::move(ASTNodes));
            case TokenType::INVALID_TOK:
                Err = "Invalid token found: " + Lexer->getCurSymbol();
                throw ParserException(Err);
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

    return nullptr;
}

/*void Parser::handleTopLevelExpression() {
    auto expr = parseExpression();

    LogDebug(expr.get());

    if (Lexer->getCurToken() != TokenType::KW_SEMICOLON) {
        Err = "Expected a ';' at the end of an expression, \
instead got " + Lexer->getStrValue();
        throw ParserException(Err);
    }

    Lexer->readNextToken(); // eat ';'

    ASTNodes.push_back(std::move(expr));
}*/

// Not checking a ';' at the end of a statement as that's handled
// by the parsing functions.
void Parser::handleTopLevelExpression() {
    auto stmt = parseStatement();
    
    LogDebug(stmt.get());

    ASTNodes.push_back(std::move(stmt));
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

/* Important case: a + 2; OR a;
 * A statement had to decide this is an expression at the binary op.
 * Thus, when it got to this point, curToken is actually '+'
 * and it is necessary to build the binary operator expression
 * from this point - not possible to call parseVariableExpression()
 * due to it eating the current token which we do not want.
 * Fortunately enough, Lexer's StrValue holds an identifier, not
 * the current symbol.
 */
std::unique_ptr<ASTExpression> Parser::parsePrimary() {
    switch (Lexer->getCurToken()) {
    case TokenType::NUMBER:
        return parseNumberExpression();
    case TokenType::KW_LEFTBRACKET:
        return parseParenthesisExpression();
    case TokenType::IDENTIFIER:
        return parseVariableExpression();
    case TokenType::KW_BINARYOP:
    case TokenType::KW_SEMICOLON:
        return parseBinOpRHS(0, std::make_unique<ASTExpressionVariable>(Lexer->getStrValue()));
    default:
        Err = "Unknown token " + Lexer->getCurSymbol() + " found \
when parsing expression.";
        throw ParserException(Err);
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

std::unique_ptr<ASTExpressionVariable> Parser::parseVariableExpression(void) {
    LogDebug("Parsing variable expression");

    auto var = std::make_unique<ASTExpressionVariable>(Lexer->getStrValue());
    Lexer->readNextToken(); // eat ident
    return var;
}

std::unique_ptr<ASTExpression> Parser::parseParenthesisExpression() {
    LogDebug("Parsing a parenthesis expression");

    Lexer->readNextToken(); // eat '('

    auto expr = parseExpression();

    if (Lexer->getCurToken() != TokenType::KW_RIGHTBRACKET) {
        Err = "Expected ')', instead got: " + Lexer->getCurSymbol();
        throw ParserException(Err);
    }
    Lexer->readNextToken(); // eat ')'

    return expr;
}

// function ::= 'def' prototype '{' statement* '}'
void Parser::handleFunction() {
    Lexer->readNextToken(); // eat def

    auto proto = parsePrototype();

    if (Lexer->getCurToken() != TokenType::KW_LEFTCURLYBRACKET) {
        Err = "Expected '{', instead got: " + Lexer->getCurSymbol();
        throw ParserException(Err);
    }
    Lexer->readNextToken(); // eat '{'

    // parse the following block
    auto block = parseBlock();

    if (Lexer->getCurToken() != TokenType::KW_RIGHTCURLYBRACKET) {
        Err = "Expected '}', instead got: " + Lexer->getCurSymbol();
        throw ParserException(Err);
    }
    Lexer->readNextToken(); // eat '}'

    auto fn = std::make_unique<ASTFunction>(std::move(proto), std::move(block));

    ASTNodes.push_back(std::move(fn));
}

// prototype ::= identifier '(' arguments ')' ':' datatype
std::unique_ptr<ASTFunctionPrototype> Parser::parsePrototype() {
    LogDebug("Parsing function prototype");

    auto name = Lexer->getStrValue();
    Lexer->readNextToken(); // eat identifier

    if (Lexer->getCurToken() != TokenType::KW_LEFTBRACKET) {
        Err = "Expected '(', instead got: " + Lexer->getCurSymbol();
        throw ParserException(Err);
    }
    Lexer->readNextToken(); // eat '('

    auto args = parseArguments();
    Lexer->readNextToken(); // eat ')'

    if (Lexer->getCurToken() != TokenType::KW_COLON) {
        Err = "Expected ':', instead got: " + Lexer->getCurSymbol();
        throw ParserException(Err);
    }
    Lexer->readNextToken(); // eat ':'

    if (Lexer->getCurToken() != TokenType::KW_DATATYPE) {
        Err = "Expected datatype, instead got: " + Lexer->getCurSymbol();
        throw ParserException(Err);
    }
    auto type = Lexer->getDataType();
    Lexer->readNextToken(); // eat datatype

    return std::make_unique<ASTFunctionPrototype>(name, std::move(args), type);
}

// arguments ::= datatype identifier ',' | ')'
std::vector<std::unique_ptr<ASTArgument>> Parser::parseArguments() {
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
            Err = "Expected ',' or ')', instead got: " + Lexer->getCurSymbol();
            throw ParserException(Err);
        }
    }
}

// DECL ::= DATATYPE IDENT
std::unique_ptr<ASTStatementDecl> Parser::parseDecl(void) {
    LogDebug("Parsing a declaration");

    auto type = Lexer->getDataType(); // datatype already checked by caller
    Lexer->readNextToken(); // eat DATATYPE

    if (Lexer->getCurToken() != TokenType::IDENTIFIER) {
        Err = "Expected an identifier, instead got: " + Lexer->getCurSymbol();
        throw ParserException(Err);
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
    LogDebug("Parsing a call");

    std::vector<std::unique_ptr<ASTExpression>> args;
    Lexer->readNextToken(); // eat '('

    if (Lexer->getCurToken() == TokenType::KW_RIGHTBRACKET)
        return std::make_unique<ASTStatementCall>(ident, std::move(args));

    // ARGS ::= EXPR , EXPR , ...
    while (true) {
        auto arg = parseExpression();
        args.push_back(std::move(arg));

        switch (Lexer->getCurToken()) {
        case TokenType::KW_COMMA:
            Lexer->readNextToken(); // eat ',' 
            continue;
        case TokenType::KW_RIGHTBRACKET:
            Lexer->readNextToken(); // eat ')'
            return std::make_unique<ASTStatementCall>(ident, std::move(args));
        default:
            Err = "Expected ',' or ')', instead got: " + Lexer->getCurSymbol();
            throw ParserException(Err);
        }
    }

    throw ParserException("parseCall() should never get here.");
}

// ASSIGN ::= '=' EXPR
std::unique_ptr<ASTStatementAssign> Parser::parseAssignment(const std::string ident) {
    LogDebug("Parsing an assignment");

    Lexer->readNextToken(); // eat '='

    auto expr = parseExpression();

    return std::make_unique<ASTStatementAssign>(ident, std::move(expr));
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
    std::unique_ptr<ASTStatement> stmt;

    // DECL ::= DATATYPE IDENT ';'
    if (Lexer->getCurToken() == TokenType::KW_DATATYPE) {
        stmt = parseDecl();
    }
    // CALL ::= IDENT '('
    // ASSIGN ::= IDENT '='
    // EXPR ::= IDENT BINOP* (default behaviour)
    else if (Lexer->getCurToken() == TokenType::IDENTIFIER) {
        std::string ident = Lexer->getStrValue();
        Lexer->readNextToken(); // eat IDENT
        if (Lexer->getCurToken() == TokenType::KW_LEFTBRACKET)
            stmt = parseCall(ident);
        else if (Lexer->getCurToken() == TokenType::KW_EQUALITYOP) {
            stmt = parseAssignment(ident);
        }
        else { // Necessary to check that we're not parsing IDENT IDENT
            if (Lexer->getCurToken() == TokenType::IDENTIFIER) {
                throw ParserException("Did not expect an identifier after an identifier.");
            }
            auto expr = parseExpression();
            stmt = std::make_unique<ASTStatementExpr>(std::move(expr));
        }
    }
    // EXPR ::= NUMBER BINOP*
    // ANYTHING ::= EXPR (basically last resort, e.g. starting with '(')
    else if (Lexer->getCurToken() == TokenType::NUMBER) {
        auto expr = parseExpression();
        stmt = std::make_unique<ASTStatementExpr>(std::move(expr));
    }

    if (Lexer->getCurToken() != TokenType::KW_SEMICOLON) {
        Err = "Expected ';' at the end of a statement, instead got: " +
            Lexer->getCurSymbol();
        throw ParserException(Err);
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

    assert("parseBlock() should never get here" && 0);
}

BinOpPrecedence Parser::OpPrecedence = BinOpPrecedence();
