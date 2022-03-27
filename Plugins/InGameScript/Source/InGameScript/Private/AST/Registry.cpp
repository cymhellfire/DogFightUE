#include "AST/Registry.h"

#include "AST/AbstractSyntaxTreeNode.h"

FRegistry::FRegistry()
{
}

FRegistry::~FRegistry()
{
	RegistryMap.Empty();
}

void FRegistry::RegisterNewVariable(FName VariableName, EValueType::Type ValueType)
{
	// Remove existing one first
	if (RegistryMap.Contains(VariableName))
	{
		RegistryMap.Remove(VariableName);
	}

	FVariableDefinition NewVariable(ValueType);
	RegistryMap.Add(VariableName, NewVariable);
}

void FRegistry::RegisterNewVariable(FName VariableName, TSharedPtr<FASTIDNode> VariableNode)
{
	// Remove existing one first
	if (RegistryMap.Contains(VariableName))
	{
		RegistryMap.Remove(VariableName);
	}

	FVariableDefinition NewVariable(VariableNode->GetValueType());
	NewVariable.ValueNode = VariableNode;
	RegistryMap.Add(VariableName, NewVariable);
}

void FRegistry::UnRegisterVariable(FName VariableName)
{
	if (RegistryMap.Contains(VariableName))
	{
		RegistryMap.Remove(VariableName);
	}
}

bool FRegistry::SetVariableValue(FName VariableName, TSharedPtr<FASTIDNode> VariableNode)
{
	FVariableDefinition* VariableDefinition = RegistryMap.Find(VariableName);
	if (VariableDefinition)
	{
		VariableDefinition->ValueType = VariableNode->GetValueType();
		VariableDefinition->ValueNode = VariableNode;
		return true;
	}

	return false;
}

TSharedPtr<FASTIDNode> FRegistry::GetVariableNode(FName VariableName)
{
	return RegistryMap.Contains(VariableName) ? RegistryMap[VariableName].ValueNode : nullptr;
}
