#include "parser.h"
#include "util.cpp"

Parser::Parser(std::istream& input) {
	Lexer = std::make_unique<::Lexer>(input);
}

void Parser::parse() {
	// read first token
	Lexer->readNextToken();

	while (true) {
		switch (Lexer->getCurToken()) {
			// TODO more tokens
		case TokenType::KW_SEMICOLON:
			Lexer->readNextToken();
			break;
		case TokenType::KW_DEF: 
			handleFunction();
			break;
		case TokenType::EOFTOK: 
			return;
		case TokenType::INVALID_TOK: 
			// TODO error
			break;
		default: 
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

void Parser::handleFunction() {
	// TODO
}

std::unique_ptr<ASTExpression> Parser::parseExpression() {
	// TODO
	return nullptr;
}

std::unique_ptr<ASTExpression> Parser::parsePrimary() {
	// TODO
	return nullptr;
}
