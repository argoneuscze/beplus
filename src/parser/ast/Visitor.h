#pragma once

class ASTFunction;
class ASTArgument;

class Visitor {
public:
	virtual ~Visitor() = default;
	Visitor(const Visitor& other) = default;
	Visitor(Visitor&& other) noexcept = default;
	Visitor& operator=(const Visitor& other) = default;
	Visitor& operator=(Visitor&& other) noexcept = default;

	virtual void visit(ASTFunction* func) = 0;
	virtual void visit(ASTArgument* arg) = 0;
};