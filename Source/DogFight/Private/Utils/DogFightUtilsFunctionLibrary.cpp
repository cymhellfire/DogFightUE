#include "Utils/DogFightUtilsFunctionLibrary.h"

#define LUA_SCRIPT_BASE_PATH		TEXT("Script/")
#define LUA_TEMPLATE_BASE_PATH		TEXT("EditorResources/ScriptTemplates")

FString UDogFightUtilsFunctionLibrary::GenerateLuaScriptPath(const FLuaScriptCreateArgument& InArgument)
{
	FString NewFileName = InArgument.ScriptName + ".lua";
	if (InArgument.bUsePrefix)
	{
		NewFileName = InArgument.TemplateName + NewFileName;
	}

	if (!InArgument.Path.IsEmpty())
	{
		NewFileName = InArgument.Path / NewFileName;
	}
	return FString::Printf(TEXT("%s%s%s/%s/%s"), *FPaths::ProjectContentDir(), LUA_SCRIPT_BASE_PATH, *InArgument.ModuleName,
		*InArgument.TemplateName, *NewFileName);
}

FString UDogFightUtilsFunctionLibrary::CreateLuaScriptByTemplate(const FLuaScriptCreateArgument& InArgument)
{
	FString NewClassName = InArgument.ScriptName;
	if (InArgument.bUsePrefix)
	{
		NewClassName = InArgument.TemplateName + NewClassName;
	}
	FString NewScriptPath = GenerateLuaScriptPath(InArgument);

	// Check duplicated files
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.FileExists(*NewScriptPath))
	{
		return "File already exists!";
	}

	// Check template file
	FString TemplatePath = FString::Printf(TEXT("%s%s/%s.temp"), *FPaths::ProjectDir(), LUA_TEMPLATE_BASE_PATH, *InArgument.TemplateName);
	if (!PlatformFile.FileExists(*TemplatePath))
	{
		return "Template file doesn't exist!";
	}

	// Replace template file content
	FString Content;
	FFileHelper::LoadFileToString(Content, *TemplatePath);
	Content = Content.Replace(TEXT("$Class$"), *NewClassName);
	FFileHelper::SaveStringToFile(Content, *NewScriptPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);

	return "Success";
}
