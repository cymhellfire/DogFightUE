#pragma once
#include "Common.h"

class FASTIDNode;

struct INGAMESCRIPT_API FVariableDefinition
{
	explicit FVariableDefinition(EValueType::Type InType)
	{
		ValueType = InType;
		ValueNode = nullptr;
	}

	EValueType::Type ValueType;
	TSharedPtr<FASTIDNode> ValueNode;
};

// A table stores all defined variables in current code scope.
class INGAMESCRIPT_API FRegistry
{
public:
	FRegistry();
	~FRegistry();

	void RegisterNewVariable(FName VariableName, EValueType::Type ValueType);
	void RegisterNewVariable(FName VariableName, TSharedPtr<FASTIDNode> VariableNode = nullptr);
	void UnRegisterVariable(FName VariableName);

	bool SetVariableValue(FName VariableName, TSharedPtr<FASTIDNode> VariableNode);

	TSharedPtr<FASTIDNode> GetVariableNode(FName VariableName);

protected:
	TMap<FName, FVariableDefinition> RegistryMap;
};
