#pragma once

#include "ASTNode.h"
#include "ASTBlock.h"

class ASTFunction : public ASTNode {
public:
    ASTFunction(const char * fnName, std::unique_ptr<ASTBlock> block) : Name(fnName), Block(std::move(block)) { }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    // Returns a raw pointer to the Block so that it can be accepted/visited.
    ASTBlock * getBlock(void) const {
        return Block.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "Function name: " << Name;
    }
    
private:
    const std::unique_ptr<ASTBlock> Block;
    const char * Name;
};
