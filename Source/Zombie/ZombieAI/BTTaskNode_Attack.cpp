// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Zombie/Character/ZombieCharacterBot.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)  return EBTNodeResult::Failed;
	AZombieCharacterBot* Character = Cast<AZombieCharacterBot>(OwnerComp.GetAIOwner()->GetPawn());
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr) return EBTNodeResult::Failed;
	if (Character)
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), PlayerPawn->GetActorLocation());
		Character->SetActorRotation(Rotation);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsAttacking"), true);
		Character->Attack();
	}
	return EBTNodeResult::Succeeded;
}
