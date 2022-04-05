#include "AST/Registry.h"

#include "AST/AbstractSyntaxTreeNode.h"

FRegistry::FRegistry()
{
}

FRegistry::~FRegistry()
{
	RegistryMap.Empty();
}

void FRegistry::RegisterNewVariable(FName VariableName, TSharedPtr<FASTIDNode> VariableNode)
{
	// Remove existing one first
	if (RegistryMap.Contains(VariableName))
	{
		RegistryMap.Remove(VariableName);
	}

	FRegistryEntry NewVariable(ERegistryEntryType::RET_Variable);
	NewVariable.ValueType = VariableNode->GetValueType();
	NewVariable.ASTNode = VariableNode;
	RegistryMap.Add(VariableName, NewVariable);
}

void FRegistry::UnRegisterVariable(FName VariableName)
{
	if (RegistryMap.Contains(VariableName))
	{
		RegistryMap.Remove(VariableName);
	}
}

bool FRegistry::RegisterNewFunction(FName FunctionName, TSharedPtr<FASTFunctionNode> FunctionNode)
{
	// Check if function is already defined
	if (RegistryMap.Contains(FunctionName))
	{
		return false;
	}

	TSharedPtr<FASTValueNode> ReturnValueNode = FunctionNode->GetReturnValueNode();

	FRegistryEntry NewFunction(ERegistryEntryType::RET_Function);
	NewFunction.ASTNode = FunctionNode;
	NewFunction.ValueType = ReturnValueNode->GetValueType();
	RegistryMap.Add(FunctionName, NewFunction);
	return true;
}

bool FRegistry::RegisterNewClass(FName ClassName, TSharedPtr<FASTClassNode> ClassNode)
{
	// Check if class has same name is already defined
	if (RegistryMap.Contains(ClassName))
	{
		return false;
	}

	FRegistryEntry NewClass(ERegistryEntryType::RET_Class);
	NewClass.ValueType = EValueType::GetValueTypeFromClassName(ClassNode->GetClassID());
	NewClass.ASTNode = ClassNode;
	RegistryMap.Add(ClassName, NewClass);
	return true;
}

FRegistryEntry* FRegistry::GetRegistryEntry(FName EntryName, int32 TypeMask)
{
	FRegistryEntry* Result = RegistryMap.Find(EntryName);
	if (Result != nullptr)
	{
		// Check if result meats requirement
		if ((TypeMask & Result->EntryType) == Result->EntryType)
		{
			return Result;
		}
	}
	return nullptr;
}
