#include <memory>
#include "ast/ASTExpression.h"
#include <string>

std::unique_ptr<ASTNode> LogParsingError(const char* msg) {
	fprintf(stderr, "Parsing Error: %s\n", msg);
	return nullptr;
}
