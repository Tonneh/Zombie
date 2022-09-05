// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistanceToPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ZombieCharacterBot.h"

UBTService_DistanceToPlayer::UBTService_DistanceToPlayer()
{
	NodeName = TEXT("DistanceToPlayer");
}

void UBTService_DistanceToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Pawn = Pawn == nullptr ? UGameplayStatics::GetPlayerPawn(GetWorld(), 0) : Pawn;
	if (Pawn == nullptr) return;
	bool CanAttack = false;

	OwnerCharacter = OwnerCharacter == nullptr
		                 ? Cast<AZombieCharacterBot>(OwnerComp.GetAIOwner()->GetPawn())
		                 : OwnerCharacter;
	if (OwnerCharacter)
	{
		if (OwnerCharacter->GetDistanceTo(Pawn) <= 100)
		{
			CanAttack = true;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), CanAttack);
}
