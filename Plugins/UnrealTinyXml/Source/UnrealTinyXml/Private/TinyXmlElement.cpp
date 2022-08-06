#include "TinyXmlElement.h"

#include "src/tinyxml.h"

FTinyXmlElement::FTinyXmlElement()
	: FTinyXmlElement("")
{
}

FTinyXmlElement::FTinyXmlElement(FString Name)
{
	RawNode = new TiXmlElement(TCHAR_TO_UTF8(*Name));
}

FTinyXmlElement::FTinyXmlElement(TiXmlElement* InNode)
	: RawNode(InNode)
{
	
}

FTinyXmlElement::~FTinyXmlElement()
{
	// Free memory if raw node has no parent node
	if (RawNode->Parent() == nullptr)
	{
		delete RawNode;
	}
}

FString FTinyXmlElement::GetValue() const
{
	return FString(UTF8_TO_TCHAR(RawNode->Value()));
}

void FTinyXmlElement::SetValue(FString InValue)
{
	RawNode->SetValue(TCHAR_TO_UTF8(*InValue));
}

bool FTinyXmlElement::GetBoolAttribute(FString AttrName, bool* OutValue) const
{
	const int Result = RawNode->QueryBoolAttribute(TCHAR_TO_UTF8(*AttrName), OutValue);

	return Result == TIXML_SUCCESS;
}

bool FTinyXmlElement::GetIntAttribute(FString AttrName, int32* OutValue) const
{
	const int Result = RawNode->QueryIntAttribute(TCHAR_TO_UTF8(*AttrName), OutValue);

	return Result == TIXML_SUCCESS;
}

bool FTinyXmlElement::GetFloatAttribute(FString AttrName, float* OutValue) const
{
	const int Result = RawNode->QueryFloatAttribute(TCHAR_TO_UTF8(*AttrName), OutValue);

	return Result == TIXML_SUCCESS;
}

bool FTinyXmlElement::GetStringAttribute(FString AttrName, FString* OutValue) const
{
	const char* Result = RawNode->Attribute(TCHAR_TO_UTF8(*AttrName));
	if (Result == nullptr)
	{
		return false;
	}

	*OutValue = FString(UTF8_TO_TCHAR(Result));
	return true;
}

void FTinyXmlElement::SetBoolAttribute(FString AttrName, bool InValue)
{
	const FString StringValue = InValue ? TEXT("true") : TEXT("false");
	SetStringAttribute(AttrName, StringValue);
}

void FTinyXmlElement::SetIntAttribute(FString AttrName, int32 InValue)
{
	RawNode->SetAttribute(TCHAR_TO_UTF8(*AttrName), InValue);
}

void FTinyXmlElement::SetFloatAttribute(FString AttrName, float InValue)
{
	const FString StringValue = FString::Printf(TEXT("%f"), InValue);
	SetStringAttribute(AttrName, StringValue);
}

void FTinyXmlElement::SetStringAttribute(FString AttrName, FString InValue)
{
	RawNode->SetAttribute(TCHAR_TO_UTF8(*AttrName), TCHAR_TO_UTF8(*InValue));
}

TSharedPtr<FTinyXmlElement> FTinyXmlElement::GetFirstChild()
{
	return MakeShareable(new FTinyXmlElement(RawNode->FirstChildElement()));
}

void FTinyXmlElement::AppendChild(TWeakPtr<FTinyXmlElement> InChild)
{
	if (InChild.IsValid())
	{
		TSharedPtr<FTinyXmlElement> PinElement = InChild.Pin();
		RawNode->LinkEndChild(PinElement->RawNode);
		PinElement->SetParentNode(this->AsWeak());
	}
}
