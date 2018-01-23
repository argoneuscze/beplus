#pragma once

#include "ASTNode.h"
#include "../Lexer.h"
#include <string>
#include <utility>

class ASTArgument : public ASTNode {
public:
    ASTArgument(const DataType DType, std::string Name) : DType(DType), Name(std::move(Name)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string & getName(void) {
        return Name;
    }

protected:
    void print(std::ostream& os) const override {
        std::string StrType;
        switch (DType) {
        case DataType::DT_VOID:
            StrType = "Void";
            break;
        case DataType::DT_INT:
            StrType = "Integer";
            break;
        case DataType::DT_CHAR:
            StrType = "Char";
            break;
        default:
            StrType = "This should not happen";
        }
        os << "ARGUMENT " << Name << " " << StrType;
    }

private:
    const DataType DType;
    const std::string Name;
};
