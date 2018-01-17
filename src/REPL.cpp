#include <iostream>
#include <string>
#include <memory>
#include <sstream>

#include "parser/Parser.h"
#include "simple_interpreter/SimpleInterpreter.h"

void startREPL() {
    while (true) {
        // READ
        std::cout << "beplus> ";
        std::string input;
        std::getline(std::cin, input);
        std::istringstream stream(input);

        // EVALUATE & PRINT
        auto parser = std::make_unique<Parser>(stream);
        auto module = parser->parse();

        // check for nullptr
        if (!module)
            continue;

        auto interpreter = std::make_unique<SimpleInterpreter>(module.get());
    }
}
