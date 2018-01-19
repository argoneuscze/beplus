#pragma once
#include <string>
#include "ASTExpression.h"

class ASTExpressionVariable : public ASTExpression {
public:
    explicit ASTExpressionVariable(const std::string varName) : VarName(varName) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string getName() const {
        return VarName;
    }

protected:
    void print(std::ostream& os) const override {
        os << "VAR: " << VarName;
    }

private:
    const std::string VarName;
};
