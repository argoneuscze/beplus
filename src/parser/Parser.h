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
#include "ast/ASTExpressionAddAssign.h"
#include "ast/ASTExpressionAssign.h"
#include "ast/ASTExpressionBinOp.h"
#include "ast/ASTExpressionBool.h"
#include "ast/ASTExpressionCall.h"
#include "ast/ASTExpressionCallBuiltin.h"
#include "ast/ASTExpressionNumber.h"
#include "ast/ASTExpressionSubAssign.h"
#include "ast/ASTExpressionVariable.h"
#include "ast/ASTBlock.h"
#include "ast/ASTStatementBlock.h"
#include "ast/ASTStatementDecl.h"
#include "ast/ASTStatementExpr.h"
#include "ast/ASTStatementFor.h"
#include "ast/ASTStatementIf.h"
#include "ast/ASTStatementReturn.h"
#include "ast/ASTStatementWhile.h"
#include "ast/Module.h"

class ParserException : std::exception {
public:
    explicit ParserException(const std::string& msg) {
        this->msg = msg;
    }

    char const* what() const noexcept override {
        return msg.c_str();
    }

private:
    //const char* msg;
    std::string msg;
};

// table made according to: http://en.cppreference.com/w/c/language/operator_precedence
class BinOpPrecedence {
public:
    BinOpPrecedence() {
        Precedence = {
            {BinOp::OP_ADD, 20},
            {BinOp::OP_SUB, 20},
            {BinOp::OP_MUL, 40},
            {BinOp::OP_DIV, 40},
            {BinOp::OP_EQL, 10},
            {BinOp::OP_NEQL, 10},
            {BinOp::OP_LT, 15},
            {BinOp::OP_LTE, 15},
            {BinOp::OP_GT, 15},
            {BinOp::OP_GTE, 15},
            {BinOp::OP_AND, 6},
            {BinOp::OP_OR, 4},
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

    std::unique_ptr<Module> parse();

private:
    bool functionBuiltin(const std::string & name);
    void handleTopLevelStatement();
    void handleFunction();

    static BinOpPrecedence OpPrecedence;
    int getTokPrecedence() const;

    std::unique_ptr<ASTExpression> parseExpression();
    std::unique_ptr<ASTExpression> parsePrimary();
    std::unique_ptr<ASTExpression> parseAssignment(const std::string & ident);
    std::unique_ptr<ASTExpression> parseBinOpRHS(int prec, std::unique_ptr<ASTExpression> LHS);
    std::unique_ptr<ASTExpressionBool> parseBoolExpression(void);
    std::unique_ptr<ASTExpression> parseCall(const std::string & ident);
    std::unique_ptr<ASTExpression> parseIdentExpression(void);
    std::unique_ptr<ASTExpressionNumber> parseNumberExpression();
    std::unique_ptr<ASTExpressionVariable> parseVariableExpression(void);
    std::unique_ptr<ASTExpression> parseParenthesisExpression();

    std::unique_ptr<ASTStatement> parseStatement(void);
    std::unique_ptr<ASTStatementBlock> parseBlockStatement(void);
    std::unique_ptr<ASTStatementDecl> parseDecl(void);
    std::vector<std::unique_ptr<ASTStatementElsif>> parseElsif(void);
    std::unique_ptr<ASTStatementFor> parseFor(void);
    std::unique_ptr<ASTStatementIf> parseIf(void);
    std::unique_ptr<ASTStatementReturn> parseReturn(void);
    std::unique_ptr<ASTStatementWhile> parseWhile(void);

    std::unique_ptr<ASTBlock> parseBlock();

    std::unique_ptr<ASTFunctionPrototype> parsePrototype();
    std::vector<std::unique_ptr<ASTArgument>> parseArguments();

    std::unique_ptr<Lexer> Lexer;

    std::vector<std::unique_ptr<ASTNode>> ASTNodes;

    std::string Err;
};
