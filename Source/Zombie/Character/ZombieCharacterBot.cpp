// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieCharacterBot.h"

#include "Zombie/Zombie.h"

AZombieCharacterBot::AZombieCharacterBot()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_Zombie);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

void AZombieCharacterBot::BeginPlay()
{
	Super::BeginPlay();

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
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("%f"), Health); 
	if (Health <= 0.f && DeathAnimation)
	{
		IsDead = true; 
		PlayAnimMontage(DeathAnimation);
		StartDeathTimer(); 
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

void AZombieCharacterBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


