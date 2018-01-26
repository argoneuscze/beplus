#pragma once

#include <utility>
#include <vector>
#include <string>
#include <memory>

#include "ASTStatement.h"
#include "ASTExpression.h"

class ASTExpressionCall : public ASTExpression {
public:
    explicit ASTExpressionCall(std::string name,
                              std::vector<std::unique_ptr<ASTExpression>> args) : Args(std::move(args)),
                                                                                  FnName(std::move(name)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string & getName(void) {
        return FnName;
    }

    const std::vector<std::unique_ptr<ASTExpression>> * getArgs(void) {
        return &Args;
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTExpressionCall to function: " << FnName;
    }

private:
    const std::vector<std::unique_ptr<ASTExpression>> Args;
    const std::string FnName;
};
