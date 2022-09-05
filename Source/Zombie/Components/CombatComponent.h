// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AShooterPlayerController;
class AKnife;
UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Sprinting UMETA(DisplayName = "Sprinting"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_KnifeAttack UMETA(DisplayName = "KnifeAttack"),
	ECS_SprintKnifeAttack UMETA(DisplayName = "Sprint KnifeAttack"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

class AWeapon;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
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
	UPROPERTY()
	AShooterPlayerController* Controller; 
	void PlayEquippedWeaponSound(AWeapon* Weapon);

	/*
	 *Knife
	 */

	UPROPERTY()
	AKnife* Knife;
	
	void EquipKnife(AKnife* knife);

	void KnifeAttack();

	UFUNCTION(BlueprintCallable)
	void KnifeSwingBP();
	/*
	 *Combat
	 */

	ECombatState CombatState;
	
	void Fire();
	void FireButtonPressed(bool Pressed);
	bool bCanFire = true;
	bool bFireButtonPressed;
	int32 AmountToReload(); 

	void Reload(); 
	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	UFUNCTION(BlueprintCallable)
	void PlayWeaponLeaving();
	UFUNCTION(BlueprintCallable)
	void PlayWeaponInsert();
	UFUNCTION(BlueprintCallable)
	void KnifeAttackFinished();
	
	FTimerHandle FireTimer;

	void StartFireTimer();
	void FireTimerFinished();

	int32 HoldingAmmo = 100;

	/*
	 * Equipping Weapon
	 */
	void AttachActorToRightHand(AWeapon* Weapon);
	void AttachActorToPistolSocket(AWeapon* Weapon);
	void AttachKnifeToRightHand(AKnife* knife); 
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
	bool CanReload() const; 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE int32 GetHoldingAmmo() const { return HoldingAmmo; }
};

