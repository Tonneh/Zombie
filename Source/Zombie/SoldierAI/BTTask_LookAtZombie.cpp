// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAtZombie.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_LookAtZombie::UBTTask_LookAtZombie()
{
	NodeName = TEXT("LookAtZombie");
}

EBTNodeResult::Type UBTTask_LookAtZombie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const FVector Location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("Zombie"));
	const FRotator Rotation =
		UKismetMathLibrary::FindLookAtRotation(OwnerComp.GetOwner()->GetActorLocation(), Location);
	OwnerComp.GetOwner()->SetActorRotation(Rotation);

    return EBTNodeResult::Succeeded; 
}
