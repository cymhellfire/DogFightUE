// Dog Fight Game Code By CYM.


#include "Actors/Buffs/Buff_LuckyTester.h"
#include "Actors/Projectiles/TriggerableProjectile.h"
#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"

ABuff_LuckyTester::ABuff_LuckyTester()
{
	TesterDamage = 5.f;
	TesterDamageRadius = 100.f;
}

void ABuff_LuckyTester::ApplyBuff()
{
	Super::ApplyBuff();

	if (IsValid(TesterActorClass))
	{
		TesterActor = GetWorld()->SpawnActor<ATriggerableProjectile>(TesterActorClass);
		TransferTesterToTarget();
		if (IGameProjectileInterface* Projectile = Cast<IGameProjectileInterface>(TesterActor))
		{
			Projectile->GetProjectileDeadDelegate().AddDynamic(this, &ABuff_LuckyTester::OnTesterActorDead);
			Projectile->SetOwnerController(SourcePlayerController);
			Projectile->SetDamage(TesterDamage);
			Projectile->SetDamageRadius(TesterDamageRadius);
			if (IGameCardUserPlayerControllerInterface* PlayerController = Cast<IGameCardUserPlayerControllerInterface>(Projectile))
			{
				Projectile->SetOwnerCharacter(PlayerController->GetActualPawn());
			}
		}
	}
}

void ABuff_LuckyTester::RemoveBuff()
{
	if (IsValid(TesterActor))
	{
		if (IGameProjectileInterface* Projectile = Cast<IGameProjectileInterface>(TesterActor))
		{
			Projectile->GetProjectileDeadDelegate().RemoveDynamic(this, &ABuff_LuckyTester::OnTesterActorDead);
		}
		TesterActor->Dead();
	}

	Super::RemoveBuff();
}

void ABuff_LuckyTester::OnTargetPlayerRoundBegin()
{
	DoTest();
}

void ABuff_LuckyTester::DoTest()
{
	if (!IsValid(TesterActor))
	{
		return;
	}

	TransferTesterToTarget();

	const float TestResult = FMath::RandRange(0.f, 1.f);
	const bool bTestFailed = TestResult <= FailureRatio;
	if (bTestFailed)
	{
		TesterActor->SetTriggered(true);
	}
	else
	{
		GetWorldTimerManager().SetTimer(BuffProcessTimerHandle, this, &ABuff_LuckyTester::OnBuffProcessEnd, BuffDurationOnRoundBegin);
	}

	if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		PlayerCharacter->MulticastAddFloatingText(bTestFailed ? TestFailedText.GetLocalizeText() : TestPassedText.GetLocalizeText());
	}
}

void ABuff_LuckyTester::TransferTesterToTarget()
{
	const FVector NewLocation = TargetActor->GetActorLocation() + TransferOffset;
	const FVector RandomOffset = FVector(FMath::RandPointInCircle(RandomOffsetRadius), 0.f);
	TesterActor->SetActorLocation(NewLocation + RandomOffset, false, nullptr, ETeleportType::ResetPhysics);
	TesterActor->ReactivateProjectile();
}

void ABuff_LuckyTester::OnTesterActorDead(AActor* Tester)
{
	OnBuffProcessEndedEvent.Broadcast(this);

	EndBuff();
}
