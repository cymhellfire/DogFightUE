#include "AST/AbstractSyntaxTreeNode.h"

FAbstractSyntaxTreeNodeBase::FAbstractSyntaxTreeNodeBase()
	: NodeType(EASTNodeType::ANT_None)
{
}

FASTClassTypeNode::FASTClassTypeNode(TSharedPtr<FASTClassNode> ClassNode)
	: FASTSimpleExpressionNode()
{
	ValueType = EValueType::GetValueTypeFromClassName(ClassNode->GetClassID());
	SetClassNode(ClassNode);
}

void FASTMemberAccessorNode::Initialize(const FRegistryEntry& InEntry)
{
	EntryType = InEntry.EntryType;
	MemberNode = InEntry.ASTNode;
}
