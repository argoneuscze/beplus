#pragma once

class ASTFunction;
class ASTFunctionPrototype;
class ASTArgument;
class ASTExpressionBinOp;
class ASTExpressionNumber;
class ASTBlock;
class ASTStatementDecl;

class Visitor {
public:
    Visitor() = default;
    virtual ~Visitor() = default;
    Visitor(const Visitor& other) = default;
    Visitor(Visitor&& other) noexcept = default;
    Visitor& operator=(const Visitor& other) = default;
    Visitor& operator=(Visitor&& other) noexcept = default;

    virtual void visit(ASTFunction* func) = 0;
    virtual void visit(ASTFunctionPrototype* prototype) = 0;
    virtual void visit(ASTArgument* arg) = 0;
    virtual void visit(ASTExpressionBinOp* arg) = 0;
    virtual void visit(ASTExpressionNumber* arg) = 0;
    virtual void visit(ASTBlock* block) = 0;
    virtual void visit(ASTStatementDecl* decl) = 0;
};
