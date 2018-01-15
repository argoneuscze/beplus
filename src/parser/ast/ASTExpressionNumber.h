#pragma once
#include "ASTExpression.h"

class ASTExpressionNumber : public ASTExpression {
public:
    explicit ASTExpressionNumber(const long value) : Value(value) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

protected:
    void print(std::ostream& os) const override {
        os << "NUM " << Value;
    }

private:
    const long Value;
};
