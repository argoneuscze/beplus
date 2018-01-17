#include "SimpleInterpreter.h"

#include <iostream>

#include "../parser/ast/ASTArgument.h"
#include "../parser/ast/ASTFunction.h"
#include "../parser/ast/ASTExpressionBinOp.h"
#include "../parser/ast/ASTExpressionNumber.h"
#include "../parser/ast/ASTBlock.h"

SimpleInterpreter::SimpleInterpreter(::Module* mod)
    : Environment(std::make_unique<::Environment>()), Module(mod), ValueNum(0) {
    interpret();
}

void SimpleInterpreter::visit(::Module* module) {
    std::cout << "[SimInt] Visiting Module" << std::endl;

    // todo create root env etc.

    for (auto& node : *module->getNodes()) {
        node->accept(this);
    }
}

void SimpleInterpreter::visit(ASTFunction* func) {
    std::cout << "[SimInt] Visiting ASTFunction" << std::endl;
    func->getPrototype()->accept(this);
    // TODO - push prototype args into env, check their existence ...
    func->getBlock()->accept(this);
}

void SimpleInterpreter::visit(ASTFunctionPrototype* prototype) {
    std::cout << "[SimInt] Visiting ASTFunctionPrototype" << std::endl;
}

void SimpleInterpreter::visit(ASTArgument* arg) {
}

void SimpleInterpreter::visit(ASTExpressionBinOp* binOp) {
    std::cout << "[SimInt] Visiting ASTExpressionBinOp" << std::endl;

    // get operator
    const auto op = binOp->getOperator();

    // evaluate left side
    binOp->getLHS()->accept(this);
    const auto val1 = ValueNum;

    // evaluate right side
    binOp->getRHS()->accept(this);
    const auto val2 = ValueNum;

    // calculate the result and store it
    switch (op) {
    case BinOp::OP_ADD:
        ValueNum = val1 + val2;
        break;
    case BinOp::OP_SUB:
        ValueNum = val1 - val2;
        break;
    case BinOp::OP_MUL:
        ValueNum = val1 * val2;
        break;
    case BinOp::OP_DIV:
        ValueNum = val1 / val2;
        break;
    default:
        break;
    }

    std::cout << "Result: " << ValueNum << std::endl;
}

void SimpleInterpreter::visit(ASTExpressionNumber* num) {
    ValueNum = num->getValue();
}

void SimpleInterpreter::visit(ASTBlock* block) {
    std::cout << "[SimInt] Visiting a block" << std::endl;

    for (auto& node : *(block->getStatements())) {
        node->accept(this);
    }
}

void SimpleInterpreter::visit(ASTStatementDecl* decl) {
    std::cout << "[SimInt] Visiting a declaration" << std::endl;
}

void SimpleInterpreter::visit(ASTStatementCall* call) {
    std::cout << "[SimInt] Visiting a call" << std::endl;
}

void SimpleInterpreter::interpret() {
    Module->accept(this);
}
