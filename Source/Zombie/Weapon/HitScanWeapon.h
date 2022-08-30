// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"


UCLASS()
class ZOMBIE_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire() override;

private:
	// LineTrace
	FVector HitTarget;
	void PerformLineTrace(FHitResult& HitResult);

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

public:

};
