#pragma once

#include <memory>

#include "ASTExpression.h"
#include "../Lexer.h"

class ASTExpressionBinOp : public ASTExpression {
public:
    ASTExpressionBinOp(std::unique_ptr<ASTExpression> LHS, std::unique_ptr<ASTExpression> RHS, const BinOp op)
        : LHS(std::move(LHS)), RHS(std::move(RHS)), Operator(op) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    BinOp getOperator() const {
        return Operator;
    }

    ASTExpression* getLHS() const {
        return LHS.get();
    }

    ASTExpression* getRHS() const {
        return RHS.get();
    }

protected:
    void print(std::ostream& os) const override {
        std::string opString;
        switch (Operator) {
        case BinOp::OP_ADD:
            opString = "Add";
            break;
        case BinOp::OP_SUB:
            opString = "Subtract";
            break;
        case BinOp::OP_MUL:
            opString = "Multiply";
            break;
        case BinOp::OP_DIV:
            opString = "Divide";
            break;
        default:
            break;
        }

        os << "LHS: <" << *LHS << ">, ";
        os << "OP: " << opString << ", ";
        os << "RHS: <" << *RHS << ">";
    }

private:
    const std::unique_ptr<ASTExpression> LHS;
    const std::unique_ptr<ASTExpression> RHS;
    const BinOp Operator;
};
