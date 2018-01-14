#include <memory>
#include <iostream>

const bool IS_DEBUG = true;

void LogParsingError(const char* msg) {
	fprintf(stderr, "Parsing Error: %s\n", msg);
}

void LogDebug(const char* msg) {
	if (IS_DEBUG)
		fprintf(stdout, "DEBUG: %s\n", msg);
}

void LogDebug(const ASTNode* node) {
	if (IS_DEBUG)
		std::cout << "DEBUG: " << *node << std::endl;
}
