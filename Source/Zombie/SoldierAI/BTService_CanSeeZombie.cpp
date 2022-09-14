// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CanSeeZombie.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ZombieCharacterBot.h"
#include "TimerManager.h"
#include "Zombie/GameMode/ShooterGameMode.h"

UBTService_CanSeeZombie::UBTService_CanSeeZombie()
{
	NodeName = TEXT("CanSeeZombie");
}

void UBTService_CanSeeZombie::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	ShooterGameMode = ShooterGameMode == nullptr
		                  ? Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode())
		                  : ShooterGameMode;
	if (ShooterGameMode == nullptr) return;
	if (ShooterGameMode->ZombieArray.Num() <= 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Zombie"), nullptr);
		return;
	}
	// IsValid checks if the zombie is nullptr or if its pending to be destroyed
	float Distance = 100000.f;
	AZombieCharacterBot* ClosestZombie = ShooterGameMode->ZombieArray[0];
	// Get the closest Zombie 
	for (const auto Zombie : ShooterGameMode->ZombieArray)
	{
		if (OwnerComp.GetOwner()->GetDistanceTo(Zombie) < Distance && OwnerComp.GetAIOwner()->LineOfSightTo(Zombie))
		{
			ClosestZombie = Zombie; 
		}
	}
	
	// We have sight of first zombie and its not dead, so fire at it. 
	if (IsValid(ClosestZombie) && !ClosestZombie->IsDead && OwnerComp.GetAIOwner()->LineOfSightTo(ClosestZombie))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Zombie"), ClosestZombie);
	}
	// if we no longer see the closest zombie or its dead, or its not longer valid 
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Zombie"), nullptr);
	}
}
