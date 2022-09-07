// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CanSeeZombie.generated.h"

class AZombieCharacterBot;
/**
 * 
 */
UCLASS()
class ZOMBIE_API UBTService_CanSeeZombie : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CanSeeZombie();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY()
	APawn* Pawn;
	
	UPROPERTY()
	TArray<AZombieCharacterBot*> ZombieActors;

	UPROPERTY()
	AZombieCharacterBot* ZombieActor; 

};
