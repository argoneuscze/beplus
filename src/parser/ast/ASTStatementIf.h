#pragma once

class ASTStatementIf : public ASTStatement {
public:
    explicit ASTStatementIf(std::unique_ptr<ASTExpression> cond, std::unique_ptr<ASTStatement> condExec) : Cond(std::move(cond)), CondExec(std::move(condExec)) {
}

    void accept(Visitor* v) {
        v->visit(this);
    }

    ASTExpression * getCond(void) {
        return Cond.get();
    }

    ASTStatement * getCondExec(void) {
        return CondExec.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementIf";
    }

private:
    const std::unique_ptr<ASTExpression> Cond;
    const std::unique_ptr<ASTStatement> CondExec;
};
