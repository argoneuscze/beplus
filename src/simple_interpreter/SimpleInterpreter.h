#pragma once

#include <map>
#include <memory>
#include <string>

#include "../parser/ast/Visitor.h"
#include "../parser/ast/ASTNode.h"
#include "../parser/ast/Module.h"

#include "Environment.h"
#include "Heap.h"
#include "Value.h"

class InterpreterException : std::exception {
public:
    explicit InterpreterException(const std::string& msg) {
        this->msg = msg;
    }

    char const* what() const noexcept override {
        return msg.c_str();
    }

private:
    //const char* msg;
    std::string msg;
};

class ReturnException : std::exception {
public:
    explicit ReturnException() {
    }

    char const* what() const noexcept override {
        return "Return found in AST block.";
    }
};

class SimpleInterpreter : public Visitor {
public:
    explicit SimpleInterpreter(Module* mod);
    explicit SimpleInterpreter(void);

    void interpretModule(Module* module);

    void visit(Module* module) override;
    void visit(ASTArgument* arg) override;
    void visit(ASTBlock* block) override;
    void visit(ASTFunction* func) override;
    void visit(ASTFunctionPrototype* prototype) override;
    void visit(ASTExpressionAddAssign* assign) override;
    void visit(ASTExpressionAssign* assign) override;
    void visit(ASTExpressionBinOp* binOp) override;
    void visit(ASTExpressionBool* boolExpr) override;
    void visit(ASTExpressionCall* call) override;
    void visit(ASTExpressionCallBuiltin* call) override;
    void visit(ASTExpressionNumber* num) override;
    void visit(ASTExpressionSubAssign* assign) override;
    void visit(ASTExpressionVariable* var) override;
    void visit(ASTIdentVariable* id) override;
    void visit(ASTStatementBlock* block) override;
    void visit(ASTStatementDecl* decl) override;
    void visit(ASTStatementElsif* elsif) override;
    void visit(ASTStatementExpr* expr) override;
    void visit(ASTStatementFor* forExpr) override;
    void visit(ASTStatementIf* ifStmt) override;
    void visit(ASTStatementReturn* ret) override;
    void visit(ASTStatementWhile* whileStmt) override;
    void visit(ASTStatementStructDecl* decl) override;
    void visit(ASTStatementStructInit* init) override;

private:
    void builtinPrint(ASTExpressionCallBuiltin* call);
    void builtinReadInput(ASTExpressionCallBuiltin* call);

    std::unique_ptr<Environment> GlobalEnv;
    Environment* CurEnv;

    Heap Heap;
    std::unique_ptr<GarbageCollector> GC;

    Module* const Module;
    std::shared_ptr<Value> CurValue;

    std::map<std::string, std::unique_ptr<ASTFunction>> FunctionTable;
    std::map<std::string, std::unique_ptr<ASTStatementStructDecl>> StructTable;

    void interpret();
};
