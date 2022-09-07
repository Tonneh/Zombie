// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoldierCharacter.generated.h"

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
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
	UPROPERTY()
	AWeapon* EquippedWeapon;

	void EquipWeapon();
	void AttachWeaponToHand();

public:	
	virtual void Tick(float DeltaTime) override;
};
