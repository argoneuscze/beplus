#pragma once

#include <utility>
#include <vector>
#include "ASTNode.h"

class ASTBlock : public ASTNode {
public:
    ASTBlock(std::vector<std::unique_ptr<ASTNode>> vecBlock) : Block(std::move(vecBlock)) { }

    void accept(Visitor * v) override {
        v->visit(this);
    }

    const std::vector<std::unique_ptr<ASTNode>> * getStatements() const {
        return &Block;
    }

protected:
    // Possibly print more? e.g. amount of AST nodes within block?
    void print(std::ostream& os) const override {
        os << "ASTBlock";
    }

private:
    const std::vector<std::unique_ptr<ASTNode>> Block;
};
