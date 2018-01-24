#include "SimpleInterpreter.h"

#include <iostream>
#include <cassert>

#include "../parser/ast/ASTArgument.h"
#include "../parser/ast/ASTFunction.h"
#include "../parser/ast/ASTExpressionBinOp.h"
#include "../parser/ast/ASTExpressionNumber.h"
#include "../parser/ast/ASTExpressionVariable.h"
#include "../parser/ast/ASTStatement.h"
#include "../parser/ast/ASTStatementAssign.h"
#include "../parser/ast/ASTStatementBlock.h"
#include "../parser/ast/ASTStatementCall.h"
#include "../parser/ast/ASTStatementCallBuiltin.h"
#include "../parser/ast/ASTStatementDecl.h"
#include "../parser/ast/ASTStatementElsif.h"
#include "../parser/ast/ASTStatementExpr.h"
#include "../parser/ast/ASTStatementIf.h"
#include "../parser/ast/ASTStatementWhile.h"
#include "../parser/ast/ASTBlock.h"

SimpleInterpreter::SimpleInterpreter(::Module* mod)
    : GlobalEnv(nullptr), CurEnv(nullptr), Module(mod), CurValue(nullptr) {
    interpret();
}

SimpleInterpreter::SimpleInterpreter(void) : Module(nullptr) {
    std::cout << "[SimInt] Empty constructor called" << std::endl;

    GlobalEnv = std::make_unique<Environment>(nullptr);
    CurEnv = GlobalEnv.get();
}

void SimpleInterpreter::visit(::Module* module) {
    std::cout << "[SimInt] Visiting Module" << std::endl;

    //GlobalEnv = std::make_unique<Environment>(nullptr);
    //CurEnv = GlobalEnv.get();

    for (auto& node : *module->getNodes()) {
        node->accept(this);
    }
}

// Visiting function node in the parser only leads to saving the function
// into the function table. Call node takes care of actual calling
// and dealing with arguments and the block.
void SimpleInterpreter::visit(ASTFunction* func) {
    std::cout << "[SimInt] Visiting ASTFunction" << std::endl;

    auto fn = FunctionTable.find(func->getPrototype()->getName());

    if (fn != FunctionTable.end()) {
        std::string err = "Function " + func->getPrototype()->getName() + " already exists in the table.";
        throw InterpreterException(err);
    }

    // Necessary, otherwise the moving of *func can nullify prototype/func.
    auto name = func->getPrototype()->getName();

    FunctionTable[name] = std::make_unique<ASTFunction>(std::move(*func));
}

// TODO remove visiting prototype
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
    const auto val1_ptr = CurValue;
    const auto val1 = dynamic_cast<ValueNumber*>(val1_ptr.get());

    // evaluate right side
    binOp->getRHS()->accept(this);
    const auto val2_ptr = CurValue;
    const auto val2 = dynamic_cast<ValueNumber*>(val2_ptr.get());

    std::cout << "[SimInt] Calculating binop, LHS: " << val1->getValue() << " RHS: " << val2->getValue() << std::endl;

    assert(val1 != nullptr && val2 != nullptr);

    // calculate the result and store it
    switch (op) {
    case BinOp::OP_ADD:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() + val2->getValue());
        break;
    case BinOp::OP_SUB:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() - val2->getValue());
        break;
    case BinOp::OP_MUL:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() * val2->getValue());
        break;
    case BinOp::OP_DIV:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() / val2->getValue());
        break;
    case BinOp::OP_EQL:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() == val2->getValue());
        break;
    case BinOp::OP_NEQL:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() != val2->getValue());
        break;
    case BinOp::OP_LT:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() < val2->getValue());
        break;
    case BinOp::OP_LTE:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() <= val2->getValue());
        break;
    case BinOp::OP_GT:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() > val2->getValue());
        break;
    case BinOp::OP_GTE:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() >= val2->getValue());
        break;
    case BinOp::OP_AND:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() && val2->getValue());
        break;
    case BinOp::OP_OR:
        CurValue = std::make_shared<ValueNumber>(val1->getValue() || val2->getValue());
        break;
    default:
        break;
    }

    std::cout << "[SimInt] Result: " << dynamic_cast<ValueNumber*>(CurValue.get())->getValue() << std::endl;
}

void SimpleInterpreter::visit(ASTExpressionNumber* num) {
    std::cout << "[SimInt] Visiting an ASTExpressionNumber" << std::endl;
    CurValue = std::make_shared<ValueNumber>(num->getValue());
}

void SimpleInterpreter::visit(ASTExpressionVariable* var) {
    std::cout << "[SimInt] Visiting an ASTExpressionVariable" << std::endl;
    CurValue = CurEnv->getVariable(var->getName());
    std::cout << "[SimInt] ==> " << dynamic_cast<ValueNumber*>(CurValue.get())->getValue() << std::endl;
}

