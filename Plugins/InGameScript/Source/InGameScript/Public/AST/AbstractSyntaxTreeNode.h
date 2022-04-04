#pragma once

#include "ASTMacro.h"
#include "Common.h"
#include "Registry.h"

// Base class of all AST node.
class INGAMESCRIPT_API FAbstractSyntaxTreeNodeBase
{
public:
	FAbstractSyntaxTreeNodeBase();
	virtual ~FAbstractSyntaxTreeNodeBase() {}

	EASTNodeType::Type GetNodeType() const
	{
		return NodeType;
	}

protected:
	EASTNodeType::Type NodeType;
};

class INGAMESCRIPT_API FASTValueNode : public FAbstractSyntaxTreeNodeBase
{
public:
	FASTValueNode()
	{
		NodeType = EASTNodeType::ANT_Value;
		ValueType = EValueType::VT_None;
	}

	DEFINE_CHILD_VARIABLE_PROTECTED(EValueType::Type, ValueType);
};

class INGAMESCRIPT_API FASTSimpleExpressionNode : public FASTValueNode
{
public:
	FASTSimpleExpressionNode()
		: FASTValueNode()
	{
		NodeType = EASTNodeType::ANT_SimpleExpression;
	}
};

class INGAMESCRIPT_API FASTBooleanNode : public FASTSimpleExpressionNode
{
public:
	FASTBooleanNode()
		: FASTSimpleExpressionNode()
	{
		ValueType = EValueType::VT_Boolean;
		Value = false;
	}

	DEFINE_CHILD_VARIABLE(bool, Value);
};

class INGAMESCRIPT_API FASTNumberNode : public FASTSimpleExpressionNode
{
public:
	FASTNumberNode()
		: FASTSimpleExpressionNode()
	{
		ValueType = EValueType::VT_Number;
		Value = 0.f;
	}

	DEFINE_CHILD_VARIABLE(float, Value)
};

class INGAMESCRIPT_API FASTStringNode : public FASTSimpleExpressionNode
{
public:
	FASTStringNode()
		: FASTSimpleExpressionNode()
	{
		ValueType = EValueType::VT_String;
	}

	DEFINE_CHILD_VARIABLE(FString, Value);
};

class INGAMESCRIPT_API FASTVoidNode : public FASTSimpleExpressionNode
{
public:
	FASTVoidNode()
		: FASTSimpleExpressionNode()
	{
		ValueType = EValueType::VT_Void;
	}
};

class INGAMESCRIPT_API FASTClassTypeNode : public FASTSimpleExpressionNode
{
public:
	FASTClassTypeNode()
		: FASTSimpleExpressionNode()
	{
		ValueType = EValueType::VT_Class;
	}

	DEFINE_CHILD_SHARED_PTR(FASTClassNode, ClassNode);
};

class INGAMESCRIPT_API FASTPrefixExpressionNode : public FASTSimpleExpressionNode
{
public:
	FASTPrefixExpressionNode()
	{
		NodeType = EASTNodeType::ANT_PrefixExpression;
	}

	DEFINE_CHILD_SHARED_PTR(class FASTUnaryOperatorNode, UnaryOperatorNode);
	DEFINE_CHILD_SHARED_PTR(class FASTSuffixExpressionNode, ChildExpression);
};

class INGAMESCRIPT_API FASTBinaryOperatorNode : public FASTValueNode
{
public:
	FASTBinaryOperatorNode()
	{
		NodeType = EASTNodeType::ANT_BinaryOp;
		OperatorType = EBinaryOperatorType::BOT_None;
	}

	DEFINE_CHILD_VARIABLE(EBinaryOperatorType::Type, OperatorType);
	DEFINE_CHILD_SHARED_PTR(FASTSimpleExpressionNode, LeftChildNode);
	DEFINE_CHILD_SHARED_PTR(FASTSimpleExpressionNode, RightChildNode);
};

class INGAMESCRIPT_API FASTUnaryOperatorNode : public FASTValueNode
{
public:
	FASTUnaryOperatorNode()
	{
		NodeType = EASTNodeType::ANT_UnaryOp;
		OperatorType = EUnaryOperatorType::UOT_None;
	}

	DEFINE_CHILD_VARIABLE(EUnaryOperatorType::Type, OperatorType);
	DEFINE_CHILD_SHARED_PTR(FASTSuffixExpressionNode, ChildNode);
};

class INGAMESCRIPT_API FASTSuffixExpressionNode : public FASTPrefixExpressionNode
{
public:
	FASTSuffixExpressionNode()
	{
		NodeType = EASTNodeType::ANT_SuffixExpression;
	}
};

class INGAMESCRIPT_API FASTAtomicExpression : public FASTSuffixExpressionNode
{
public:
	FASTAtomicExpression()
	{
		
	}
};

class INGAMESCRIPT_API FASTMemberAccessorNode : public FASTSuffixExpressionNode
{
public:
	FASTMemberAccessorNode()
	{
		NodeType = EASTNodeType::ANT_MemberAccessor;
	}

	void Initialize(const FRegistryEntry& InEntry);

	

