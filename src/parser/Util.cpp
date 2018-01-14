#include <memory>

const bool IS_DEBUG = true;

std::unique_ptr<ASTNode> LogParsingError(const char* msg) {
	fprintf(stderr, "Parsing Error: %s\n", msg);
	return nullptr;
}

void LogDebug(const char* msg) {
	if (IS_DEBUG)
		fprintf(stdout, "DEBUG: %s\n", msg);
}
