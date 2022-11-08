#include "DogFightEditorStyle.h"

#include "Actors/Buffs/BasicArmorBuff.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<ISlateStyle> FDogFightEditorStyle::Instance = nullptr;

FDogFightEditorStyle::FDogFightEditorStyle()
	: FSlateStyleSet("DogFightEditorStyle")
{
	const FVector2D Icon40x40(40.0f, 40.0f);

	SetContentRoot(FPaths::ProjectDir() / TEXT("EditorResources"));

	Set("DogFightEditor.DogFightLogo", new IMAGE_BRUSH("Icons/Icon_DogFight", Icon40x40));

	FSlateStyleRegistry::RegisterSlateStyle(*this);
}

FDogFightEditorStyle::~FDogFightEditorStyle()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}

#undef IMAGE_BRUSH
