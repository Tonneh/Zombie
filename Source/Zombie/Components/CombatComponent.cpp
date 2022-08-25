// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "Zombie/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"

UCombatComponent::UCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
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

void UCombatComponent::Fire()
{
	if (EquippedWeapon && bCanFire)
	{
		bCanFire = false; 
		EquippedWeapon->Fire();
		StartFireTimer();
	}
}

void UCombatComponent::FireButtonPressed(bool Pressed)
{
	bFireButtonPressed = Pressed;
	if (bFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::EquipWeapon(AWeapon* Weapon)
{
	if (Weapon == nullptr || Character == nullptr) return;
	EquippedWeapon = Weapon;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	AttachActorToRightHand(Weapon);
	Weapon->SetOwner(Character);
	PlayEquippedWeaponSound(Weapon);
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
	if (bFireButtonPressed && EquippedWeapon->bAutomatic)
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

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InterpFOV(DeltaTime);

}

