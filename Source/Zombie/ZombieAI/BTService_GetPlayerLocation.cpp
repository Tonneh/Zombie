// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetPlayerLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_GetPlayerLocation::UBTService_GetPlayerLocation()
{
	NodeName = TEXT("UpdatePlayerLocation");
}

void UBTService_GetPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr) return;

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn); 
}

