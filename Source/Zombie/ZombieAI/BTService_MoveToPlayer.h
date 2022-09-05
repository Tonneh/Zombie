// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_MoveToPlayer.generated.h"

UCLASS()
class ZOMBIE_API UBTService_MoveToPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_MoveToPlayer(); 
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY()
	APawn* Pawn;
	
	void MoveToPlayer(UBehaviorTreeComponent& OwnerComp); 
};
