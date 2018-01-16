#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

#include "Lexer.h"

#include "ast/ASTArgument.h"
#include "ast/ASTExpression.h"
#include "ast/ASTFunction.h"
#include "ast/ASTFunctionPrototype.h"
#include "ast/ASTExpressionNumber.h"
#include "ast/ASTExpressionBinOp.h"
#include "ast/ASTBlock.h"
#include "ast/ASTStatementDecl.h"

class ParserException : std::exception {
public:
    //explicit ParserException(const char* msg) {
    explicit ParserException(std::string msg) {
        this->msg = msg;
    }

    char const* what() const noexcept override {
        return msg.c_str();
    }

private:
    //const char* msg;
    std::string msg;
};

class BinOpPrecedence {
public:
    BinOpPrecedence() {
        Precedence = {
            {BinOp::OP_ADD, 20},
            {BinOp::OP_SUB, 20},
            {BinOp::OP_MUL, 40},
            {BinOp::OP_DIV, 40},
        };
    }

    int get(const BinOp op) const {
        return Precedence.at(op);
    }

private:
    std::unordered_map<BinOp, int> Precedence;
};

class Parser {
public:
    explicit Parser(std::istream& input);

    std::vector<std::unique_ptr<ASTNode>> parse();

private:
    void handleTopLevelExpression();
    void handleFunction();

    static BinOpPrecedence OpPrecedence;
    int getTokPrecedence() const;

    std::unique_ptr<ASTExpression> parseExpression();
    std::unique_ptr<ASTExpression> parsePrimary();
    std::unique_ptr<ASTExpression> parseBinOpRHS(int prec, std::unique_ptr<ASTExpression> LHS);
    std::unique_ptr<ASTExpressionNumber> parseNumberExpression();
    std::unique_ptr<ASTExpression> parseParenthesisExpression();

    std::unique_ptr<ASTStatement> parseStatement();
    std::unique_ptr<ASTStatementDecl> parseDecl();

    std::unique_ptr<ASTBlock> parseBlock();

    std::unique_ptr<ASTFunctionPrototype> parsePrototype();
    std::vector<std::unique_ptr<ASTArgument>> parseArguments();

    std::unique_ptr<Lexer> Lexer;
    std::vector<std::unique_ptr<ASTNode>> ASTRoot;
};
