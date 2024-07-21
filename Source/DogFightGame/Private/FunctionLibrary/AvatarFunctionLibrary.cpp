// Dog Fight Game Code By CYM.


#include "FunctionLibrary/AvatarFunctionLibrary.h"

#include "Common/DogFightGameLog.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameService/AvatarManagerService.h"
#include "GameService/GameService.h"
#include "Pawn/Component/CharacterAnimComponent.h"

void UAvatarFunctionLibrary::InitAvatarAppearanceWithConfigId(UObject* WorldContext, AActionGameCharacter* InAvatar,
	int32 ConfigId)
{
	if (!IsValid(WorldContext))
		return;

	if (!IsValid(InAvatar))
	{
		DFLogE(LogDogFightGame, TEXT("Invalid avatar actor to setup appearance."))
		return;
	}

	if (auto AvatarManagerService = UGameService::GetGameService<UAvatarManagerService>())
	{
		auto AssetPath = AvatarManagerService->GetAvatarDescAssetPathByConfigId(ConfigId);
		if (AssetPath.IsEmpty())
		{
			DFLogE(LogDogFightGame, TEXT("Invalid config id [%d] to get AvatarDesc asset."), ConfigId);
			return;
		}

		if (auto Asset = Cast<UAvatarDataAsset>(ULuaIntegrationFunctionLibrary::LoadObjectByPath(AssetPath)))
		{
			DFLog(LogDogFightGame, TEXT("Init avatar %s with config id %d"), *InAvatar->GetName(), ConfigId);
			InAvatar->SetupAvatarAppearanceWithAsset(Asset);
		}
		else
		{
			DFLogE(LogDogFightGame, TEXT("Failed to load avatar data asset with path: %s"), *AssetPath);
		}
	}
}

void UAvatarFunctionLibrary::PlayAnimationWithAvatar(AActionGameCharacter* InAvatar, const FString& AnimPath)
{
	if (!IsValid(InAvatar))
	{
		return;
	}

	FSoftObjectPath ResPath(AnimPath);
	UAnimMontage* AnimRes = Cast<UAnimMontage>(ResPath.IsValid() ? ResPath.ResolveObject() : ResPath.TryLoad());
	if (!IsValid(AnimRes))
	{
		return;
	}
	InAvatar->PlayAnimMontage(AnimRes);
}

void UAvatarFunctionLibrary::PlayPredefineAnimWithAvatar(AActionGameCharacter* InAvatar,
	EActionAnimPredefinedType::Type InType)
{
	if (!IsValid(InAvatar))
	{
		return;
	}

	if (auto AnimComponent = InAvatar->GetAnimComponent())
	{
		AnimComponent->MulticastPlayPredefineAnimation(InType);
	}
}
