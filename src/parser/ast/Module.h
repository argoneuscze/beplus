#pragma once

#include <vector>
#include <memory>

#include "ASTNode.h"

class Module : public Visitable {
public:
    explicit Module(std::vector<std::unique_ptr<ASTNode>>&& nodes) : Nodes(std::move(nodes)) {
    }

    const std::vector<std::unique_ptr<ASTNode>>* getNodes() const {
        return &Nodes;
    };

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }

private:
    std::vector<std::unique_ptr<ASTNode>> Nodes;
};
