// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoldierCharacter.generated.h"

class AShooterGameMode;
class USphereComponent;
class AWeapon;
class UCombatComponent;
UCLASS()
class ZOMBIE_API ASoldierCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASoldierCharacter();
	virtual void PostInitializeComponents() override;
	void Fire();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsFiring = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator AORotation;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AShooterGameMode* ShooterGameMode;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
private:
	UPROPERTY()
	AWeapon* EquippedWeapon;

	bool bCanFire = true;
	
	void EquipWeapon();
	void AttachWeaponToHand();

	float Delay = 3.f; 
	FTimerHandle Timer;
	void IsFiringTimerStarted();
	void IsFiringTimerFinished(); 
	FTimerHandle FireTimer;
	void StartFireTimer();
	void FireTimerFinished();
public:
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE void SetAORotation(FRotator Rotation) { AORotation = Rotation; }
};
