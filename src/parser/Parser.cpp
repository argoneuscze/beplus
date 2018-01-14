#include "Parser.h"
#include "Util.cpp"
#include "ast/ASTArgument.h"

Parser::Parser(std::istream& input) {
	Lexer = std::make_unique<::Lexer>(input);
}

void Parser::parse() {
	LogDebug("Parsing started.");

	// read first token
	Lexer->readNextToken();

	while (true) {
		switch (Lexer->getCurToken()) {
			// TODO more tokens
		case TokenType::KW_SEMICOLON:
			LogDebug("Found stray ';', skipping.");
			Lexer->readNextToken();
			break;
		case TokenType::KW_DEF:
			LogDebug("Function definition found.");
			handleFunction();
			break;
		case TokenType::EOFTOK:
			LogDebug("EOF encountered.");
			return;
		case TokenType::INVALID_TOK:
			LogDebug("Invalid token found.");
			LogParsingError("Invalid token encountered.");
			return;
		default:
			LogDebug("Parsing top level expression.");
			handleTopLevelExpression();
			break;
		}
	}
}

void Parser::handleTopLevelExpression() {
	auto expr = parseExpression();

	if (Lexer->getCurToken() != TokenType::KW_SEMICOLON) {
		LogParsingError("Expected a ';' at the end of an expression.");
		return;
	}

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
	if (!proto)
		exit(-1); // TODO better errors

}

// prototype ::= identifier '(' arguments ')' ':' datatype
std::unique_ptr<ASTFunctionPrototype> Parser::parsePrototype() {
	std::string name = Lexer->getStrValue();
	Lexer->readNextToken(); // eat identifier

	if (Lexer->getCurToken() != TokenType::KW_LEFTBRACKET) {
		LogParsingError("Expected '('.");
		return nullptr;
	}
	Lexer->readNextToken(); // eat '('

	auto args = parseArguments();
	// TODO check if there were errors
	Lexer->readNextToken(); // eat ')'

	if (Lexer->getCurToken() != TokenType::KW_COLON) {
		LogParsingError("Expected ':'.");
		return nullptr;
	}
	Lexer->readNextToken(); // eat ':'

	if (Lexer->getCurToken() != TokenType::KW_DATATYPE) {
		LogParsingError("Expected datatype.");
		return nullptr;
	}
	DataType type = Lexer->getDataType();
	Lexer->readNextToken(); // eat datatype

	// TODO make arguments
	//return std::make_unique<ASTFunctionPrototype>()
}

// arguments ::= datatype identifier ',' | ')'
std::vector<std::unique_ptr<ASTArgument>> Parser::parseArguments() {
	std::vector<std::unique_ptr<ASTArgument>> args;

	if (Lexer->getCurToken() == TokenType::KW_RIGHTBRACKET)
		return args;

	while (true) {
		if (Lexer->getCurToken() != TokenType::KW_DATATYPE) {
			LogParsingError("Expected datatype.");
			// need to return here
			// TODO better error handling...
			exit(-1);
		}
		auto type = Lexer->getDataType();
		Lexer->readNextToken(); // eat datatype

		if (Lexer->getCurToken() != TokenType::IDENTIFIER) {
			LogParsingError("Expected identifier.");
			// need to return here
			// TODO better error handling...
			exit(-1);
		}
		auto ident = Lexer->getStrValue();
		Lexer->readNextToken(); // eat identifier

		args.push_back(std::make_unique<ASTArgument>(type, ident));

		switch (Lexer->getCurToken()) {
		case TokenType::KW_COMMA:
			Lexer->readNextToken(); // eat ','
			continue;
		case TokenType::KW_RIGHTBRACKET:
			return args;
		default:
			exit(-1);
			// TODO better error
			break;
		}
	}
}
