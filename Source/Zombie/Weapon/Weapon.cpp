// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Zombie/Character/ZombieCharacterBot.h"
#include "Zombie/Zombie.h"
#include "Zombie/Components/CombatComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if (ShooterCharacter)
	{
		ShooterCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if (ShooterCharacter)
	{
		ShooterCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::SpendRound()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MaxAmmo);
	Character = Character == nullptr ? Cast<AShooterCharacter>(GetOwner()) : Character;
	if (Character)
	{
		Character->SetHUDAmmo();
	}
	UE_LOG(LogTemp, Warning, TEXT("%d"), Ammo);
}

void AWeapon::Fire()
{
	Character = Character == nullptr ? Cast<AShooterCharacter>(GetOwner()) : Character;
	if (Ammo == 0 && NoAmmoSound && Character->GetCombat()->GetHoldingAmmo() == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, NoAmmoSound, GetActorLocation());
		return;
	}
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	if (WeaponType == EWeaponType::EWT_AssaultRifle)
	{
		SpendRound();
	}
}

void AWeapon::PlayReloadLeaving()
{
	if (ReloadAnimationLeaving && WeaponMesh)
	{
		WeaponMesh->PlayAnimation(ReloadAnimationLeaving, false);
	}
}

void AWeapon::PlayReloadInsert()
{
	if (ReloadAnimationInsert && WeaponMesh)
	{
		WeaponMesh->PlayAnimation(ReloadAnimationInsert, false);
	}
}

void AWeapon::ChangeWeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		OnEquipped();
		break;
	default:
		break;
	}
}

void AWeapon::OnEquipped()
{
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::Reload(int32 AmountToReload)
{
	Ammo += AmountToReload;
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	ChangeWeaponState();
}
