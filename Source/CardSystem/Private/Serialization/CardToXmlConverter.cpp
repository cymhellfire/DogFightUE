#include "Serialization/CardToXmlConverter.h"
#include "TinyXmlDocument.h"
#include "TinyXmlElement.h"

void FCardToXmlConverter::ExportTest()
{
	TSharedPtr<FTinyXmlDocument> NewDocument = FTinyXmlDocument::NewFile();
	TSharedPtr<FTinyXmlElement> Root = NewDocument->AddRootElement("Card");

	TSharedPtr<FTinyXmlElement> NewElement = MakeShareable(new FTinyXmlElement("Command"));
	NewElement->SetIntAttribute("ID", 2);

	Root->AppendChild(NewElement);

	FString SavePath = FPaths::Combine(FPaths::ProjectSavedDir(), "ExportXml");
	NewDocument->SaveFile(SavePath);
}
