#pragma once

#include <memory>
#include <vector>

#include "Lexer.h"

#include "ast/ASTArgument.h"
#include "ast/ASTExpression.h"
#include "ast/ASTFunctionPrototype.h"

class ParserException : std::exception {
public:
    explicit ParserException(const char* msg) {
        this->msg = msg;
    }

    char const* what() const override {
        return msg;
    }

private:
    const char* msg;
};

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
    std::vector<std::unique_ptr<ASTArgument>> parseArguments();

    std::unique_ptr<Lexer> Lexer;
    std::vector<std::unique_ptr<ASTNode>> ASTRoot;
};
