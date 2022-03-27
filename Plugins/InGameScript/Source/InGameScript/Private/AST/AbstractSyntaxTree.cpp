#include "AST/AbstractSyntaxTree.h"

FAbstractSyntaxTree::FAbstractSyntaxTree()
	: RootNode(nullptr)
{
}

void FAbstractSyntaxTree::SetRootNode(TSharedPtr<FAbstractSyntaxTreeNodeBase> NewRoot)
{
	RootNode = NewRoot;
}
