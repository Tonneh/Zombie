// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class AShooterHUD;

UCLASS()
class ZOMBIE_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDAmmo(int32 WeaponAmmo, int32 HoldingAmmo);
	void SetHUDWeapon(UTexture2D* WeaponPic);
	void ShowHitMarkerHead();
	void ShowHitMarkerBody();
	void ShowShop();
	void RemoveShop(); 
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	AShooterHUD* ShooterHUD;
	
};
