#pragma once

#include <memory>
#include <vector>

#include "ast/ASTNode.h"
#include "lexer.h"
#include "ast/ASTExpression.h"
#include "ast/ASTFunctionPrototype.h"

class Parser {
public:
	explicit Parser(std::istream& input);

	void parse();
private:
	void handleTopLevelExpression();
	void handleFunction();

	std::unique_ptr<ASTExpression> parseExpression();
	std::unique_ptr<ASTExpression> parsePrimary();

	std::unique_ptr<ASTFunctionPrototype> parsePrototype();

	std::unique_ptr<Lexer> Lexer;
	std::vector<std::unique_ptr<ASTNode>> ASTRoot;
};