	DEFINE_CHILD_VARIABLE(ERegistryEntryType::Type, EntryType);
	DEFINE_CHILD_SHARED_PTR(FASTSuffixExpressionNode, OwningNode);
	DEFINE_CHILD_SHARED_PTR(FAbstractSyntaxTreeNodeBase, MemberNode);
};

class INGAMESCRIPT_API FASTFunctionInvokerNode : public FASTSuffixExpressionNode
{
public:
	FASTFunctionInvokerNode()
	{
		NodeType = EASTNodeType::ANT_FunctionInvoker;
	}

	DEFINE_CHILD_SHARED_PTR(FASTSuffixExpressionNode, FuncPathNode);
	DEFINE_CHILD_SHARED_PTR(class FASTFuncParamNode, FuncParamNode);
};

class INGAMESCRIPT_API FASTIndexAccessorNode : public FASTSuffixExpressionNode
{
public:
	FASTIndexAccessorNode()
	{
		NodeType = EASTNodeType::ANT_IndexAccessor;
	}

	DEFINE_CHILD_SHARED_PTR(FASTSuffixExpressionNode, OuterNode);
	DEFINE_CHILD_SHARED_PTR(FASTValueNode, IndexNode);
};

class INGAMESCRIPT_API FASTIDNode : public FASTAtomicExpression
{
public:
	FASTIDNode()
	{
		
	}

	DEFINE_CHILD_VARIABLE(FName, ID);
	DEFINE_CHILD_SHARED_PTR(FASTSimpleExpressionNode, Value);
};

class INGAMESCRIPT_API FASTFuncParamNode : public FAbstractSyntaxTreeNodeBase
{
public:
	FASTFuncParamNode()
	{
		
	}

	virtual ~FASTFuncParamNode() override
	{
		ClearParamList();
	}

	DEFINE_CHILD_SHARED_PTR_ARRAY(FASTValueNode, ParamList);
};

class INGAMESCRIPT_API FASTFuncArgsNode : public FAbstractSyntaxTreeNodeBase
{
public:
	FASTFuncArgsNode()
	{}

	virtual ~FASTFuncArgsNode() override
	{
		ClearArgsList();
	}

	DEFINE_CHILD_SHARED_PTR_ARRAY(FASTIDNode, ArgsList);
};

class INGAMESCRIPT_API FASTAssignStatementNode : public FAbstractSyntaxTreeNodeBase
{
public:
	FASTAssignStatementNode()
	{
		NodeType = EASTNodeType::ANT_Assign;
	}

	DEFINE_CHILD_SHARED_PTR(FASTIDNode, DestNode);
	DEFINE_CHILD_SHARED_PTR(FASTValueNode, SourceNode);
};

class INGAMESCRIPT_API FASTReturnStatementNode : public FASTValueNode
{
public:
	FASTReturnStatementNode()
	{
		NodeType = EASTNodeType::ANT_Return;
	}

	DEFINE_CHILD_SHARED_PTR(FASTBinaryOperatorNode, ChildNode);
};

class INGAMESCRIPT_API FASTStateListNode : public FAbstractSyntaxTreeNodeBase
{
public:
	FASTStateListNode()
	{
		NodeType = EASTNodeType::ANT_Block;
	}
	virtual ~FASTStateListNode() override
	{
		ClearChildNodes();
	}

	DEFINE_CHILD_SHARED_PTR_ARRAY(FASTValueNode, ChildNodes)
};

class INGAMESCRIPT_API FASTIfNode : public FAbstractSyntaxTreeNodeBase
{
public:
	FASTIfNode()
	{
		NodeType = EASTNodeType::ANT_If;
	}

	DEFINE_CHILD_SHARED_PTR(FASTValueNode, ChildConditionNode)
	DEFINE_CHILD_SHARED_PTR(FASTStateListNode, TrueChildBlockNode)
	DEFINE_CHILD_SHARED_PTR(FASTStateListNode, FalseChildBlockNode)
};

class INGAMESCRIPT_API FASTFunctionNode : public FAbstractSyntaxTreeNodeBase
{
public:
	FASTFunctionNode()
	{
		NodeType = EASTNodeType::ANT_Function;
	}

	DEFINE_CHILD_VARIABLE(FName, FunctionName)
	DEFINE_CHILD_SHARED_PTR(FASTFuncArgsNode, ArgsNode)
	DEFINE_CHILD_SHARED_PTR(FASTStateListNode, StateListNode)
	DEFINE_CHILD_SHARED_PTR(FASTSimpleExpressionNode, ReturnValueNode);
};

class INGAMESCRIPT_API FASTClassNode : public FAbstractSyntaxTreeNodeBase
{
public:
	FASTClassNode()
	{
		NodeType = EASTNodeType::ANT_Class;
	}

	DEFINE_CHILD_VARIABLE(FName, ClassID)
	DEFINE_CHILD_SHARED_PTR(FRegistry, ClassRegistry);
	// Member variable initialization statements
	DEFINE_CHILD_SHARED_PTR_ARRAY(FASTAssignStatementNode, VariableInitialList)
	DEFINE_CHILD_SHARED_PTR_ARRAY(FASTFunctionNode, FunctionNodeList)
};
