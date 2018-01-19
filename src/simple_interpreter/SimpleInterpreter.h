#pragma once

#include <memory>

#include "../parser/ast/Visitor.h"
#include "../parser/ast/ASTNode.h"
#include "../parser/ast/Module.h"

#include "Environment.h"
#include "Value.h"

class InterpreterException : std::exception {
public:
    //explicit ParserException(const char* msg) {
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

class SimpleInterpreter : public Visitor {
public:
    explicit SimpleInterpreter(Module* mod);

    void visit(Module* module) override;
    void visit(ASTFunction* func) override;
    void visit(ASTFunctionPrototype* prototype) override;
    void visit(ASTArgument* arg) override;
    void visit(ASTExpressionBinOp* binOp) override;
    void visit(ASTExpressionNumber* num) override;
    void visit(ASTBlock* block) override;
    void visit(ASTStatementDecl* decl) override;
    void visit(ASTStatementCall* call) override;

private:
    std::unique_ptr<Environment> GlobalEnv;
    Environment* CurEnv;

    Module* const Module;
    std::shared_ptr<Value> CurValue;

    void interpret();
};
