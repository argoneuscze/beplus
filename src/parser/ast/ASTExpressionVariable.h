#pragma once

#include <string>
#include "ASTIdentVariable.h"
#include "ASTExpression.h"

class ASTExpressionVariable : public ASTExpression {
public:
    explicit ASTExpressionVariable(std::unique_ptr<ASTIdentVariable> ident) : Ident(std::move(ident)) {
    }

    explicit ASTExpressionVariable(const std::string& varName) : Ident(
        std::make_unique<ASTIdentVariable>(varName, "")) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTIdentVariable* getIdent(void) const {
        return Ident.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "VAR";
    }

private:
    const std::unique_ptr<ASTIdentVariable> Ident;
};
