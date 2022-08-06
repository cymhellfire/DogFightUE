#include "TinyXmlDocument.h"

#include "TinyXmlElement.h"
#include "UnrealTinyXml.h"
#include "HAL/PlatformFileManager.h"
#include "src/tinyxml.h"

FTinyXmlDocument::FTinyXmlDocument()
{
	bNewCreated = true;
	XmlDocument = MakeShareable(new TiXmlDocument());
}

FTinyXmlDocument::FTinyXmlDocument(FString FilePath)
{
	bNewCreated = false;
	XmlDocument = MakeShareable(new TiXmlDocument(TCHAR_TO_ANSI(*FilePath)));

	// Load specified file
	LoadFile();
}

FTinyXmlDocument::~FTinyXmlDocument()
{
	XmlDocument.Reset();
}

void FTinyXmlDocument::LoadFile()
{
	XmlDocument->LoadFile(TIXML_ENCODING_UTF8);

	// Get the root element once loaded
	if (TiXmlElement* RawRootElement = XmlDocument->RootElement())
	{
		RootElement = MakeShareable(new FTinyXmlElement(RawRootElement));
	}
}

bool FTinyXmlDocument::SaveFile()
{
	if (bNewCreated)
	{
		UE_LOG(LogUnrealTinyXml, Error, TEXT("New created file must have a valid path to save."));
		return false;
	}

	return XmlDocument->SaveFile();
}

bool FTinyXmlDocument::SaveFile(FString FilePath)
{
	if (FilePath.IsEmpty())
	{
		UE_LOG(LogUnrealTinyXml, Error, TEXT("Empty file path is invalid."));
		return false;
	}

	// Ensure file name extension
	if (!FilePath.EndsWith(".xml", ESearchCase::IgnoreCase))
	{
		FilePath.Append(".xml");
	}

	return XmlDocument->SaveFile(TCHAR_TO_UTF8(*FilePath));
}

TSharedPtr<FTinyXmlElement> FTinyXmlDocument::GetRootElement()
{
	return RootElement;
}

TSharedPtr<FTinyXmlElement> FTinyXmlDocument::AddRootElement(FString InName)
{
	if (RootElement.IsValid())
	{
		return GetRootElement();
	}

	TiXmlElement* NewRoot = new TiXmlElement(TCHAR_TO_UTF8(*InName));
	XmlDocument->LinkEndChild(NewRoot);

	RootElement = MakeShareable(new FTinyXmlElement(NewRoot));
	return RootElement;
}

TSharedPtr<FTinyXmlDocument> FTinyXmlDocument::NewFile()
{
	TSharedPtr<FTinyXmlDocument> NewDocument = MakeShareable(new FTinyXmlDocument());

	// Initial new created document
	TiXmlDeclaration* Decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	NewDocument->XmlDocument->LinkEndChild(Decl);

	return NewDocument;
}

TSharedPtr<FTinyXmlDocument> FTinyXmlDocument::LoadFile(FString FilePath)
{
	// Check passed in file path
	if (FilePath.IsEmpty())
	{
		UE_LOG(LogUnrealTinyXml, Log, TEXT("Empty file path is not valid."));
		return nullptr;
	}

	// Check whether the file is exist
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.FileExists(*FilePath))
	{
		UE_LOG(LogUnrealTinyXml, Error, TEXT("File with path %s is not exist."), *FilePath);
		return nullptr;
	}

	return MakeShareable(new FTinyXmlDocument(FilePath));
}
