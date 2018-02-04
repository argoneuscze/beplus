#pragma once

#include <string>

class ASTIdentVariable : public ASTNode {
public:
    explicit ASTIdentVariable(const std::string& varName, const std::string& attrName) : VarName(varName),
                                                                                         AttrName(attrName) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string& getVarName(void) const {
        return VarName;
    }

    const std::string& getAttrName(void) const {
        return AttrName;
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTIdentVariable: " << VarName + AttrName;
    }

private:
    const std::string VarName;
    const std::string AttrName;
};