void SimpleInterpreter::visit(ASTBlock* block) {
    std::cout << "[SimInt] Visiting a block" << std::endl;

    for (auto& node : *(block->getStatements())) {
        node->accept(this);
    }
}

void SimpleInterpreter::visit(ASTStatementAssign* assign) {
    std::cout << "[SimInt] Visiting an assignment" << std::endl;
    const auto name = assign->getName();
    assign->getExpr()->accept(this);
    const auto val_ptr = CurValue;

    if(!CurEnv->setVariable(name, val_ptr))
        throw InterpreterException("Could not set a variable to its value.");
}

void SimpleInterpreter::visit(ASTStatementBlock* block) {
    std::cout << "[SimInt] Visiting a block statement" << std::endl;

    block->getBlock()->accept(this);
}

void SimpleInterpreter::visit(ASTStatementCall* call) {
    std::cout << "[SimInt] Visiting a call" << std::endl;

    auto func = FunctionTable.find(call->getName());

    if (func == FunctionTable.end()) {
        std::string err = "Function " + call->getName() + " not found in the function table.";
        throw InterpreterException(err);
    }

    CurEnv = CurEnv->fork();

    // TODO resolve args + var names, push them into env
    // TODO check datatypes
    auto argsPrototype = func->second->getPrototype()->getArgs();
    auto argsCall = call->getArgs();

    if(argsCall->size() != argsPrototype->size())
        throw InterpreterException("Wrong number of arguments when calling function.");
  
    for(int i = 0; i < argsCall->size(); i++) {
        auto name = (*argsPrototype)[i]->getName();
        (*argsCall)[i]->accept(this);

        if(!CurEnv->initVariable(name, CurValue))
            throw InterpreterException("Could not initialize a variable in the new environment.");
    }

    func->second->getBlock()->accept(this);

    CurEnv = CurEnv->restorePrev();
}

void SimpleInterpreter::visit(ASTStatementCallBuiltin* call) {
    std::cout << "[SimInt] Visiting a builtin call" << std::endl;
    if (call->getName() == "print")
        builtinPrint(call);
}

void SimpleInterpreter::visit(ASTStatementDecl* decl) {
    std::cout << "[SimInt] Visiting a declaration" << std::endl;

    if(!CurEnv->initVariable(decl->getIdent(), std::make_shared<ValueNumber>(0)))
        throw InterpreterException("Unable to initialize variable.");
}

void SimpleInterpreter::visit(ASTStatementElsif* elsif) {
    std::cout << "[SimInt] Visiting an ASTStatementElsif" << std::endl;
}

void SimpleInterpreter::visit(ASTStatementExpr* expr) {
    std::cout << "[SimInt] Visiting an ASTStatementExpr" << std::endl;

    expr->getExpr()->accept(this);
}

void SimpleInterpreter::visit(ASTStatementIf* ifStmt) {
    std::cout << "[SimInt] Visiting an if statement" << std::endl;
    ifStmt->getCond()->accept(this);

    if (dynamic_cast<ValueNumber*>(CurValue.get())->getValue())
        ifStmt->getCondExec()->accept(this);
    else {
        for (auto & elsif : *(ifStmt->getElsifs())) {
            elsif->getCond()->accept(this);
            if (dynamic_cast<ValueNumber*>(CurValue.get())->getValue()) {
                elsif->getCondExec()->accept(this);
                break;
            }
        }
        if (ifStmt->getElseExec() != nullptr)
            ifStmt->getElseExec()->accept(this);
    }
}

void SimpleInterpreter::visit(ASTStatementWhile* whileStmt) {
    std::cout << "[SimInt] Visiting a while statement" << std::endl;

    while (true) {
        whileStmt->getCond()->accept(this);

        if (!(dynamic_cast<ValueNumber*>(CurValue.get())->getValue()))
            break;

        whileStmt->getStatement()->accept(this);
    }
}

void SimpleInterpreter::interpret() {
    try {
        Module->accept(this);
    }
    catch (InterpreterException& ex) {
        std::cout << "InterpreterException: " << ex.what() << std::endl;
    }
}

void SimpleInterpreter::builtinPrint(ASTStatementCallBuiltin* call) {
    std::cout << "[SimInt] Executing builtin print()" << std::endl;

    for (auto& arg : *(call->getArgs())) {
        arg->accept(this);
        std::cout << dynamic_cast<ValueNumber*>(CurValue.get())->getValue() << std::endl;
    }
}

void SimpleInterpreter::interpretModule(::Module* module) {
    try {
        module->accept(this);
    }
    catch (InterpreterException& ex) {
        std::cout << "InterpreterException: " << ex.what() << std::endl;
    }
}
