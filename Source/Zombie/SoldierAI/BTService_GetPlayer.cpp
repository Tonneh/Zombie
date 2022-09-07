// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_GetPlayer::UBTService_GetPlayer()
{
	NodeName = TEXT("GetPlayer");
}

void UBTService_GetPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	Pawn = Pawn == nullptr ? UGameplayStatics::GetPlayerPawn(GetWorld(), 0) : Pawn;
	if (Pawn == nullptr) return;

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Pawn); 

}
