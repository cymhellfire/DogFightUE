#pragma once

#include "CoreMinimal.h"

class TiXmlDocument;
class FTinyXmlElement;

class UNREALTINYXML_API FTinyXmlDocument
{
public:
	~FTinyXmlDocument();

	static TSharedPtr<FTinyXmlDocument> NewFile();
	static TSharedPtr<FTinyXmlDocument> LoadFile(FString FilePath);

	void LoadFile();

	bool SaveFile();
	bool SaveFile(FString FilePath);

	TSharedPtr<FTinyXmlElement> GetRootElement();

	TSharedPtr<FTinyXmlElement> AddRootElement(FString InName);

protected:
	FTinyXmlDocument();
	FTinyXmlDocument(FString FilePath);

private:
	bool bNewCreated;

	TSharedPtr<TiXmlDocument> XmlDocument;
	TSharedPtr<FTinyXmlElement> RootElement;
};
