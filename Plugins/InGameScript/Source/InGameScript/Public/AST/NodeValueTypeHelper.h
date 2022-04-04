#pragma once

#include "AbstractSyntaxTreeNode.h"

// Helper class to decide value type of a AST node.
class INGAMESCRIPT_API FNodeValueTypeHelper
{
public:
	/**
	 * Update node value type based on all children nodes.
	 * @param UpdateNode			ASTNode to update.
	 */
	static void UpdateASTNodeValueType(TSharedPtr<FASTValueNode> UpdateNode);

	static void InitializeConvertMap();
private:
	static EValueType::Type GetBinaryOperatorReturnValueType(EValueType::Type LeftType, EValueType::Type RightType);

	static TMap<EValueType::Type, TMap<EValueType::Type, EValueType::Type>> ConvertMap;
};
