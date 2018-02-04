#pragma once

class ASTStatementBlock : public ASTStatement {
public:
    explicit ASTStatementBlock(std::unique_ptr<ASTBlock> block) : Block(std::move(block)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    ASTBlock* getBlock(void) {
        return Block.get();
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementBlock";
    }

private:
    const std::unique_ptr<ASTBlock> Block;
};
