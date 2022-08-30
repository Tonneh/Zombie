// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AKnife;
class UCombatComponent;
class AWeapon;
class AShooterPlayerController;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UTexture2D;
class AShooterHUD;

UCLASS()
class ZOMBIE_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter();
	virtual void PostInitializeComponents() override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	void SetCrossHairs();
	void HideCrossHairs();
	void SetHUDHealth();
	void SetHUDAmmo();
	void PlayReloadAnimation();
	void PlayKnifeAttackAnimation();
protected:
	virtual void BeginPlay() override;
	// Override for Mesh1P
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;
private:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EquipButtonPressed();
	void FireButtonPressed();
	void FireButtonReleased();
	void AimButtonPressed();
	void AimButtonReleased();
	void ReloadButtonPressed();
	
	float MaxHealth = 100.f;
	float Health = MaxHealth;

	/*
	 *	Knife
	 */

	UPROPERTY(EditAnywhere)
	TSubclassOf<AKnife> DefaultKnife;

	UPROPERTY(EditAnywhere)
	UAnimMontage* KnifeAttackMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* KnifeAttackMontage2;
	
	void SpawnDefaultWeapon();

	void KnifeButtonPressed(); 
	/*
	 * Combat 
	 */
	UPROPERTY()
	UCombatComponent* Combat;

	bool bAiming;

	UPROPERTY(EditAnywhere) 
	UAnimMontage* ReloadMontage;
	
	/*
	 * HUD and Crosshairs 
	 */

	UPROPERTY()
	AShooterPlayerController* ShooterController;
	UPROPERTY()
	AShooterHUD* HUD;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;

	// Weapon Attachments

	UPROPERTY()
	AWeapon* OverlappingWeapon;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; }
	FORCEINLINE bool IsAiming() { return bAiming; }
	FORCEINLINE UCameraComponent* GetCamera() const { return FirstPersonCameraComponent; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	bool IsWeaponEquipped() const;
};
