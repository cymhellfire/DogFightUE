#pragma once

#include "Common/Localization.h"
#include "GameCardTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameCards, Log, All);

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

UENUM(BlueprintType)
enum class EUpgradableFloatConvertType : uint8
{
	Default,
	Percentage,
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

#pragma region Card Display Info
UENUM(BlueprintType)
enum class ECardDisplayInfoLocType : uint8
{
	ILT_Card			UMETA(DisplayName="Card", ToolTip="Use localization from ST_Card file."),
	ILT_Projectile		UMETA(DisplayName="Projectile", ToolTip="Use localization from ST_Projectile file."),
	ILT_Buff			UMETA(DisplayName="Buff", ToolTip="Use localization from ST_Buff file."),
	ILT_Raw				UMETA(DisplayName="Raw", ToolTip="Not use any localization."),
	ILT_Image			UMETA(DisplayName="Image", ToolTip="Use image defined in DT_RichTextImageSet."),
	ILT_CardEnhance		UMETA(DisplayName="CardEnhancement", ToolTip="Use localization from ST_CardEnhance."),
	ILT_Weapon			UMETA(DisplayName="Weapon", ToolTip="Use localization from ST_Weapon."),
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardDisplayArgument")
	bool DisableRichTextStyle;

	FText GetLocalizedText() const
	{
		if (LocalizeType == ECardDisplayInfoLocType::ILT_Image)
		{
			return FText::FromString(FString::Printf(TEXT("<img id=\"%s\"/>"), *StringValue));
		}

		// Get localized text based on type
		FText LocalizedText;
		switch(LocalizeType)
		{
		case ECardDisplayInfoLocType::ILT_Card:
			LocalizedText = FText::FromStringTable(ST_CARD_LOC, StringValue);
			break;
		case ECardDisplayInfoLocType::ILT_Projectile:
			LocalizedText = FText::FromStringTable(ST_PROJECTILE_LOC, StringValue);
			break;
		case ECardDisplayInfoLocType::ILT_Buff:
			LocalizedText = FText::FromStringTable(ST_BUFF_LOC, StringValue);
			break;
		case ECardDisplayInfoLocType::ILT_Raw:
			LocalizedText = FText::FromString(StringValue);
			break;
		case ECardDisplayInfoLocType::ILT_CardEnhance:
			LocalizedText = FText::FromStringTable(ST_CARD_ENHANCE_LOC, StringValue);
			break;
		case ECardDisplayInfoLocType::ILT_Weapon:
			LocalizedText = FText::FromStringTable(ST_WEAPON_LOC, StringValue);
			break;
		default:
			LocalizedText = FText::FromString(TEXT("Invalid Argument"));
			break;
		}

		if (DisableRichTextStyle)
		{
			return LocalizedText;
		}

		// Surround by style
		FFormatOrderedArguments FormatArgumentValues;
		FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(FString::Printf(TEXT("<%s>"), *DisplayStyle))));
		FormatArgumentValues.Add(FFormatArgumentValue(LocalizedText));
		FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(TEXT("</>"))));

		return FText::Format(FText::FromStringTable(ST_CARD_LOC, TEXT("ArgumentFormatString")), FormatArgumentValues);
	}
};

