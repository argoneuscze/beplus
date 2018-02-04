#pragma once

class ASTStatementWhile : public ASTStatement {
public:
    explicit ASTStatementWhile(std::unique_ptr<ASTExpression> cond,
                               std::unique_ptr<ASTStatement> statement) : Cond(std::move(cond)),
                                                                          Statement(std::move(statement)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTExpression* getCond(void) {
        return Cond.get();
    }

    ASTStatement* getStatement(void) {
        return Statement.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementWhile";
    }

private:
    const std::unique_ptr<ASTExpression> Cond;
    const std::unique_ptr<ASTStatement> Statement;
};
