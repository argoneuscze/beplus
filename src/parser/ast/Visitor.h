#pragma once

class Module;
class ASTFunction;
class ASTFunctionPrototype;
class ASTArgument;
class ASTExpressionAddAssign;
class ASTExpressionAssign;
class ASTExpressionBinOp;
class ASTExpressionBool;
class ASTExpressionCall;
class ASTExpressionCallBuiltin;
class ASTExpressionNumber;
class ASTExpressionSubAssign;
class ASTExpressionVariable;
class ASTBlock;
class ASTStatementBlock;
class ASTStatementCall;
class ASTStatementCallBuiltin;
class ASTStatementDecl;
class ASTStatementElsif;
class ASTStatementExpr;
class ASTStatementFor;
class ASTStatementIf;
class ASTStatementReturn;
class ASTStatementWhile;

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
    virtual void visit(ASTExpressionAddAssign* assign) = 0;
    virtual void visit(ASTExpressionAssign* assign) = 0;
    virtual void visit(ASTExpressionBinOp* arg) = 0;
    virtual void visit(ASTExpressionBool* boolExpr) = 0;
    virtual void visit(ASTExpressionCall* call) = 0;
    virtual void visit(ASTExpressionCallBuiltin* call) = 0;
    virtual void visit(ASTExpressionNumber* arg) = 0;
    virtual void visit(ASTExpressionSubAssign* assign) = 0;
    virtual void visit(ASTExpressionVariable* var) = 0;
    virtual void visit(ASTBlock* block) = 0;
    virtual void visit(ASTStatementBlock* block) = 0;
    virtual void visit(ASTStatementDecl* decl) = 0;
    virtual void visit(ASTStatementElsif* elsif) = 0;
    virtual void visit(ASTStatementExpr* expr) = 0;
    virtual void visit(ASTStatementFor* forStmt) = 0;
    virtual void visit(ASTStatementIf* ifStmt) = 0;
    virtual void visit(ASTStatementReturn* ret) = 0;
    virtual void visit(ASTStatementWhile* whileStmt) = 0;
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
