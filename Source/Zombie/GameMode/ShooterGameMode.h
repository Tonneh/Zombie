// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShooterGameMode.generated.h"

class AZombieCharacterBot;
/**
 * 
 */
UCLASS()
class ZOMBIE_API AShooterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<AZombieCharacterBot*> ZombieArray; 
protected:
private:
public:
};
