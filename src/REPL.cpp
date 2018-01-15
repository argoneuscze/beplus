#include <iostream>
#include <string>
#include <memory>
#include <sstream>

#include "parser/Parser.h"

void startREPL() {
    while (true) {
        // READ
        std::cout << "beplus> ";
        std::string input;
        std::getline(std::cin, input);
        std::istringstream stream(input);

        // EVALUATE
        auto parser = std::make_unique<Parser>(stream);
        parser->parse();

        // PRINT
        // TODO
    }
}
