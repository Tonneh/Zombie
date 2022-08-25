// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

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

protected:
	virtual void BeginPlay() override;

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

	float MaxHealth = 100.f;
	float Health = MaxHealth;

	/*
	 * Combat 
	 */
	UPROPERTY()
	UCombatComponent* Combat;

	bool bAiming;
	/*
	 * HUD and Crosshairs 
	 */

	UPROPERTY()
	AShooterPlayerController* ShooterController;
	UPROPERTY()
	AShooterHUD* HUD;

	void SetCrossHairs();
	void SetHUDHealth();

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
	FORCEINLINE UCameraComponent* GetCamera() { return FirstPersonCameraComponent; }
	bool IsWeaponEquipped() const;
};