USTRUCT(BlueprintType)
struct FUpgradablePropertyDisplayInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PropertyDisplayInfo")
	FString StringValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PropertyDisplayInfo")
	ECardDisplayInfoLocType LocalizeType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PropertyDisplayInfo")
	TArray<FCardDisplayInfoArgument> ValueArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PropertyDisplayInfo")
	int32 CurrentLevel;

	FText GetLocalizedText() const
	{
		switch (LocalizeType)
		{
		case ECardDisplayInfoLocType::ILT_Card:
			return FText::FromStringTable(ST_CARD_LOC, StringValue);
		case ECardDisplayInfoLocType::ILT_Projectile: 
			return FText::FromStringTable(ST_PROJECTILE_LOC, StringValue);
		case ECardDisplayInfoLocType::ILT_Buff: 
			return FText::FromStringTable(ST_BUFF_LOC, StringValue);
		case ECardDisplayInfoLocType::ILT_Raw: 
			return FText::FromString(StringValue);
		case ECardDisplayInfoLocType::ILT_CardEnhance:
			return FText::FromStringTable(ST_CARD_ENHANCE_LOC, StringValue);
		case ECardDisplayInfoLocType::ILT_Weapon:
			return FText::FromStringTable(ST_WEAPON_LOC, StringValue);
		case ECardDisplayInfoLocType::ILT_Image:
		default: ;
		}

		return FText();
	}

	FText GetLocalizedArgument(int32 Index) const
	{
		if (Index < 0 || Index >= ValueArray.Num())
			return FText();

		return ValueArray[Index].GetLocalizedText();
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardInstanceInfo")
	TArray<FUpgradablePropertyDisplayInfo> PropertyInfoList;

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
		case ECardDisplayInfoLocType::ILT_Image:
			return FText::FromString(FString::Printf(TEXT("<img id=\"%s\"/>"), *StringValue));
		case ECardDisplayInfoLocType::ILT_CardEnhance:
			return FText::Format(FText::FromStringTable(ST_CARD_ENHANCE_LOC, StringValue), FormatArgumentValues);
		case ECardDisplayInfoLocType::ILT_Weapon:
			return FText::Format(FText::FromStringTable(ST_WEAPON_LOC, StringValue), FormatArgumentValues);
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

	FUpgradablePropertyDisplayInfo GetPropertyDisplayInfo(int32 PropertyIndex) const
	{
		if (PropertyIndex >= 0 && PropertyIndex < PropertyInfoList.Num())
		{
			return PropertyInfoList[PropertyIndex];
		}

		UE_LOG(LogInit, Error, TEXT("Invalid property index %d given."), PropertyIndex);
		return FUpgradablePropertyDisplayInfo();
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CardInstanceInfo")
	TArray<FString> CardEnhancePrefix;

	FText GetCardNameText() const
	{
		FFormatOrderedArguments FormatArgumentValues;
		for (FCardDisplayInfoArgument Argument : NameArguments)
		{
			FormatArgumentValues.Add(Argument.GetLocalizedText());
		}

		FText LocalizedCardName = FText::Format(FText::FromStringTable(ST_CARD_LOC, CardName), FormatArgumentValues);

		// Append all enhance prefix to card name
		if (CardEnhancePrefix.Num() > 0)
		{
			for (FString Prefix : CardEnhancePrefix)
			{
				LocalizedCardName = FText::Format(FText::FromStringTable(ST_CARD_LOC, TEXT("AddPrefixFormatString")),
					LocalizedCardName, FText::FromStringTable(ST_CARD_ENHANCE_LOC, Prefix));
			}
		}

		return LocalizedCardName;
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
#pragma endregion Card Display Info

/** Test if all desired flags are toggled on. */
#define TEST_MULTI_CARD_CATEGORY(Bitmask, DesiredFlags) (((Bitmask) & (DesiredFlags)) == (DesiredFlags))

UENUM(Meta = (Bitflags))
enum class ECardCategoryFlags
{
	ECC_Attack			UMETA(DisplayName = "Attack"),
	ECC_Defence			UMETA(DisplayName = "Defence"),
	ECC_Support			UMETA(DisplayName = "Support")
};

UENUM(BlueprintType)
enum class ECardUseMethod : uint8
{
	CUM_Aggressive		UMETA(DisplayName="Aggressive", Tooltip="This card can be used in player's round."),
	CUM_Passive			UMETA(DisplayName="Passive", Tooltip="This card can only be used when requested."),
};

#pragma region Upgradable Property

USTRUCT(BlueprintType)
struct FUpgradableIntProperty
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty")
	FString LocalizationString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty")
	ECardDisplayInfoLocType LocalizeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty")
	TArray<int32> ValueArray;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty", meta=(ClampMin=1, AllowPrivateAccess=true))
	int32 CurrentLevel;

public:

	FUpgradableIntProperty() : CurrentLevel(1)
	{
		LocalizeType = ECardDisplayInfoLocType::ILT_Card;
		ValueArray.Add(0);
	}

	FUpgradableIntProperty(int32 DefaultValue) : CurrentLevel(1)
	{
		LocalizeType = ECardDisplayInfoLocType::ILT_Card;
		ValueArray.Add(DefaultValue);
	}

	FUpgradableIntProperty(int32 DefaultValue, FString LocalizeKey, ECardDisplayInfoLocType LocType) : CurrentLevel(1)
	{
		LocalizationString = LocalizeKey;
		LocalizeType = LocType;
		ValueArray.Add(DefaultValue);
	}

	int32 GetValue() const
	{
		return ValueArray[CurrentLevel - 1];
	}

	FORCEINLINE int32 GetCurrentLevel() const
	{
		return CurrentLevel;
	}

	void SetLevel(int32 NewLevel)
	{
		CurrentLevel = FMath::Clamp(NewLevel, 1, ValueArray.Num());
	}

	FUpgradablePropertyDisplayInfo GetDisplayInfo() const
	{
		FUpgradablePropertyDisplayInfo DisplayInfo;
		DisplayInfo.StringValue = LocalizationString;
		DisplayInfo.LocalizeType = LocalizeType;
		DisplayInfo.CurrentLevel = CurrentLevel;
		TArray<FCardDisplayInfoArgument> Arguments;
		for (int32 Value : ValueArray)
		{
			Arguments.Add(FCardDisplayInfoArgument{
				TEXT("Default"),
				FString::Printf(TEXT("%d"), Value),
				ECardDisplayInfoLocType::ILT_Raw,
				true
			});
		}
		DisplayInfo.ValueArray = Arguments;

		return DisplayInfo;
	}
};

USTRUCT(BlueprintType)
struct FUpgradableFloatProperty
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty")
	FString LocalizationString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty")
	ECardDisplayInfoLocType LocalizeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty")
	EUpgradableFloatConvertType ValueConvertType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty")
	TArray<float> ValueArray;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UpgradableProperty", meta=(ClampMin="1", AllowPrivateAccess=true))
	int32 CurrentLevel;

public:

	FUpgradableFloatProperty() : CurrentLevel(1)
	{
		LocalizeType = ECardDisplayInfoLocType::ILT_Card;
		ValueConvertType = EUpgradableFloatConvertType::Default;
		ValueArray.Add(0);
	}

	FUpgradableFloatProperty(float DefaultValue) : CurrentLevel(1)
	{
		LocalizeType = ECardDisplayInfoLocType::ILT_Card;
		ValueConvertType = EUpgradableFloatConvertType::Default;
		ValueArray.Add(DefaultValue);
	}

	FUpgradableFloatProperty(float DefaultValue, FString LocalizeKey, ECardDisplayInfoLocType LocType) : CurrentLevel(1)
	{
		LocalizationString = LocalizeKey;
		ValueConvertType = EUpgradableFloatConvertType::Default;
		LocalizeType = LocType;
		ValueArray.Add(DefaultValue);
	}

	float GetValue() const
	{
		return ValueArray[CurrentLevel - 1];
	}

	FORCEINLINE int32 GetCurrentLevel() const
	{
		return CurrentLevel;
	}

	void SetLevel(int32 NewLevel)
	{
		CurrentLevel = FMath::Clamp(NewLevel, 1, ValueArray.Num());
	}

	FUpgradablePropertyDisplayInfo GetDisplayInfo() const
	{
		FUpgradablePropertyDisplayInfo DisplayInfo;
		DisplayInfo.StringValue = LocalizationString;
		DisplayInfo.LocalizeType = LocalizeType;
		DisplayInfo.CurrentLevel = CurrentLevel;
		TArray<FCardDisplayInfoArgument> Arguments;
		for (float Value : ValueArray)
		{
			Arguments.Add(FCardDisplayInfoArgument{
				TEXT("Default"),
				ConvertValueToString(Value),
				ECardDisplayInfoLocType::ILT_Raw,
				true
			});
		}
		DisplayInfo.ValueArray = Arguments;

		return DisplayInfo;
	}

	FString ConvertValueToString(float Value) const
	{
		const float Percentage = Value * 100.f;
		switch(ValueConvertType)
		{
			case EUpgradableFloatConvertType::Percentage:
				return FString::Printf(TEXT("%.0f%%"), Percentage);
			default:
				return FString::Printf(TEXT("%.2f"), Value);
		}
	}
};

#pragma endregion Upgradable Property
