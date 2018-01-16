#pragma once

#include <utility>
#include <vector>
#include "ASTNode.h"

class ASTBlock : public ASTNode {
public:
    ASTBlock() { }
    void pushNode(std::unique_ptr<ASTNode> node) {
        Block.push_back(std::move(node));
    }

    void accept(Visitor * v) override {
        v->visit(this);
    }
  
protected:
    // Possibly print more? e.g. amount of AST nodes within block?
    void print(std::ostream& os) const override {
        os << "ASTBlock";
    }

private:
    std::vector<std::unique_ptr<ASTNode>> Block;
};
