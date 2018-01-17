#pragma once

#include <memory>
#include <vector>

#include "../parser/ast/Visitor.h"
#include "../parser/ast/ASTNode.h"

class SimpleInterpreter : public Visitor {
public:
    explicit SimpleInterpreter(const std::vector<std::unique_ptr<ASTNode>>& rootNodes);

    void visit(ASTFunction* func) override;
    void visit(ASTFunctionPrototype* prototype) override;
    void visit(ASTArgument* arg) override;
    void visit(ASTExpressionBinOp* binOp) override;
    void visit(ASTExpressionNumber* num) override;
    void visit(ASTBlock* block) override;
    void visit(ASTStatementDecl* decl) override;
    void visit(ASTStatementCall* call) override;

private:
    const std::vector<std::unique_ptr<ASTNode>>& RootNodes;

    void interpret();

    long ValueNum;
};
