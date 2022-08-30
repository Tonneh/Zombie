// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "Zombie/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Zombie/Weapon/Knife.h"

UCombatComponent::UCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	CombatState = ECombatState::ECS_Unoccupied;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		if (Character->GetCamera())
		{
			DefaultFOV = Character->GetCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
}

void UCombatComponent::PlayEquippedWeaponSound(AWeapon* Weapon)
{
	if (Weapon && Weapon->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Weapon->EquipSound, Character->GetActorLocation());
	}
}

void UCombatComponent::EquipKnife(AKnife* knife)
{
	Knife = knife; 
	if (Knife && Character)
	{
		AttachKnifeToRightHand(Knife);
		Knife->SetOwner(Character);
	}
}

void UCombatComponent::KnifeAttack()
{
	if (Knife && Character && CombatState == ECombatState::ECS_Unoccupied)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->SetActorHiddenInGame(true);
		}
		Character->PlayKnifeAttackAnimation();
		CombatState = ECombatState::ECS_KnifeAttack;
		Knife->SetActorHiddenInGame(false);
		Knife->KnifeSwing();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Knife->GetSwingSound(), Character->GetActorLocation());
	}
}

void UCombatComponent::KnifeSwingBP()
{
	if (Knife)
	{
		Knife->KnifeSwing();
	}
}

void UCombatComponent::Fire()
{
	if (EquippedWeapon)
	{
		bCanFire = false; 
		EquippedWeapon->Fire();
		StartFireTimer();
		if (EquippedWeapon->GetAmmo() == 0)
		{
			Reload(); 
		}
	}
}

void UCombatComponent::FireButtonPressed(bool Pressed)
{
	bFireButtonPressed = Pressed;
	if (bFireButtonPressed && CombatState == ECombatState::ECS_Unoccupied && bCanFire)
	{
		Fire();
	}
}

void UCombatComponent::EquipWeapon(AWeapon* Weapon)
{
	if (Weapon == nullptr || Character == nullptr || Knife == nullptr) return;
	EquippedWeapon = Weapon;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	Knife->SetActorHiddenInGame(true);
	AttachActorToRightHand(Weapon);
	Weapon->SetOwner(Character);
	PlayEquippedWeaponSound(Weapon);
}

int32 UCombatComponent::AmountToReload()
{
	if (EquippedWeapon == nullptr) return 0;
	const int32 RoomInMag = EquippedWeapon->GetMaxAmmo() - EquippedWeapon->GetAmmo();
	const int32 Least = FMath::Min(RoomInMag, HoldingAmmo);
	return FMath::Clamp(RoomInMag, 0, Least);
}

void UCombatComponent::Reload()
{
	if (CombatState == ECombatState::ECS_Unoccupied)
	{
		if (Character)
		{
			Character->PlayReloadAnimation();
		}
		CombatState = ECombatState::ECS_Reloading; 
		const int32 ReloadAmount = AmountToReload(); 
		EquippedWeapon->Reload(ReloadAmount);
		HoldingAmmo -= ReloadAmount;
		bCanFire = false; 
	}
}

void UCombatComponent::FinishReloading()
{
	bCanFire = true; 
	CombatState = ECombatState::ECS_Unoccupied;
	Character->SetHUDAmmo();
}

void UCombatComponent::PlayWeaponLeaving()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->PlayReloadLeaving();
	}
}

void UCombatComponent::PlayWeaponInsert()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->PlayReloadInsert();
	}
}

void UCombatComponent::KnifeAttackFinished()
{
	if (Knife == nullptr) return; 
	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorHiddenInGame(false);
		Knife->SetActorHiddenInGame(true);
	}
	CombatState = ECombatState::ECS_Unoccupied;
}

void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UCombatComponent::FireTimerFinished, EquippedWeapon->FireDelay);
}

void UCombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return; 
	bCanFire = true;
	if (bFireButtonPressed && EquippedWeapon->bAutomatic && CombatState == ECombatState::ECS_Unoccupied)
	{
		Fire();
	}
}

void UCombatComponent::AttachActorToRightHand(AWeapon* Weapon)
{
	if (Character == nullptr|| Character->Mesh1P == nullptr || Weapon == nullptr)
		return;
	const USkeletalMeshSocket* HandSocket = Character->Mesh1P->GetSocketByName(FName("WeaponSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(Weapon, Character->Mesh1P);
	}
}

void UCombatComponent::AttachKnifeToRightHand(AKnife* knife)
{
	if (Character == nullptr|| Character->Mesh1P == nullptr || knife == nullptr)
		return;
	const USkeletalMeshSocket* HandSocket = Character->Mesh1P->GetSocketByName(FName("WeaponSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(knife, Character->Mesh1P);
	}
}

void UCombatComponent::AttachActorToAimingSocket(AWeapon* Weapon)
{
	if (Character == nullptr|| Character->Mesh1P == nullptr || Weapon == nullptr)
		return;
	const USkeletalMeshSocket* AimingSocket = Character->Mesh1P->GetSocketByName(FName("AimSocket"));
	if (AimingSocket)
	{
		AimingSocket->AttachActor(Weapon, Character->Mesh1P);
	}
}


void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;

	if (Character)
	{
		if (Character->IsAiming())
		{
			CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomInterpSpeed());
		}
		else
		{
			CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
		}
		if (Character->GetCamera())
		{
			Character->GetCamera()->SetFieldOfView(CurrentFOV);
		}
	}
}

bool UCombatComponent::CanReload() const
{
	if (EquippedWeapon == nullptr) return false; 
	return EquippedWeapon->GetAmmo() != EquippedWeapon->GetMaxAmmo() && CombatState != ECombatState::ECS_Reloading;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InterpFOV(DeltaTime);

}

