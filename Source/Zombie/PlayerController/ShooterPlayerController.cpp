// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Zombie/HUD/CharacterOverlay.h"
#include "Zombie/HUD/ShooterHUD.h"

void AShooterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
	if (ShooterHUD && ShooterHUD->CharacterOverlay && ShooterHUD->CharacterOverlay->HealthText && ShooterHUD->CharacterOverlay->HealthBar)
	{
		const float HealthPercent = Health / MaxHealth;
		const FString HealthText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Health));
		ShooterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent); 
		ShooterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}
