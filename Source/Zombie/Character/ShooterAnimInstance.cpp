// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"

#include "ShooterCharacter.h"
#include "Zombie/Components/CombatComponent.h"
#include "Zombie/Weapon/Weapon.h"

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UShooterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}
	if (ShooterCharacter == nullptr) return;
	if (Combat == nullptr)
	{
		Combat = ShooterCharacter->GetCombat();
	}
	if (Combat == nullptr) return;
	bWeaponEquipped = ShooterCharacter->IsWeaponEquipped();
	bIsAiming = ShooterCharacter->IsAiming();
	bIsSprinting = ShooterCharacter->IsSprinting();
	WeaponType = ShooterCharacter->GetWeaponType();
	if (Combat->GetEquippedWeapon() == nullptr) return;
	bIsFiringPistol = Combat->Fired() && Combat->GetEquippedWeapon()->GetAmmo() > 0;
	if (bWeaponEquipped && ShooterCharacter->Mesh1P && Combat->GetEquippedWeapon()->
	                                                           GetWeaponMesh())
	{
		LeftHandTransform = Combat->GetEquippedWeapon()->GetWeaponMesh()->GetSocketTransform(
			FName("ClipSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		ShooterCharacter->Mesh1P->TransformToBoneSpace(FName("Character001RArmPalm"), LeftHandTransform.GetLocation(),
		                                               FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
	bUseFabrik = ShooterCharacter->GetCombat()->GetCombatState() == ECombatState::ECS_Reloading;
}
