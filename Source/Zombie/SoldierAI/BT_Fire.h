// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Fire.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API UBT_Fire : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBT_Fire();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
