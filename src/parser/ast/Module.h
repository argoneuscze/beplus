#pragma once

#include <vector>
#include <memory>

#include "ASTNode.h"

class Module {
public:
    explicit Module(std::vector<std::unique_ptr<ASTNode>>&& nodes) : Nodes(std::move(nodes)) {
    }

    const std::vector<std::unique_ptr<ASTNode>>* getNodes() const {
        return &Nodes;
    };

private:
    std::vector<std::unique_ptr<ASTNode>> Nodes;
};
