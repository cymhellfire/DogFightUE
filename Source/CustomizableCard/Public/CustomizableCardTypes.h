#pragma once

#include "CustomizableCardTypes.generated.h"

#define ST_CARD_LOC				"/Game/DogFight/Localization/ST_Card.ST_Card"
#define ST_PROJECTILE_LOC		"/Game/DogFight/Localization/ST_Projectile.ST_Projectile"
#define ST_BUFF_LOC				"/Game/DogFight/Localization/ST_Buff.ST_Buff"

UENUM(BlueprintType)
enum class ECardInstructionExecuteType : uint8
{
	Synchronous,
	Asynchronous
};

UENUM(BlueprintType)
enum class ECardInstructionTargetType : uint8
{
	Actor,
	Position,
	Direction
};

USTRUCT(BlueprintType)
struct FCardInstructionTargetInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetInfo")
	ECardInstructionTargetType TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetInfo")
	AActor* ActorPtr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetInfo")
	FVector PositionValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetInfo")
	FVector DirectionValue;

	friend bool operator==(const FCardInstructionTargetInfo& Lhs, const FCardInstructionTargetInfo& RHS)
	{
		return Lhs.TargetType == RHS.TargetType
			&& Lhs.ActorPtr == RHS.ActorPtr
			&& Lhs.PositionValue == RHS.PositionValue
			&& Lhs.DirectionValue == RHS.DirectionValue;
	}

	friend bool operator!=(const FCardInstructionTargetInfo& Lhs, const FCardInstructionTargetInfo& RHS)
	{
		return !(Lhs == RHS);
	}
};

UENUM(BlueprintType)
enum class ECardDisplayInfoLocType : uint8
{
	ILT_Card			UMETA(DisplayName="Card", ToolTip="Use localization from ST_Card file."),
	ILT_Projectile		UMETA(DisplayName="Projectile", ToolTip="Use localization from ST_Projectile file."),
	ILT_Buff			UMETA(DisplayName="Buff", ToolTip="Use localization from ST_Buff file."),
	ILT_Raw				UMETA(DisplayName="Raw", ToolTip="Not use any localization."),
	ILT_RawNoStyle		UMETA(DisplayName="RawNoStyle", ToolTip="No localization and no style."),
	ILT_Image			UMETA(DisplayName="Image", ToolTip="Use image defined in DT_RichTextImageSet."),
};

USTRUCT(BlueprintType)
struct FCardDisplayInfoArgument
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDisplayArgument")
	FString DisplayStyle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDisplayArgument")
	FString StringValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDisplayArgument")
	ECardDisplayInfoLocType LocalizeType;

	FText GetLocalizedText() const
	{
		if (LocalizeType == ECardDisplayInfoLocType::ILT_Image)
		{
			return FText::FromString(FString::Printf(TEXT("<img id=\"%s\"/>"), *StringValue));
		}
		else if (LocalizeType == ECardDisplayInfoLocType::ILT_RawNoStyle)
		{
			return FText::FromString(StringValue);
		}

		FFormatOrderedArguments FormatArgumentValues;
		FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(FString::Printf(TEXT("<%s>"), *DisplayStyle))));
		switch(LocalizeType)
		{
		case ECardDisplayInfoLocType::ILT_Card:
			FormatArgumentValues.Add(FFormatArgumentValue(FText::FromStringTable(ST_CARD_LOC, StringValue)));
			break;
		case ECardDisplayInfoLocType::ILT_Projectile:
			FormatArgumentValues.Add(FFormatArgumentValue(FText::FromStringTable(ST_PROJECTILE_LOC, StringValue)));
			break;
		case ECardDisplayInfoLocType::ILT_Buff:
			FormatArgumentValues.Add(FFormatArgumentValue(FText::FromStringTable(ST_BUFF_LOC, StringValue)));
			break;
		case ECardDisplayInfoLocType::ILT_Raw:
			FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(StringValue)));
			break;
		default:
			FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(TEXT("Invalid Argument"))));
			break;
		}
		FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(TEXT("</>"))));

		return FText::Format(FText::FromStringTable(ST_CARD_LOC, TEXT("ArgumentFormatString")), FormatArgumentValues);
	}
};

