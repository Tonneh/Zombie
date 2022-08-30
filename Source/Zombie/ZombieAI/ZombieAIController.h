// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

class AZombieCharacterBot;
/**
 * 
 */
UCLASS()
class ZOMBIE_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override; 
protected:
	virtual void BeginPlay() override; 
private:
	UPROPERTY(EditAnywhere) 
	UBehaviorTree* AIBehavior;

	UPROPERTY()
	AZombieCharacterBot* ZombieCharacter;

public:
};
