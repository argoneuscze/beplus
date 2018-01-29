#pragma once

#include "ASTExpression.h"
#include "ASTIdentVariable.h"

class ASTExpressionAssign : public ASTExpression {
public:
    explicit ASTExpressionAssign(std::unique_ptr<ASTIdentVariable> ident, std::unique_ptr<ASTExpression> expr) : Ident(std::move(ident)), Expr(std::move(expr)) { }
    
    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTIdentVariable* getIdent(void) const {
        return Ident.get();
    }

    ASTExpression* getExpr(void) const {
        return Expr.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTExpressionAssign";
    }

private:
    const std::unique_ptr<ASTIdentVariable> Ident;
    const std::unique_ptr<ASTExpression> Expr;
};
