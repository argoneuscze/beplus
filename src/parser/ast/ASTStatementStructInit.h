#pragma once

#include <string>

class ASTStatementStructInit : public ASTStatement {
public:
    explicit ASTStatementStructInit(const std::string& structName, const std::string& objName) : StructName(structName),
                                                                                                 ObjName(objName) {
    }

    void accept(Visitor* v) override {
        v->visit(this);
    }

    const std::string& getObjName(void) const {
        return ObjName;
    }

    const std::string& getStructName(void) const {
        return StructName;
    }

protected:
    void print(std::ostream& os) const override {
        os << "ASTStatementStructInit";
    }

private:
    const std::string StructName;
    const std::string ObjName;
};
