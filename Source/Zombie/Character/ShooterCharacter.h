// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class ABuyArea;
class AAmmobox;
enum class EWeaponType : uint8;
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
	void SetHUDWeapon();
	void PlayReloadAnimation(EWeaponType WeaponType);
	void PlayKnifeAttackAnimation();
	void CloseShop(); 
protected:
	virtual void BeginPlay() override;
	// Override for Mesh1P
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f,
	                              FName StartSectionName = NAME_None) override;
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                   AController* InstigatorController, AActor* DamageCauser);

private:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY()
	AShooterPlayerController* ShooterPlayerController;

	bool MovingForward = false;
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
	void SprintButtonPressed();
	void SprintButtonReleased();
	void RefillButtonPressed();
	void ShopButtonPressed();
	void ShopButtonReleased();
	bool Sprinting = false;

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

	UPROPERTY(EditAnywhere)
	UAnimMontage* PistolReloadMontage;

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

	UPROPERTY()
	AAmmobox* OverlappingAmmoBox;

	UPROPERTY()
	ABuyArea* OverlappingBuyArea;

	bool ShopOpened = false; 
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; }
	FORCEINLINE void SetOverlappingAmmoBox(AAmmobox* AmmoBox) { OverlappingAmmoBox = AmmoBox; }
	FORCEINLINE void SetOverlappingBuyArea(ABuyArea* BuyArea) { OverlappingBuyArea = BuyArea; }
	FORCEINLINE bool IsAiming() const { return bAiming; }
	FORCEINLINE UCameraComponent* GetCamera() const { return FirstPersonCameraComponent; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool IsSprinting() const { return Sprinting; }
	FORCEINLINE bool IsShopping() const { return ShopOpened; }
	FORCEINLINE AShooterPlayerController* GetShooterPlayerController() const { return ShooterPlayerController; }
	bool IsWeaponEquipped() const;
	int32 GetWeaponType();
};
