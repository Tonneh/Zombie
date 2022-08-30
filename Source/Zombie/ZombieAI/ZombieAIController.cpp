// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"

#include "BrainComponent.h"
#include "Zombie/Character/ZombieCharacterBot.h"

void AZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ZombieCharacter = ZombieCharacter == nullptr ? Cast<AZombieCharacterBot>(GetPawn()) : ZombieCharacter; 
	if (ZombieCharacter && ZombieCharacter->IsDead)
	{
		BrainComponent->StopLogic(""); 
	}
}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior); 
	}
}
