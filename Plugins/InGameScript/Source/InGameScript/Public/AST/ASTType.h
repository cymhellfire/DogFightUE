#pragma once

#include "Common.h"

// Base class of all AST node.
class INGAMESCRIPT_API FAbstractSyntaxTreeNodeBase
{
public:
	FAbstractSyntaxTreeNodeBase();

protected:
	EASTNodeType::Type NodeType;
};

// Abstract syntax tree of script language.
class INGAMESCRIPT_API FAbstractSyntaxTree
{
public:
	FAbstractSyntaxTree();
private:
	FAbstractSyntaxTreeNodeBase* RootNode;
};
