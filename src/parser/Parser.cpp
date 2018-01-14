#include "Parser.h"
#include "Util.cpp"
#include "ast/ASTArgument.h"
#include "ParserException.cpp"

Parser::Parser(std::istream& input) {
	Lexer = std::make_unique<::Lexer>(input);
}

void Parser::parse() {
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
				return;
			case TokenType::INVALID_TOK:
				LogDebug("Invalid token found");
				throw ParserException("Invalid token found");
				return;
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
}

void Parser::handleTopLevelExpression() {
	auto expr = parseExpression();

	if (Lexer->getCurToken() != TokenType::KW_SEMICOLON)
		throw ParserException("Expected a ';' at the end of an expression");

	ASTRoot.push_back(std::move(expr));
}

std::unique_ptr<ASTExpression> Parser::parseExpression() {
	// TODO
	return nullptr;
}

std::unique_ptr<ASTExpression> Parser::parsePrimary() {
	// TODO
	return nullptr;
}

// function ::= 'def' prototype '{' statement* '}'
void Parser::handleFunction() {
	Lexer->readNextToken(); // eat def

	auto proto = parsePrototype();

	// TODO parse the rest of the function
}

// prototype ::= identifier '(' arguments ')' ':' datatype
std::unique_ptr<ASTFunctionPrototype> Parser::parsePrototype() {
	LogDebug("Parsing function prototype");

	std::string name = Lexer->getStrValue();
	Lexer->readNextToken(); // eat identifier

	if (Lexer->getCurToken() != TokenType::KW_LEFTBRACKET)
		throw ParserException("Expected '('");

	Lexer->readNextToken(); // eat '('

	auto args = parseArguments();
	Lexer->readNextToken(); // eat ')'

	if (Lexer->getCurToken() != TokenType::KW_COLON)
		throw ParserException("Expected ':'");
	Lexer->readNextToken(); // eat ':'

	if (Lexer->getCurToken() != TokenType::KW_DATATYPE)
		throw ParserException("Expected datatype");
	auto type = Lexer->getDataType();
	Lexer->readNextToken(); // eat datatype

	throw ParserException("Not implemented yet");
	// TODO make arguments
	//return std::make_unique<ASTFunctionPrototype>()
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
			throw ParserException("Expected ',' or ')'");
		}
	}
}
