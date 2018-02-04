#pragma once

#include <vector>
#include <string>
#include "ASTStatementDecl.h"

class ASTStatementStructDecl : public ASTStatement {
public:
    explicit ASTStatementStructDecl(const std::string& name,
                                    std::vector<std::unique_ptr<ASTStatementDecl>>& decls) : Name(name),
                                                                                             Decls(std::move(decls)) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string& getName(void) const {
        return Name;
    }

    const std::vector<std::unique_ptr<ASTStatementDecl>>* getDecls(void) const {
        return &Decls;
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStructDecl";
    }

private:
    const std::string Name;
    std::vector<std::unique_ptr<ASTStatementDecl>> Decls;
};
