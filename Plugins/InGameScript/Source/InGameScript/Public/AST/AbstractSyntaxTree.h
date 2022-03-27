#pragma once

#include "AbstractSyntaxTreeNode.h"

// Abstract syntax tree of script language.
class INGAMESCRIPT_API FAbstractSyntaxTree
{
public:
	FAbstractSyntaxTree();

	void SetRootNode(TSharedPtr<FAbstractSyntaxTreeNodeBase> NewRoot);
private:
	TSharedPtr<FAbstractSyntaxTreeNodeBase> RootNode;
};
