// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class AShooterCharacter;
protected:
	virtual void BeginPlay() override;
private:

	UPROPERTY()
	AShooterCharacter* Character;
	
	void PlayEquippedWeaponSound(AWeapon* Weapon);
	
	/*
	 *Combat
	 */
	void Fire();
	void FireButtonPressed(bool Pressed);
	bool bCanFire = true;
	bool bFireButtonPressed;
	
	FTimerHandle FireTimer;

	void StartFireTimer();
	void FireTimerFinished();

	/*
	 * Equipping Weapon
	 */
	void AttachActorToRightHand(AWeapon* Weapon);
	void AttachActorToAimingSocket(AWeapon* Weapon);
	
	UPROPERTY()
	AWeapon* EquippedWeapon;
	
	void EquipWeapon(AWeapon* Weapon);

	/*
	 * Aiming and FOV 
	 */
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
