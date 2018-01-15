#pragma once

#include "../parser/ast/Visitor.h"

class SimpleInterpreter : public Visitor {
public:
    void visit(ASTFunction* func) override;
    void visit(ASTArgument* arg) override;
};
