#pragma once

#include "ASTStatement.h"
#include "ASTExpression.h"

class ASTExpressionAssign : public ASTExpression {
public:
    explicit ASTExpressionAssign(const std::string varName, std::unique_ptr<ASTExpression> expr) : VarName(varName), Expr(std::move(expr)) { }
    
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
        os << "ASTStatementAssign into var: " << VarName;
    }

private:
    const std::string VarName;
    const std::unique_ptr<ASTExpression> Expr;
};
