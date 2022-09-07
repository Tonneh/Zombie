// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombie/Weapon/Weapon.h"
#include "SoldierWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API ASoldierWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;
private:
	UPROPERTY()
	APawn* OwnerPawn;
	void PerformLineTrace(FHitResult& HitResult, FVector& ShotDirection);

	// FX

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UParticleSystem* BodyImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* BodyHitSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UParticleSystem* MuzzleFlash;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UParticleSystem* BeamParticles;

};
