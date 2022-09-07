// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Fire.h"

#include "AIController.h"
#include "Zombie/Soldier/SoldierCharacter.h"

UBT_Fire::UBT_Fire()
{
    NodeName = TEXT("Fire");
}

EBTNodeResult::Type UBT_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory); 

    ASoldierCharacter* Character = Cast<ASoldierCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Character == nullptr) return EBTNodeResult::Failed;

    Character->Fire();

    return EBTNodeResult::Succeeded; 
}
