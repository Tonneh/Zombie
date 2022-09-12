// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "Zombie/Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Zombie/PlayerController/ShooterPlayerController.h"
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
	if (Knife && Character && (CombatState == ECombatState::ECS_Unoccupied || CombatState ==
		ECombatState::ECS_Sprinting))
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->SetActorHiddenInGame(true);
		}
		if (CombatState == ECombatState::ECS_Sprinting)
		{
			CombatState = ECombatState::ECS_SprintKnifeAttack;
		}
		else
		{
			CombatState = ECombatState::ECS_KnifeAttack;
		}
		Character->PlayKnifeAttackAnimation();
		Knife->SetActorHiddenInGame(false);
		Knife->KnifeSwing();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Knife->GetSwingSound(), Character->GetActorLocation());
	}
}

void UCombatComponent::KnifeAttackFinished()
{
	if (Knife == nullptr || Character == nullptr) return;
	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorHiddenInGame(false);
		Knife->SetActorHiddenInGame(true);
		Character->SetHUDWeapon();
	}
	if (CombatState == ECombatState::ECS_SprintKnifeAttack)
	{
		CombatState = ECombatState::ECS_Sprinting;
	}
	else
	{
		CombatState = ECombatState::ECS_Unoccupied;
	}
	Knife->AlreadyHitActors.Empty();
	Knife->ResetHits();
}

void UCombatComponent::FireAnimFinished()
{
	bIsFiring = false; 
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
	if (EquippedWeapon == nullptr) return; 
	if (EquippedWeapon->GetAmmo() == 0 && EquippedWeapon->GetNoAmmoSound() && EquippedWeapon->
		GetHoldingAmmo() == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->GetNoAmmoSound(),
		                                      EquippedWeapon->GetActorLocation());
		return;
	}
	bCanFire = false;
	bIsFiring = true;
	EquippedWeapon->Fire();
	Recoil();
	StartFireTimer();
	if (EquippedWeapon->GetAmmo() == 0 && EquippedWeapon->GetHoldingAmmo() > 0)
	{
		Reload();
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

void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UCombatComponent::FireTimerFinished,
											   EquippedWeapon->FireDelay);
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

void UCombatComponent::EquipWeapon(AWeapon* Weapon)
{
	if (Weapon == nullptr || Character == nullptr || Knife == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (EquippedWeapon != nullptr)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	}
	EquippedWeapon = Weapon;
	Weapon->SetWeaponState(EWeaponState::EWS_Equipped);
	Knife->SetActorHiddenInGame(true);
	if (Weapon->GetWeaponType() == EWeaponType::EWT_Pistol)
	{
		AttachActorToPistolSocket(Weapon);
	}
	else
	{
		AttachActorToRightHand(Weapon);
	}
	Weapon->SetOwner(Character);
	PlayEquippedWeaponSound(Weapon);
}

int32 UCombatComponent::AmountToReload()
{
	if (EquippedWeapon == nullptr) return 0;
	const int32 RoomInMag = EquippedWeapon->GetMaxAmmo() - EquippedWeapon->GetAmmo();
	const int32 Least = FMath::Min(RoomInMag, EquippedWeapon->GetHoldingAmmo());
	return FMath::Clamp(RoomInMag, 0, Least);
}

void UCombatComponent::Reload()
{
	if (Character && CanReload() && EquippedWeapon)
	{
		CombatState = ECombatState::ECS_Reloading;
		const int32 ReloadAmount = AmountToReload();
		EquippedWeapon->Reload(ReloadAmount);
		const int32 NewAmmoAmount = EquippedWeapon->GetHoldingAmmo() - ReloadAmount;
		EquippedWeapon->SetHoldingAmmo(NewAmmoAmount);
		bCanFire = false;
		Character->PlayReloadAnimation(EquippedWeapon->GetWeaponType());
	}
}

void UCombatComponent::FinishReloading()
{
	if (Character && Character->IsSprinting())
	{
		bCanFire = true;
		CombatState = ECombatState::ECS_Sprinting;
		Character->SetHUDAmmo();
	}
	else
	{
		bCanFire = true;
		CombatState = ECombatState::ECS_Unoccupied;
		Character->SetHUDAmmo();
	}
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

void UCombatComponent::AttachActorToRightHand(AWeapon* Weapon)
{
	if (Character == nullptr || Character->Mesh1P == nullptr || Weapon == nullptr)
		return;
	const USkeletalMeshSocket* HandSocket = Character->Mesh1P->GetSocketByName(FName("WeaponSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(Weapon, Character->Mesh1P);
	}
}

void UCombatComponent::AttachActorToPistolSocket(AWeapon* Weapon)
{
	if (Character == nullptr || Character->Mesh1P == nullptr || Weapon == nullptr)
		return;
	const USkeletalMeshSocket* PistolSocket = Character->Mesh1P->GetSocketByName(FName("PistolSocket"));
	if (PistolSocket)
	{
		PistolSocket->AttachActor(Weapon, Character->Mesh1P);
	}
}

void UCombatComponent::AttachKnifeToRightHand(AKnife* knife)
{
	if (Character == nullptr || Character->Mesh1P == nullptr || knife == nullptr)
		return;
	const USkeletalMeshSocket* HandSocket = Character->Mesh1P->GetSocketByName(FName("WeaponSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(knife, Character->Mesh1P);
	}
}

void UCombatComponent::AttachActorToAimingSocket(AWeapon* Weapon)
{
	if (Character == nullptr || Character->Mesh1P == nullptr || Weapon == nullptr)
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
			CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime,
			                              EquippedWeapon->GetZoomInterpSpeed());
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

void UCombatComponent::Recoil()
{
	if (Character == nullptr || Character->GetController() == nullptr) return;
	if (EquippedWeapon == nullptr || EquippedWeapon->GetVerticalRecoilCurve() == nullptr || EquippedWeapon->
		GetHorizontalRecoilCurve() == nullptr)
		return;
	RecoilTimerPerShot += EquippedWeapon->GetFireDelay();

	float VerticalRecoilAmount = EquippedWeapon->GetVerticalRecoilCurve()->GetFloatValue(RecoilTimerPerShot);
	VerticalRecoilAmount *= 0.4;
	float HorizontalRecoilAmount = EquippedWeapon->GetHorizontalRecoilCurve()->GetFloatValue(RecoilTimerPerShot);
	HorizontalRecoilAmount *= 0.4;
	const FRotator Rotation = Character->Controller->GetControlRotation().Add(
		VerticalRecoilAmount, HorizontalRecoilAmount, 0);
	Character->Controller->SetControlRotation(Rotation);
}

void UCombatComponent::ResetRecoil()
{
	RecoilTimerPerShot = 0;
}

bool UCombatComponent::CanReload() const
{
	if (EquippedWeapon == nullptr) return false;
	return EquippedWeapon->GetAmmo() != EquippedWeapon->GetMaxAmmo() && CombatState == ECombatState::ECS_Unoccupied;
}

bool UCombatComponent::IsFull() const
{
	if (EquippedWeapon == nullptr) return false;
	return EquippedWeapon->IsFull(); 
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*
	if (CombatState == ECombatState::ECS_Unoccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unoccupied"));
	}
	if (CombatState == ECombatState::ECS_Reloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading"));
	}
	if (CombatState == ECombatState::ECS_Sprinting)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprinting"));
	}
	if (CombatState == ECombatState::ECS_KnifeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("KnifeAttack"));
	}
	if (CombatState == ECombatState::ECS_SprintKnifeAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("SprintKnifeAttack"));
	}
	*/
	InterpFOV(DeltaTime);
}
