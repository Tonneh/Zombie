// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public: 
	virtual void NativeInitializeAnimation() override; 
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter; 

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped; 

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsFiringPistol;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	int32 WeaponType;
};
