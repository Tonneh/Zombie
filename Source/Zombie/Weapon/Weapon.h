// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USoundCue;
class AShooterCharacter;
class USphereComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),

	EWT_Max UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class ZOMBIE_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	// Weapon Combat
	UPROPERTY(EditAnywhere)
	USoundBase* EquipSound;

	virtual void Fire();

	void PlayReloadLeaving();

	void PlayReloadInsert();

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = true;

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AShooterCharacter* Character;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Combat)
	float Damage = 20.f;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	// Combat Stuff 

	UPROPERTY(EditAnywhere, Category = Combat)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, Category = Combat)
	int32 Ammo;

	void SpendRound();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	bool CanZoom = true;

	// FX

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* NoAmmoSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* ReloadAnimationLeaving;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* ReloadAnimationInsert;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UTexture2D* WeaponPic;


	// Weapon States
	void ChangeWeaponState();

	void OnEquipped();

	void OnDropped(); 
public:
	virtual void Tick(float DeltaTime) override;
	void Reload(int32 AmountToReload);
	void SetWeaponState(EWeaponState State);
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }
	FORCEINLINE UTexture2D* GetWeaponPic() const { return WeaponPic; }
	FORCEINLINE bool CanAim() const { return CanZoom; }
};
