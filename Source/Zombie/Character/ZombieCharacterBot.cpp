// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieCharacterBot.h"

#include "ShooterCharacter.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Zombie/Zombie.h"
#include "GameFramework/MovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "Zombie/GameMode/ShooterGameMode.h"

AZombieCharacterBot::AZombieCharacterBot()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_Zombie);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void AZombieCharacterBot::Attack()
{
	if (AttackMontage == nullptr) return;
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage) || IsDead)
	{
		return; 
	}
	PlayAnimMontage(AttackMontage);
}

void AZombieCharacterBot::DoDamage()
{
	TArray<AActor*> HitActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	const FVector HitLocation = GetArrowComponent()->GetComponentLocation();
	UKismetSystemLibrary::SphereOverlapActors(this, HitLocation, 100,
	                                          ObjectTypes, AShooterCharacter::StaticClass(), IgnoreActors,
	                                          HitActors);
	for (const auto HitActor : HitActors)
	{
		UGameplayStatics::ApplyDamage(HitActor, HitDamage, GetInstigatorController(), this,
		                              UDamageType::StaticClass());
	}
}

void AZombieCharacterBot::FinishAttack()
{
	UAIBlueprintHelperLibrary::GetBlackboard(this)->SetValueAsBool(FName("IsAttacking"), false);
}

void AZombieCharacterBot::DeathAnimFinished()
{
	Destroy();
}


void AZombieCharacterBot::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnTakeAnyDamage.AddDynamic(this, &AZombieCharacterBot::ReceiveDamage);
}

void AZombieCharacterBot::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                        AController* InstigatorController, AActor* DamageCauser)
{
	if (IsDead)
	{
		return;
	}

	if (Damage <= 0.f)
	{
		return;
	}

	PlayAnimMontage(HitReact);
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

	ShooterGameMode = ShooterGameMode == nullptr
					  ? Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode())
					  : ShooterGameMode;
	if (ShooterGameMode == nullptr) return; 
	if (Health <= 0.f && DeathAnimation)
	{
		ShooterGameMode->ZombieArray.Remove(this);
		IsDead = true;
		GetMesh()->GetAnimInstance()->Montage_Stop(1.f);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayAnimMontage(DeathAnimation);
	}
}

void AZombieCharacterBot::StartDeathTimer()
{
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AZombieCharacterBot::DeathTimerFinished, DeathDelay);
}

void AZombieCharacterBot::DeathTimerFinished()
{
	Destroy();
}

void AZombieCharacterBot::CheckNearby()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Distance = GetDistanceTo(PlayerPawn);
	}
}

void AZombieCharacterBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckNearby();
}
