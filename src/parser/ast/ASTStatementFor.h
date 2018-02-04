#pragma once

class ASTStatementFor : public ASTStatement {
public:
    explicit ASTStatementFor(std::unique_ptr<ASTExpression> init, std::unique_ptr<ASTExpression> cond,
                             std::unique_ptr<ASTExpression> iter, std::unique_ptr<ASTStatement> statement) :
        Init(std::move(init)), Cond(std::move(cond)), Iter(std::move(iter)), Statement(std::move(statement)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTExpression* getInit(void) {
        return Init.get();
    }

    ASTExpression* getCond(void) {
        return Cond.get();
    }

    ASTExpression* getIter(void) {
        return Iter.get();
    }

    ASTStatement* getStatement(void) {
        return Statement.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementFor";
    }

private:
    const std::unique_ptr<ASTExpression> Init;
    const std::unique_ptr<ASTExpression> Cond;
    const std::unique_ptr<ASTExpression> Iter;
    const std::unique_ptr<ASTStatement> Statement;
};
