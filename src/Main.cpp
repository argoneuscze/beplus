#include <iostream>
#include <fstream>
#include "parser/Parser.h"

void startREPL();

void runParser(const char* filename) {
	std::ifstream file;

	// TODO check if file exists
	file.open(filename);

	auto parser = std::make_unique<Parser>(file);
	parser->parse();

	file.close();
}

int main(const int argc, const char* argv[]) {
	if (argc == 1) {
		startREPL();
	}
	else if (argc == 2) {
		runParser(argv[1]);
	}
	else {
		std::cout << "Usage: ./beplus [filename]" << std::endl;
		return 1;
	}

	return 0;
}
