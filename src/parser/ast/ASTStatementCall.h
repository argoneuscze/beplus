#pragma once

#include <vector>
#include <string>
#include <memory>

#include "ASTStatement.h"
#include "ASTExpression.h"

class ASTStatementCall : public ASTStatement {
public:
    explicit ASTStatementCall(const std::string name, std::vector<std::unique_ptr<ASTExpression>> args) : FnName(name), Args(std::move(args)) { }

    void accept(Visitor* v) override {
        v->visit(this);
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementCall to function: " << FnName;
    }

private:
    const std::vector<std::unique_ptr<ASTExpression>> Args;
    const std::string FnName;
};
