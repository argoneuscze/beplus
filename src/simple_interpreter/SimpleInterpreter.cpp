#include "SimpleInterpreter.h"

#include "../parser/ast/ASTArgument.h"
#include "../parser/ast/ASTFunction.h"
#include "../parser/ast/ASTExpressionBinOp.h"
#include "../parser/ast/ASTExpressionNumber.h"
#include "../parser/ast/ASTBlock.h"
#include <iostream>

SimpleInterpreter::SimpleInterpreter(const std::vector<std::unique_ptr<ASTNode>>& rootNodes)
    : RootNodes(rootNodes), ValueNum(0) {
    interpret();
}

void SimpleInterpreter::visit(ASTFunction* func) {
    std::cout << "[SimInt] Visiting ASTFunction" << std::endl;
}

void SimpleInterpreter::visit(ASTArgument* arg) {
}

void SimpleInterpreter::visit(ASTExpressionBinOp* binOp) {
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
    std::cout << "Simple interpreter visiting a block" << std::endl;
}

void SimpleInterpreter::visit(ASTStatementDecl* decl) {
    std::cout << "[SimInt] Visiting a declaration" << std::endl;
}

void SimpleInterpreter::interpret() {
    for (auto& node : RootNodes) {
        node->accept(this);
    }
}
