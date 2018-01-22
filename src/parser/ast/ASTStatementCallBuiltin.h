#pragma once
#include <vector>

class ASTStatementCallBuiltin : public ASTStatement {
public:
    explicit ASTStatementCallBuiltin(const std::string & fnName, std::vector<std::unique_ptr<ASTExpression>> args) : FnName(fnName), Args(std::move(args)) {
}

    void accept(Visitor* v) {
        v->visit(this);
    }

    const std::string & getName(void) const {
        return FnName;
    }

    const std::vector<std::unique_ptr<ASTExpression>>* getArgs(void) const {
        return &Args;
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementCallBuiltin to " << FnName;
    }

private:
    const std::string FnName;
    const std::vector<std::unique_ptr<ASTExpression>> Args;
};
