// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CanSeeZombie.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ZombieCharacterBot.h"

UBTService_CanSeeZombie::UBTService_CanSeeZombie()
{
	NodeName = TEXT("CanSeeZombie");
}

void UBTService_CanSeeZombie::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ZombieActor = Cast<
	AZombieCharacterBot>(UGameplayStatics::GetActorOfClass(this, AZombieCharacterBot::StaticClass()));

	if (ZombieActor == nullptr) return;
	
	if (OwnerComp.GetAIOwner()->LineOfSightTo(ZombieActor) && !ZombieActor->IsDead)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Zombie"), ZombieActor);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
}
