// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionPlayAnimation.h"
#include "Card/CardBase.h"
#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"
#include "GameFramework/Character.h"
#include "Animation/AnimNotify_Delegate.h"
#include "Actors/Interfaces/GameAnimatedCharacterInterface.h"

UInstructionPlayAnimation::UInstructionPlayAnimation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = "PlayAnimation";

	CameraFocusType = EInstructionCameraFocusType::ICFT_User;
}

void UInstructionPlayAnimation::Execute()
{
	if (IGameCardUserPlayerControllerInterface* CardUserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(GetOwnerCard()->GetOwnerPlayerController()))
	{
		if (ACharacter* UserCharacter = Cast<ACharacter>(CardUserPlayerController->GetActualPawn()))
		{
			// Bind all actions
			BindAllDelegateActions();

			// Play montage
			if (IGameAnimatedCharacterInterface* AnimatedCharacter = Cast<IGameAnimatedCharacterInterface>(UserCharacter))
			{
				const float Duration = AnimatedCharacter->PlayMontage(MontageToPlay);
				if (Duration == 0.f)
				{
					UE_LOG(LogGameCards, Error, TEXT("Failed to play montage: %s"), *MontageToPlay->GetPathName());

					Finish();
				}
				else
				{
					UWorld* World = GetWorld();
					World->GetTimerManager().SetTimer(AnimationWaitingTimerHandle, this, &UInstructionPlayAnimation::OnAnimationFinished, Duration);
				}
			}
		}
	}
}

void UInstructionPlayAnimation::BeginDestroy()
{
	// Clear actions
	DelegateActions.Empty();

	Super::BeginDestroy();
}

void UInstructionPlayAnimation::AddAnimNotifyDelegateAction(const FAnimNotifyDelegateAction& NewAction)
{
	DelegateActions.Add(NewAction);

	// Also link the instruction to owning card to avoid be destructed by GC
	if (IsValid(NewAction.InstructionToExecute))
	{
		OwnerCard->AddExtraCardInstruction(NewAction.InstructionToExecute);
		NewAction.InstructionToExecute->SetOwnerCard(OwnerCard);
	}
}

void UInstructionPlayAnimation::BindAllDelegateActions()
{
	if (!IsValid(MontageToPlay))
	{
		return;
	}

	// Collect all AnimNotify_Delegate instances
	TMap<EAnimNotifyDelegateType, TArray<UAnimNotify_Delegate*>> NotifyMap;
	for (FAnimNotifyEvent Notify : MontageToPlay->Notifies)
	{
		UAnimNotify* AnimNotify = Notify.Notify;
		// Filter notifies based on type
		if (UAnimNotify_Delegate* DelegateNotify = Cast<UAnimNotify_Delegate>(AnimNotify))
		{
			NotifyMap.FindOrAdd(DelegateNotify->DelegateType).Add(DelegateNotify);

			// Bind callback
			DelegateNotify->OnAnimNotifyTriggered.AddDynamic(this, &UInstructionPlayAnimation::OnAnimNotifyDelegateTriggered);
		}
	}

	// Match action with notify
	for (FAnimNotifyDelegateAction Action : DelegateActions)
	{
		// Find by delegate type
		if (TArray<UAnimNotify_Delegate*>* NotifyArray = NotifyMap.Find(Action.TargetDelegateType))
		{
			// Check desired index
			if (Action.DesireIndex < NotifyArray->Num())
			{
				TArray<UAnimNotify_Delegate*> Array = *NotifyArray;
				// Record matching result into map
				ActionMap.FindOrAdd(Array[Action.DesireIndex]).Add(Action);
			}
		}
	}
}

void UInstructionPlayAnimation::OnAnimNotifyDelegateTriggered(UAnimNotify_Delegate* NotifyInstance)
{
	// Unregister callback
	NotifyInstance->OnAnimNotifyTriggered.RemoveDynamic(this, &UInstructionPlayAnimation::OnAnimNotifyDelegateTriggered);

	// Check ActionMap
	if (TArray<FAnimNotifyDelegateAction>* ActionList = ActionMap.Find(NotifyInstance))
	{
		// Execute all bound actions
		for (FAnimNotifyDelegateAction DelegateAction : *ActionList)
		{
			// Execute the instruction
			if (UCardInstructionBase* Instruction = DelegateAction.InstructionToExecute)
			{
				Instruction->BeginExecute();
			}
		}
	}
}

void UInstructionPlayAnimation::OnAnimationFinished()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(AnimationWaitingTimerHandle);
	}

	Finish();
}


