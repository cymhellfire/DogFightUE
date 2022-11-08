#pragma once

#include "Styling/SlateStyle.h"

class FDogFightEditorStyle : public FSlateStyleSet
{
public:
	FDogFightEditorStyle();
	~FDogFightEditorStyle();

	static TSharedPtr<ISlateStyle> GetInstance()
	{
		if (!Instance.IsValid())
		{
			Instance = MakeShared<FDogFightEditorStyle>();
		}

		return Instance;
	}

private:
	static TSharedPtr<ISlateStyle> Instance;
};
