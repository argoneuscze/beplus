#pragma once

#include <string>

class ASTStatementStructInit : public ASTStatement {
public:
    explicit ASTStatementStructInit(const std::string & name) : Name(name) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string & getName(void) const {
        return Name; 
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementStructInit";
    }

private:
    const std::string Name;
};
