#include "SimpleInterpreter.h"

#include "../parser/ast/ASTArgument.h"

void SimpleInterpreter::visit(ASTFunction* func) {
	// TODO
}

void SimpleInterpreter::visit(ASTArgument* arg) {
	// TODO
	arg->accept(this);
}
