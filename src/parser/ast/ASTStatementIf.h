#pragma once

#include <vector>
#include "ASTStatementElsif.h"

class ASTStatementIf : public ASTStatement {
public:
    explicit ASTStatementIf(std::unique_ptr<ASTExpression> cond, std::unique_ptr<ASTStatement> condExec,
                            std::unique_ptr<ASTStatement> elseExec,
                            std::vector<std::unique_ptr<ASTStatementElsif>> elseIfs) : Cond(std::move(cond)),
                                                                                       CondExec(std::move(condExec)),
                                                                                       ElseExec(std::move(elseExec)),
                                                                                       ElseIfs(std::move(elseIfs)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTExpression* getCond(void) {
        return Cond.get();
    }

    ASTStatement* getCondExec(void) {
        return CondExec.get();
    }

    ASTStatement* getElseExec(void) {
        return ElseExec.get();
    }

    const std::vector<std::unique_ptr<ASTStatementElsif>>* getElsifs(void) {
        return &ElseIfs;
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementIf";
    }

private:
    const std::unique_ptr<ASTExpression> Cond;
    const std::unique_ptr<ASTStatement> CondExec;
    const std::unique_ptr<ASTStatement> ElseExec;
    const std::vector<std::unique_ptr<ASTStatementElsif>> ElseIfs;
};
