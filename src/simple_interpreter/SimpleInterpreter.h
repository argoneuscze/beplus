#pragma once

#include <memory>
#include <vector>

#include "../parser/ast/Visitor.h"
#include "../parser/ast/ASTNode.h"
#include "../parser/ast/Module.h"

#include "Environment.h"
#include "Value.h"

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
    Environment* CurEnv;
    Module* const Module;
    std::unique_ptr<Value> CurValue;

    void interpret();
};
