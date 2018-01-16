#pragma once

#include "ASTNode.h"
#include "ASTBlock.h"
#include "ASTFunctionPrototype.h"

class ASTFunction : public ASTNode {
public:
    ASTFunction(std::unique_ptr<ASTFunctionPrototype> prototype, std::unique_ptr<ASTBlock> block) : Prototype(std::move(prototype)), Block(std::move(block)) { }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    // Returns a raw pointer to the prototype so that it can be accepted/visited
    ASTFunctionPrototype * getPrototype(void) const {
        return Prototype.get();
    }

    // Returns a raw pointer to the Block so that it can be accepted/visited.
    ASTBlock * getBlock(void) const {
        return Block.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "Function node";
    }
    
private:
    std::unique_ptr<ASTBlock> Block;
    std::unique_ptr<ASTFunctionPrototype> Prototype;
};
