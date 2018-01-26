#pragma once

#include "ASTStatement.h"
#include <string>

class ASTStatementReturn : public ASTStatement {
public:
    explicit ASTStatementReturn(std::unique_ptr<ASTExpression> expr) : RetExpr(std::move(expr)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTExpression * getRetExpr(void) {
        return RetExpr.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementReturn";
    }

private:
    const std::unique_ptr<ASTExpression> RetExpr;
};
