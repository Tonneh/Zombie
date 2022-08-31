// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Zombie/HUD/CharacterOverlay.h"
#include "Zombie/HUD/ShooterHUD.h"

void AShooterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
	if (ShooterHUD && ShooterHUD->CharacterOverlay && ShooterHUD->CharacterOverlay->HealthText && ShooterHUD->
		CharacterOverlay->HealthBar)
	{
		const float HealthPercent = Health / MaxHealth;
		const FString HealthText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Health));
		ShooterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		ShooterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void AShooterPlayerController::SetHUDAmmo(int32 WeaponAmmo, int32 HoldingAmmo)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
	if (ShooterHUD && ShooterHUD->CharacterOverlay && ShooterHUD->CharacterOverlay->Ammo && ShooterHUD->CharacterOverlay
		->HoldingAmmo && ShooterHUD->CharacterOverlay->Slash)
	{
		ShooterHUD->CharacterOverlay->Ammo->SetText(FText::AsNumber(WeaponAmmo));
		ShooterHUD->CharacterOverlay->HoldingAmmo->SetText(FText::AsNumber(HoldingAmmo));
		ShooterHUD->CharacterOverlay->Slash->SetText(FText::FromString("/"));
	}
}

void AShooterPlayerController::SetHUDWeapon(UTexture2D* WeaponPic)
{
	ShooterHUD = ShooterHUD == nullptr ? Cast<AShooterHUD>(GetHUD()) : ShooterHUD;
	if (ShooterHUD && ShooterHUD->CharacterOverlay && ShooterHUD->CharacterOverlay->Weapon && WeaponPic)
	{
		ShooterHUD->CharacterOverlay->Weapon->SetBrushTintColor(FSlateColor(FLinearColor(1, 1, 1, 1)));
		ShooterHUD->CharacterOverlay->Weapon->SetBrushFromTexture(WeaponPic);
	}
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
