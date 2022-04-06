#include "AST/AbstractSyntaxTreeNode.h"
#include "AST/ASTType.h"

FAbstractSyntaxTreeNodeBase::FAbstractSyntaxTreeNodeBase()
	: NodeType(EASTNodeType::ANT_None)
{
}

FASTClassTypeNode::FASTClassTypeNode(TSharedPtr<FASTClassNode> ClassNode)
	: FASTSimpleExpressionNode()
{
	ValueType = EValueType::FValueTypeManager::GetInstance()->GetValueTypeFromClassName(ClassNode->GetClassID());
	SetClassNode(ClassNode);
}

void FASTMemberAccessorNode::Initialize(const FRegistryEntry& InEntry)
{
	EntryType = InEntry.EntryType;
	MemberNode = InEntry.ASTNode;
}
