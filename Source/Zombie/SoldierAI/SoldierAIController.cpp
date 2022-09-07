// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAIController.h"

void ASoldierAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoldierAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior); 
	}
}
