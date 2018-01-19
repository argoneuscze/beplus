#pragma once

#include "ASTExpression.h"

class ASTStatementExpr : public ASTStatement {
public:
    explicit ASTStatementExpr(std::unique_ptr<ASTExpression> expr) : Expr(std::move(expr)) { }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTExpression * getExpr(void) {
        return Expr.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementExpr";
    }

private:
    const std::unique_ptr<ASTExpression> Expr;
};
