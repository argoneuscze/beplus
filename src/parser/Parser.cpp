#include "Parser.h"
#include "Util.cpp"

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
