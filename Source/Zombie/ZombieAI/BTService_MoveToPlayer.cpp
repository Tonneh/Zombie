// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MoveToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTService_MoveToPlayer::UBTService_MoveToPlayer()
{
	NodeName = TEXT("Move To Player Location"); 

}

void UBTService_MoveToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	MoveToPlayer(OwnerComp); 
}

void UBTService_MoveToPlayer::MoveToPlayer(UBehaviorTreeComponent& OwnerComp)
{
	// Get playerpawn
	Pawn = Pawn == nullptr ? UGameplayStatics::GetPlayerPawn(GetWorld(), 0) : Pawn;
	if (Pawn == nullptr) return;

	const float Distance = OwnerComp.GetAIOwner()->GetOwner()->GetDistanceTo(Pawn);
}
