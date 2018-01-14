#include "ASTArgument.h"
#include <utility>

ASTArgument::ASTArgument(const DataType DType, std::string Name) : DType(DType), Name(std::move(Name)) {
}

void ASTArgument::accept(Visitor* v) {
	v->visit(this);
}

void ASTArgument::print(std::ostream& os) const {
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
