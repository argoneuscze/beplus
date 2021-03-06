#pragma once

#include <string>
#include "ASTNode.h"
#include "../Lexer.h" // return type

class ASTFunctionPrototype : public ASTNode {
public:
    ASTFunctionPrototype(const std::string name, std::vector<std::unique_ptr<ASTArgument>> args,
                         const DataType type) : Args(std::move(args)), Name(std::move(name)), DType(type) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string getName(void) const {
        return Name;
    }

    const std::vector<std::unique_ptr<ASTArgument>>* getArgs(void) {
        return &Args;
    }

    const DataType getType(void) const {
        return DType;
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
