#include "SimpleInterpreter.h"

#include <iostream>
#include <cassert>

#include "../parser/ast/ASTArgument.h"
#include "../parser/ast/ASTFunction.h"
#include "../parser/ast/ASTExpressionBinOp.h"
#include "../parser/ast/ASTExpressionNumber.h"
#include "../parser/ast/ASTBlock.h"

SimpleInterpreter::SimpleInterpreter(::Module* mod)
    : CurEnv(nullptr), Module(mod), CurValue(nullptr) {
    interpret();
}

void SimpleInterpreter::visit(::Module* module) {
    std::cout << "[SimInt] Visiting Module" << std::endl;

    CurEnv = std::make_unique<Environment>(nullptr).get();

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
    const auto val1_ptr = std::move(CurValue);
    const auto val1 = dynamic_cast<ValueNumber*>(val1_ptr.get());

    // evaluate right side
    binOp->getRHS()->accept(this);
    const auto val2_ptr = std::move(CurValue);
    const auto val2 = dynamic_cast<ValueNumber*>(val2_ptr.get());

    assert(val1 != nullptr && val2 != nullptr);

    // calculate the result and store it
    switch (op) {
    case BinOp::OP_ADD:
        CurValue = std::make_unique<ValueNumber>(val1->getValue() + val2->getValue());
        break;
    case BinOp::OP_SUB:
        CurValue = std::make_unique<ValueNumber>(val1->getValue() - val2->getValue());
        break;
    case BinOp::OP_MUL:
        CurValue = std::make_unique<ValueNumber>(val1->getValue() * val2->getValue());
        break;
    case BinOp::OP_DIV:
        CurValue = std::make_unique<ValueNumber>(val1->getValue() / val2->getValue());
        break;
    default:
        break;
    }

    std::cout << "Result: " << dynamic_cast<ValueNumber*>(CurValue.get())->getValue() << std::endl;
}

void SimpleInterpreter::visit(ASTExpressionNumber* num) {
    CurValue = std::make_unique<ValueNumber>(num->getValue());
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