USTRUCT(BlueprintType)
struct FCardDescriptionItemInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDescriptionItem")
	FString StringValue;

	/** Arguments will be used by main string formatting. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDescriptionItem")
	TArray<FCardDisplayInfoArgument> Arguments;

	/** Additional arguments which are not used by main string formatting. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDescriptionItem")
	TArray<FCardDisplayInfoArgument> ExtraArguments;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDescriptionItem")
	ECardDisplayInfoLocType LocalizeType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDescriptionItem")
	UClass* ItemWidgetClass;

	FText GetLocalizedText() const
	{
		FFormatOrderedArguments FormatArgumentValues;
		for (FCardDisplayInfoArgument Argument : Arguments)
		{
			FormatArgumentValues.Add(Argument.GetLocalizedText());
		}

		switch (LocalizeType)
		{
		case ECardDisplayInfoLocType::ILT_Card:
			return FText::Format(FText::FromStringTable(ST_CARD_LOC, StringValue), FormatArgumentValues);
		case ECardDisplayInfoLocType::ILT_Projectile:
			return FText::Format(FText::FromStringTable(ST_PROJECTILE_LOC, StringValue), FormatArgumentValues);
		case ECardDisplayInfoLocType::ILT_Buff:
			return FText::Format(FText::FromStringTable(ST_BUFF_LOC, StringValue), FormatArgumentValues);
		case ECardDisplayInfoLocType::ILT_Raw:
			return FText::Format(FText::FromString(StringValue), FormatArgumentValues);
		case ECardDisplayInfoLocType::ILT_RawNoStyle:
			return FText::FromString(StringValue);
		case ECardDisplayInfoLocType::ILT_Image:
			return FText::FromString(FString::Printf(TEXT("<img id=\"%s\"/>"), *StringValue));
		default: ;
		}

		return FText();
	}

	FText GetLocalizedExtraArgumentText(int32 Index) const
	{
		if (Index >= 0 && Index < ExtraArguments.Num())
		{
			return ExtraArguments[Index].GetLocalizedText();
		}

		return FText();
	}
};

USTRUCT(BlueprintType)
struct FCardInstanceDisplayInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardInstanceInfo")
	FString CardName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardInstanceInfo")
	TArray<FCardDisplayInfoArgument> NameArguments;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardInstanceInfo")
	FString CardDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardInstanceInfo")
	TArray<FCardDisplayInfoArgument> DescArguments;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardInstanceInfo")
	TArray<FCardDescriptionItemInfo> DescExtraItems;

	FText GetCardNameText() const
	{
		FFormatOrderedArguments FormatArgumentValues;
		for (FCardDisplayInfoArgument Argument : NameArguments)
		{
			FormatArgumentValues.Add(Argument.GetLocalizedText());
		}

		return FText::Format(FText::FromStringTable(ST_CARD_LOC, CardName), FormatArgumentValues);
	}

	FText GetCardDescriptionText() const
	{
		FFormatOrderedArguments FormatArgumentValues;
		for (FCardDisplayInfoArgument Argument : DescArguments)
		{
			FormatArgumentValues.Add(Argument.GetLocalizedText());
		}

		return FText::Format(FText::FromStringTable(ST_CARD_LOC, CardDescription), FormatArgumentValues);
	}
};

/** Test if all desired flags are toggled on. */
#define TEST_MULTI_CARD_CATEGORY(Bitmask, DesiredFlags) (((Bitmask) & (DesiredFlags)) == (DesiredFlags))

UENUM(Meta = (Bitflags))
enum class ECardCategoryFlags
{
	ECC_Attack			UMETA(DisplayName = "Attack"),
	ECC_Defence			UMETA(DisplayName = "Defence"),
	ECC_Support			UMETA(DisplayName = "Support")
};
