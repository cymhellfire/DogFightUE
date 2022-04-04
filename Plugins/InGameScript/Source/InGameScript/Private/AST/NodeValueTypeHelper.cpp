﻿#include "AST/NodeValueTypeHelper.h"

void FNodeValueTypeHelper::UpdateASTNodeValueType(TSharedPtr<FASTValueNode> UpdateNode)
{
	if (!UpdateNode.IsValid())
	{
		return;
	}

	switch(UpdateNode->GetNodeType())
	{
	case EASTNodeType::ANT_Block:
		break;
	case EASTNodeType::ANT_Statement:
		// Statement value type decided by the only child node
		{
			
		}
		break;
	case EASTNodeType::ANT_Function:
		break;
	case EASTNodeType::ANT_BinaryOp:
		// Check both left and right child node
		{
			TSharedPtr<FASTBinaryOperatorNode> BinaryOperatorNode = StaticCastSharedPtr<FASTBinaryOperatorNode>(UpdateNode);
			TSharedPtr<FASTValueNode> LeftChild = BinaryOperatorNode->GetLeftChildNode();
			TSharedPtr<FASTValueNode> RightChild = BinaryOperatorNode->GetRightChildNode();
			if (LeftChild.IsValid() && RightChild.IsValid())
			{
				EValueType::Type ResultType = GetBinaryOperatorReturnValueType(LeftChild->GetValueType(),
					RightChild->GetValueType());

				// Check invalid operation
				if (ResultType == EValueType::VT_None)
				{
					
				}

				BinaryOperatorNode->SetValueType(ResultType);
			}
		}break;
	case EASTNodeType::ANT_UnaryOp:
		// Use child node value type
		{
			TSharedPtr<FASTUnaryOperatorNode> UnaryOperatorNode = StaticCastSharedPtr<FASTUnaryOperatorNode>(UpdateNode);
			TSharedPtr<FASTValueNode> ChildNode = UnaryOperatorNode->GetChildNode();
			if (ChildNode.IsValid())
			{
				UnaryOperatorNode->SetValueType(ChildNode->GetValueType());
			}
		}break;
	case EASTNodeType::ANT_SimpleExpression: break;
	case EASTNodeType::ANT_PrefixExpression:
		// Use child expression value type
		{
			TSharedPtr<FASTPrefixExpressionNode> PrefixExpressionNode = StaticCastSharedPtr<FASTPrefixExpressionNode>(UpdateNode);
			TSharedPtr<FASTValueNode> ChildNode = PrefixExpressionNode->GetChildExpression();
			if (ChildNode.IsValid())
			{
				PrefixExpressionNode->SetValueType(ChildNode->GetValueType());
			}
		}break;
	case EASTNodeType::ANT_Return:
		// Use child node value type
		{
			TSharedPtr<FASTReturnStatementNode> ReturnStatementNode = StaticCastSharedPtr<FASTReturnStatementNode>(UpdateNode);
			TSharedPtr<FASTValueNode> ChildNode = ReturnStatementNode->GetChildNode();
			if (ChildNode.IsValid())
			{
				ReturnStatementNode->SetValueType(ChildNode->GetValueType());
			}
		}break;
	case EASTNodeType::ANT_FunctionInvoker:
		// Use return value type
		{
			TSharedPtr<FASTFunctionInvokerNode> FunctionInvokerNode = StaticCastSharedPtr<FASTFunctionInvokerNode>(UpdateNode);
			// Find 
		}break;
	case EASTNodeType::ANT_MemberAccessor:
		// Use member value type
		{
			TSharedPtr<FASTMemberAccessorNode> MemberAccessorNode = StaticCastSharedPtr<FASTMemberAccessorNode>(UpdateNode);
			TSharedPtr<FASTValueNode> ValueNode = StaticCastSharedPtr<FASTValueNode>(MemberAccessorNode->GetMemberNode());
			if (ValueNode.IsValid())
			{
				MemberAccessorNode->SetValueType(ValueNode->GetValueType());
			}
		}break;
	case EASTNodeType::ANT_IndexAccessor:
		// Use outer node value type
		{
			TSharedPtr<FASTIndexAccessorNode> IndexAccessorNode = StaticCastSharedPtr<FASTIndexAccessorNode>(UpdateNode);
			TSharedPtr<FASTValueNode> OuterNode = IndexAccessorNode->GetOuterNode();
			if (OuterNode.IsValid())
			{
				IndexAccessorNode->SetValueType(OuterNode->GetValueType());
			}
		}break;
	case EASTNodeType::ANT_None:
	case EASTNodeType::ANT_If:
	case EASTNodeType::ANT_For:
	case EASTNodeType::ANT_Value:				// There is no need to change value type for ValueNode.
	case EASTNodeType::ANT_SuffixExpression:
	case EASTNodeType::ANT_Class:				// Class node doesn't have ValueType property
	case EASTNodeType::ANT_Assign:				// Assign statement is not derived from FASTValueNode
	default: ;
	}
}

TMap<EValueType::Type, TMap<EValueType::Type, EValueType::Type>> FNodeValueTypeHelper::ConvertMap;

void FNodeValueTypeHelper::InitializeConvertMap()
{
	TMap<EValueType::Type, EValueType::Type> BooleanMap;
	// Boolean
	BooleanMap.Add(EValueType::VT_Boolean, EValueType::VT_Boolean);
	BooleanMap.Add(EValueType::VT_Number, EValueType::VT_Number);
	BooleanMap.Add(EValueType::VT_String, EValueType::VT_String);
	ConvertMap.Add(EValueType::VT_Boolean, BooleanMap);

	// Number
	TMap<EValueType::Type, EValueType::Type> NumberMap;
	NumberMap.Add(EValueType::VT_Boolean, EValueType::VT_Number);
	NumberMap.Add(EValueType::VT_Number, EValueType::VT_Number);
	NumberMap.Add(EValueType::VT_String, EValueType::VT_String);
	ConvertMap.Add(EValueType::VT_Number, NumberMap);

	// String
	TMap<EValueType::Type, EValueType::Type> StringMap;
	StringMap.Add(EValueType::VT_Boolean, EValueType::VT_String);
	StringMap.Add(EValueType::VT_Number, EValueType::VT_String);
	StringMap.Add(EValueType::VT_String, EValueType::VT_String);
	ConvertMap.Add(EValueType::VT_String, StringMap);
}

EValueType::Type FNodeValueTypeHelper::GetBinaryOperatorReturnValueType(EValueType::Type LeftType, EValueType::Type RightType)
{
	// Search result in convert map
	TMap<EValueType::Type, EValueType::Type>* LeftTypeMap = ConvertMap.Find(LeftType);
	if (LeftTypeMap)
	{
		EValueType::Type* Result = LeftTypeMap->Find(RightType);
		if (Result)
		{
			return *Result;
		}
	}

	// Return VT_None if no valid conversion available
	return EValueType::VT_None;
}
