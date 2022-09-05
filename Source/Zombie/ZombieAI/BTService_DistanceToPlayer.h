// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_DistanceToPlayer.generated.h"

class AZombieCharacterBot;
/**
 * 
 */
UCLASS()
class ZOMBIE_API UBTService_DistanceToPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_DistanceToPlayer();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY()
	APawn* Pawn;

	UPROPERTY()
	AZombieCharacterBot* OwnerCharacter; 
};
