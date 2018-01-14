#pragma once

#include "ASTNode.h"
#include "../Lexer.h"
#include <string>

class ASTArgument: public ASTNode {
public:
	ASTArgument(DataType DType, std::string Name);
	void accept(Visitor* v) override;

protected:
	void print(std::ostream& os) const override;

private:
	DataType DType;
	std::string Name;
};
