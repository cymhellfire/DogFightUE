#include "Utils/DogFightUtilsFunctionLibrary.h"

#define LUA_SCRIPT_BASE_PATH		TEXT("Script/DogFight")
#define LUA_TEMPLATE_BASE_PATH		TEXT("EditorResources/ScriptTemplates")

FString UDogFightUtilsFunctionLibrary::CreateLuaScriptByTemplate(FString Template, FString Path, FString ScriptName)
{
	const FString NewClassName = Template + ScriptName;
	FString NewFileName = NewClassName + ".lua";
	if (!Path.IsEmpty())
	{
		NewFileName = Path / NewFileName;
	}
	FString NewScriptPath = FString::Printf(TEXT("%s%s/%s/%s"), *FPaths::ProjectContentDir(), LUA_SCRIPT_BASE_PATH,
		*Template, *NewFileName);

	// Check duplicated files
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.FileExists(*NewScriptPath))
	{
		return "File already exists!";
	}

	// Check template file
	FString TemplatePath = FString::Printf(TEXT("%s%s/%s.temp"), *FPaths::ProjectDir(), LUA_TEMPLATE_BASE_PATH, *Template);
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
