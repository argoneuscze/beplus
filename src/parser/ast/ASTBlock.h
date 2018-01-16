#pragma once

#include <utility>
#include <vector>
#include "ASTNode.h"

class ASTBlock : public ASTNode {
public:
    ASTBlock(std::vector<std::unique_ptr<ASTNode>> vecBlock) : Block(std::move(vecBlock)), Idx(0) { }

    void accept(Visitor * v) override {
        v->visit(this);
    }

    const std::vector<std::unique_ptr<ASTNode>> * getStatements() const {
        return &Block;
    }

    // Zeros out the index so that the class can generate each node easily.
    void initNodeGeneration() {
        Idx = 0;
    }

    ASTNode * getNextNode() {
        if (Idx >= Block.size())
            return nullptr;

        return Block[Idx++].get(); // post increment - returns Block[i]
    }
  
protected:
    // Possibly print more? e.g. amount of AST nodes within block?
    void print(std::ostream& os) const override {
        os << "ASTBlock";
    }

private:
    const std::vector<std::unique_ptr<ASTNode>> Block;
    int Idx;
};
