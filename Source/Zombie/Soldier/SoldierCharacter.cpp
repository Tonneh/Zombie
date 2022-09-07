// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierCharacter.h"

#include "SoldierWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Zombie/Components/CombatComponent.h"

ASoldierCharacter::ASoldierCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	
}

void ASoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon();
}

void ASoldierCharacter::EquipWeapon()
{
	if (DefaultWeaponClass == nullptr) return; 
	AWeapon* StartingWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
	EquippedWeapon = StartingWeapon;
	EquippedWeapon->SetOwner(this);
	AttachWeaponToHand(); 
}

void ASoldierCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		if (GetMesh())
		{
			const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName("WeaponIdleSocket");
			if (HandSocket)
			{
				HandSocket->AttachActor(EquippedWeapon, GetMesh());
			}
		}
	}
}

void ASoldierCharacter::Fire()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Fire();
		IsFiring = true; 
	}
}

void ASoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
