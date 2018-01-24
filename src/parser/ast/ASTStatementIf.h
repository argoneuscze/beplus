#pragma once

class ASTStatementIf : public ASTStatement {
public:
    explicit ASTStatementIf(std::unique_ptr<ASTExpression> cond, std::unique_ptr<ASTStatement> condExec, std::unique_ptr<ASTStatement> elseExec) : Cond(std::move(cond)), CondExec(std::move(condExec)), ElseExec(std::move(elseExec)) {
}

    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTExpression * getCond(void) {
        return Cond.get();
    }

    ASTStatement * getCondExec(void) {
        return CondExec.get();
    }

    ASTStatement * getElseExec(void) {
        return ElseExec.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementIf";
    }

private:
    const std::unique_ptr<ASTExpression> Cond;
    const std::unique_ptr<ASTStatement> CondExec;
    const std::unique_ptr<ASTStatement> ElseExec;
};
