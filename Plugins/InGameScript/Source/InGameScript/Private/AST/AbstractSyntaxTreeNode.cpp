#include "AST/AbstractSyntaxTreeNode.h"

FAbstractSyntaxTreeNodeBase::FAbstractSyntaxTreeNodeBase()
	: NodeType(EASTNodeType::ANT_None)
{
}

void FASTMemberAccessorNode::Initialize(const FRegistryEntry& InEntry)
{
	EntryType = InEntry.EntryType;
	MemberNode = InEntry.ASTNode;
}
