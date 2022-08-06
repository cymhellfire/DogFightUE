#pragma once

#include "CoreMinimal.h"

class TiXmlElement;

class UNREALTINYXML_API FTinyXmlElement : public TSharedFromThis<FTinyXmlElement>
{
public:
	FTinyXmlElement();

	FTinyXmlElement(FString Name);
	FTinyXmlElement(TiXmlElement* InNode);

	~FTinyXmlElement();

	// ------------- Value operation -----------------

	FString GetValue() const;
	void SetValue(FString InValue);

	// ------------- Attribute operation ------------------

	bool GetBoolAttribute(FString AttrName, bool* OutValue) const;
	bool GetIntAttribute(FString AttrName, int32* OutValue) const;
	bool GetFloatAttribute(FString AttrName, float* OutValue) const;
	bool GetStringAttribute(FString AttrName, FString* OutValue) const;

	void SetBoolAttribute(FString AttrName, bool InValue);
	void SetIntAttribute(FString AttrName, int32 InValue);
	void SetFloatAttribute(FString AttrName, float InValue);
	void SetStringAttribute(FString AttrName, FString InValue);

	// ------------ Child operation -------------
	TSharedPtr<FTinyXmlElement> GetFirstChild();
	void AppendChild(TWeakPtr<FTinyXmlElement> InChild);

	void SetParentNode(TWeakPtr<FTinyXmlElement> InParent)
	{
		ParentNode = InParent;
	}

protected:
	TiXmlElement* RawNode;
	TWeakPtr<FTinyXmlElement> ParentNode;

};

