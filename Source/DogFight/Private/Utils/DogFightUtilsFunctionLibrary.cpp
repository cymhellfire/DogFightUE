#include "Utils/DogFightUtilsFunctionLibrary.h"

#define LUA_SCRIPT_BASE_PATH		TEXT("Script/")
#define LUA_TEMPLATE_BASE_PATH		TEXT("EditorResources/ScriptTemplates")

FString UDogFightUtilsFunctionLibrary::GenerateLuaScriptFolder(const FLuaScriptCreateArgument& InArgument)
{
	FString SubFolder = InArgument.bOverrideTemplateFolder ? InArgument.OverrideFolder : InArgument.TemplateName;
	SubFolder = "/" + SubFolder;

	if (!InArgument.Path.IsEmpty())
	{
		SubFolder = FString::Printf(TEXT("/%s%s"), *InArgument.Path , *SubFolder);
	}

	return FString::Printf(TEXT("%s%s%s%s"), *FPaths::ProjectContentDir(), LUA_SCRIPT_BASE_PATH, *InArgument.ModuleName,
	*SubFolder);
}

FString UDogFightUtilsFunctionLibrary::GenerateLuaScriptPath(const FLuaScriptCreateArgument& InArgument)
{
	FString NewFileName = InArgument.ScriptName;
	switch (InArgument.TemplateMode)
	{
		case ELuaScriptNameTemplateMode::TM_Prefix:
			NewFileName = InArgument.TemplateOutName + NewFileName;
			break;
		case ELuaScriptNameTemplateMode::TM_Suffix:
			NewFileName = NewFileName + InArgument.TemplateOutName ;
			break;
		case ELuaScriptNameTemplateMode::TM_None:
		default: ;
	}
	NewFileName += ".lua";

	FString FinalFolder = GenerateLuaScriptFolder(InArgument);

	return FString::Printf(TEXT("%s/%s"), *FinalFolder, *NewFileName);
}

FString UDogFightUtilsFunctionLibrary::CreateLuaScriptByTemplate(const FLuaScriptCreateArgument& InArgument)
{
	FString NewClassName = InArgument.ScriptName;
	switch (InArgument.TemplateMode)
	{
	case ELuaScriptNameTemplateMode::TM_Prefix:
		NewClassName = InArgument.TemplateOutName + NewClassName;
		break;
	case ELuaScriptNameTemplateMode::TM_Suffix:
		NewClassName = NewClassName + InArgument.TemplateOutName ;
		break;
	case ELuaScriptNameTemplateMode::TM_None:
	default: ;
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

	// Prepare the $Path$ variable
	FString OutputFolder = GenerateLuaScriptFolder(InArgument);
	FString ScriptRoot = FString::Printf(TEXT("%s%s"), *FPaths::ProjectContentDir(), LUA_SCRIPT_BASE_PATH);
	if (!FPaths::MakePathRelativeTo(OutputFolder, *ScriptRoot))
	{
		return "Failed to convert script path to relative.";
	}
	OutputFolder.ReplaceCharInline('/', '.');

	// Replace template file content
	FString Content;
	FFileHelper::LoadFileToString(Content, *TemplatePath);
	Content = Content.Replace(TEXT("$Class$"), *NewClassName);
	Content = Content.Replace(TEXT("$Path$"), *OutputFolder);
	Content = Content.Replace(TEXT("$ClassPure$"), *InArgument.ScriptName);
	FFileHelper::SaveStringToFile(Content, *NewScriptPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);

	return "Success";
}
