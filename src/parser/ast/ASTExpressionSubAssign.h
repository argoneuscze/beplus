#pragma once

#include "ASTExpression.h"

class ASTExpressionSubAssign : public ASTExpression {
public:
    explicit ASTExpressionSubAssign(const std::string & varName, std::unique_ptr<ASTExpression> expr) : VarName(std::move(varName)), Expr(std::move(expr)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string & getName(void) const {
        return VarName;
    }

    ASTExpression* getExpr(void) const {
        return Expr.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTExpressionSubAssign";
    }

private:
    const std::string VarName;
    const std::unique_ptr<ASTExpression> Expr;
};
