// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammobox.h"

#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "Zombie/Components/CombatComponent.h"

AAmmobox::AAmmobox()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoBoxMesh"));
	SetRootComponent(Mesh);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RefillAmmoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("RefillAmmoWidget"));
	RefillAmmoWidget->SetupAttachment(RootComponent);

	WeaponAmmoFullWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FullAmmoWidget"));
	WeaponAmmoFullWidget->SetupAttachment(RootComponent);
}

void AAmmobox::FillAmmo(AWeapon* EquippedWeapon)
{
	if (!EquippedWeapon) return; 
	if (!EquippedWeapon->IsHoldingAmmoFull())
	{
		EquippedWeapon->SetHoldingAmmo(EquippedWeapon->GetMaxHoldingAmmo());
		UGameplayStatics::PlaySoundAtLocation(this, RefillSound, GetActorLocation());
	}
	if(EquippedWeapon->IsHoldingAmmoFull())
	{
		RefillAmmoWidget->SetVisibility(false);
		WeaponAmmoFullWidget->SetVisibility(true);
	}
}

void AAmmobox::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmobox::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AAmmobox::OnSphereEndOverlap);

	if (RefillAmmoWidget && WeaponAmmoFullWidget)
	{
		RefillAmmoWidget->SetVisibility(false);
		WeaponAmmoFullWidget->SetVisibility(false);
	}
}

void AAmmobox::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if (ShooterCharacter && ShooterCharacter->GetCombat() && ShooterCharacter->GetCombat()->GetEquippedWeapon() && !
		ShooterCharacter->GetCombat()->IsHoldingAmmoFull())
	{
		ShooterCharacter->SetOverlappingAmmoBox(this);
		if (RefillAmmoWidget)
			RefillAmmoWidget->SetVisibility(true);
	}
	else if (ShooterCharacter && ShooterCharacter->GetCombat() && ShooterCharacter->GetCombat()->GetEquippedWeapon() &&
		ShooterCharacter->GetCombat()->IsHoldingAmmoFull())
	{
		if (WeaponAmmoFullWidget)
			WeaponAmmoFullWidget->SetVisibility(true);
	}
}

void AAmmobox::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if (ShooterCharacter)
	{
		ShooterCharacter->SetOverlappingAmmoBox(nullptr);
		if (RefillAmmoWidget && WeaponAmmoFullWidget)
		{
			RefillAmmoWidget->SetVisibility(false);
			WeaponAmmoFullWidget->SetVisibility(false);
		}
	}
}

void AAmmobox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
