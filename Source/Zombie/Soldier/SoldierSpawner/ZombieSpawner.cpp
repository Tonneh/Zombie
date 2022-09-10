// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSpawner.h"

#include "Zombie/Character/ZombieCharacterBot.h"
#include "Zombie/GameMode/ShooterGameMode.h"

AZombieSpawner::AZombieSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

	ShooterGameMode = ShooterGameMode == nullptr
		                  ? Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode())
		                  : ShooterGameMode;
	SpawnActor(SpawnAmount);
}

void AZombieSpawner::SpawnActor(int32 AmountToSpawn)
{
	if (ShooterGameMode == nullptr) return;
	for (int i = 0; i < AmountToSpawn; i++)
	{
		if (ZombieClass)
			ShooterGameMode->ZombieArray.AddUnique(
				GetWorld()->SpawnActor<AZombieCharacterBot>(ZombieClass, GetActorTransform()));
	}
}

void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
