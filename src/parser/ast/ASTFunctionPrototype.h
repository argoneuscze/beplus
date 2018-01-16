#pragma once

#include <string>
#include "ASTNode.h"
#include "../Lexer.h" // return type

class ASTFunctionPrototype : public ASTNode {
public:
    ASTFunctionPrototype(const std::string name, std::vector<std::unique_ptr<ASTArgument>> args, const DataType type) : Name(std::move(name)), Args(std::move(args)), DType(type) { }

    void accept(Visitor* v) override {
        v->visit(this);
    }

protected:
    void print(std::ostream& os) const override {
        os << "Function prototype";
    }

private:
    const std::vector<std::unique_ptr<ASTArgument>> Args;
    const std::string Name;
    const DataType DType;
};
