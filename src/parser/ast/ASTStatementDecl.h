#pragma once
#include <string>
#include "ASTStatement.h"
#include "../Lexer.h" // Lexer::DataType

class ASTStatementDecl : public ASTStatement {
public:
    explicit ASTStatementDecl(DataType type, const char* id) : Type(type), Ident(id) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string& getIdent() const {
        return Ident;
    }

    const DataType& getType(void) const {
        return Type;
    }

protected:
    void print(std::ostream& os) const override {
        os << "VAR DECL OF " << " " << Ident;
    }

private:
    const DataType Type;
    const std::string Ident;
};
