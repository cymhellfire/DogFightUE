// Dog Fight Game Code By CYM.


#include "GameObject/Weapon/DogFightWeapon.h"

#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/ActionCharacterInterface.h"

void UDogFightWeapon::OnHitTarget(AActor* TargetActor, UPrimitiveComponent* TargetComponent, const FHitResult& HitResult)
{
	Super::OnHitTarget(TargetActor, TargetComponent, HitResult);

	UCommonGameplayFunctionLibrary::DamageActor(this, 1, TargetActor, 5,
		OwnerCharacter->AsCharacter());
}
