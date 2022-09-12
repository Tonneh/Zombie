// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierCharacter.h"

#include "SoldierWeapon.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Zombie/Zombie.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "Zombie/Character/ZombieCharacterBot.h"
#include "Zombie/Components/CombatComponent.h"
#include "Zombie/GameMode/ShooterGameMode.h"
#include "TimerManager.h"

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

void ASoldierCharacter::IsFiringTimerStarted()
{
	GetWorldTimerManager().SetTimer(Timer, this, &ASoldierCharacter::IsFiringTimerFinished,
											   Delay);
}

void ASoldierCharacter::IsFiringTimerFinished()
{
	IsFiring = false; 
}

void ASoldierCharacter::Fire()
{
	if (EquippedWeapon && bCanFire)
	{
		EquippedWeapon->Fire();
		StartFireTimer();
		IsFiringTimerStarted();
		bCanFire = false;
		IsFiring = true;
	}
}

void ASoldierCharacter::StartFireTimer()
{
	GetWorldTimerManager().SetTimer(FireTimer, this, &ASoldierCharacter::FireTimerFinished,
											   EquippedWeapon->FireDelay);
}

void ASoldierCharacter::FireTimerFinished()
{
	bCanFire = true; 
}

void ASoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
