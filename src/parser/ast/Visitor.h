#pragma once

class Module;
class ASTFunction;
class ASTFunctionPrototype;
class ASTArgument;
class ASTExpressionBinOp;
class ASTExpressionNumber;
class ASTExpressionVariable;
class ASTBlock;
class ASTStatementAssign;
class ASTStatementCall;
class ASTStatementCallBuiltin;
class ASTStatementDecl;
class ASTStatementExpr;
class ASTStatementIf;

class Visitor {
public:
    Visitor() = default;
    virtual ~Visitor() = default;
    Visitor(const Visitor& other) = default;
    Visitor(Visitor&& other) noexcept = default;
    Visitor& operator=(const Visitor& other) = default;
    Visitor& operator=(Visitor&& other) noexcept = default;

    virtual void visit(Module* module) = 0;
    virtual void visit(ASTFunction* func) = 0;
    virtual void visit(ASTFunctionPrototype* prototype) = 0;
    virtual void visit(ASTArgument* arg) = 0;
    virtual void visit(ASTExpressionBinOp* arg) = 0;
    virtual void visit(ASTExpressionNumber* arg) = 0;
    virtual void visit(ASTExpressionVariable* var) = 0;
    virtual void visit(ASTBlock* block) = 0;
    virtual void visit(ASTStatementAssign* assign) = 0;
    virtual void visit(ASTStatementCall* call) = 0;
    virtual void visit(ASTStatementCallBuiltin* call) = 0;
    virtual void visit(ASTStatementDecl* decl) = 0;
    virtual void visit(ASTStatementExpr* expr) = 0;
    virtual void visit(ASTStatementIf* ifStmt) = 0;
};

class Visitable {
public:
    Visitable() = default;
    virtual ~Visitable() = default;
    Visitable(const Visitable& other) = default;
    Visitable(Visitable&& other) noexcept = default;
    Visitable& operator=(const Visitable& other) = default;
    Visitable& operator=(Visitable&& other) noexcept = default;

    virtual void accept(Visitor* visitor) = 0;
};
