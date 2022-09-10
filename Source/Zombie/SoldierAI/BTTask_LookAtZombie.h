// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LookAtZombie.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API UBTTask_LookAtZombie : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_LookAtZombie();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
};
