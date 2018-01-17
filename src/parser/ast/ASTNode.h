#pragma once

#include <ostream>
#include "Visitor.h"

class ASTNode : public Visitable {
public:
    ASTNode() = default;
    virtual ~ASTNode() = default;
    ASTNode(const ASTNode& other) = default;
    ASTNode(ASTNode&& other) noexcept = default;
    ASTNode& operator=(const ASTNode& other) = default;
    ASTNode& operator=(ASTNode&& other) noexcept = default;

    friend std::ostream& operator<<(std::ostream& os, const ASTNode& node) {
        node.print(os);
        return os;
    }

protected:
    virtual void print(std::ostream& os) const = 0;
};
