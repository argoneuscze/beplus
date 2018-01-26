#pragma once

#include "ASTExpression.h"

class ASTExpressionBool : public ASTExpression {
public:
    explicit ASTExpressionBool(const bool value) : Value(value) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    bool getValue(void) const {
        return Value;
    }

protected:
    void print(std::ostream& os) const override {
        os << "BOOL " << Value;
    }

private:
    const bool Value;
};
