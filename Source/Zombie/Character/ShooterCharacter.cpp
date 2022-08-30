// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Zombie/HUD/ShooterHUD.h"
#include "Zombie/PlayerController/ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Components/CombatComponent.h"
#include "Zombie/Weapon/Knife.h"
#include "Zombie/Weapon/Weapon.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	
}

void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->Character = this; 
	}
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetHUDHealth();
	SetCrossHairs();
	SpawnDefaultWeapon();
}

float AShooterCharacter::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	UAnimInstance * AnimInstance = (Mesh1P) ? Mesh1P->GetAnimInstance() : nullptr;
	if (AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

		if (Duration > 0.f)
		{
			// Start at a given Section.
			if (StartSectionName != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
			}

			return Duration;
		}
	}

	return 0.f;
}

void AShooterCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AShooterCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AShooterCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AShooterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AShooterCharacter::EquipButtonPressed()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
		SetHUDAmmo();
	}
}

void AShooterCharacter::FireButtonPressed()
{
	if (Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void AShooterCharacter::FireButtonReleased()
{
	if (Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void AShooterCharacter::AimButtonPressed()
{
	bAiming = true;
	if (Combat && Combat->EquippedWeapon)
	{
		Combat->AttachActorToAimingSocket(Combat->EquippedWeapon);
	}
	HideCrossHairs();
}

void AShooterCharacter::AimButtonReleased()
{
	bAiming = false;
	if (Combat && Combat->EquippedWeapon)
	{
		Combat->AttachActorToRightHand(Combat->EquippedWeapon);
	}
	SetCrossHairs();
}

void AShooterCharacter::ReloadButtonPressed()
{
	if (ReloadMontage && Combat && Combat->CanReload())  
	{
		Combat->Reload();
	}
}

void AShooterCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && DefaultKnife)
	{
		AKnife* Knife = World->SpawnActor<AKnife>(DefaultKnife);
		if (Combat)
		{
			Combat->EquipKnife(Knife);
		}
	}

}

void AShooterCharacter::KnifeButtonPressed()
{
	if (Combat && Combat->Knife)
	{
		Combat->KnifeAttack();
	}
}

void AShooterCharacter::SetCrossHairs()
{
	if (Controller == nullptr) return;
	ShooterController = ShooterController == nullptr ? Cast<AShooterPlayerController>(Controller) : ShooterController;
	if (ShooterController)
	{
		HUD = HUD == nullptr ? Cast<AShooterHUD>(ShooterController->GetHUD()) : HUD;
		if (HUD)
		{
			FHUDPackage HUDPackage;
			HUDPackage.CrosshairsBottom = CrosshairsBottom;
			HUDPackage.CrosshairsCenter = CrosshairsCenter;
			HUDPackage.CrosshairsTop = CrosshairsTop;
			HUDPackage.CrosshairsLeft = CrosshairsLeft;
			HUDPackage.CrosshairsRight = CrosshairsRight;
			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void AShooterCharacter::HideCrossHairs()
{
	if (Controller == nullptr) return;
	ShooterController = ShooterController == nullptr ? Cast<AShooterPlayerController>(Controller) : ShooterController;
	if (ShooterController)
	{
		HUD = HUD == nullptr ? Cast<AShooterHUD>(ShooterController->GetHUD()) : HUD;
		if (HUD)
		{
			FHUDPackage HUDPackage;
			HUDPackage.CrosshairsBottom = nullptr;
			HUDPackage.CrosshairsCenter = nullptr;
			HUDPackage.CrosshairsTop = nullptr;
			HUDPackage.CrosshairsLeft = nullptr;
			HUDPackage.CrosshairsRight = nullptr;
			HUD->SetHUDPackage(HUDPackage);
		}
	}
}


void AShooterCharacter::SetHUDHealth()
{
	ShooterController = ShooterController == nullptr ? Cast<AShooterPlayerController>(Controller) : ShooterController;
	if (ShooterController)
	{
		ShooterController->SetHUDHealth(Health, MaxHealth);
	}
}

void AShooterCharacter::SetHUDAmmo()
{
	ShooterController = ShooterController == nullptr ? Cast<AShooterPlayerController>(Controller) : ShooterController;
	if (ShooterController && Combat && Combat->EquippedWeapon)
	{
		ShooterController->SetHUDAmmo(Combat->EquippedWeapon->GetAmmo(), Combat->HoldingAmmo);
	}
}

void AShooterCharacter::PlayReloadAnimation()
{
	if (ReloadMontage)
	{
		AShooterCharacter::PlayAnimMontage(ReloadMontage);
	}
}

void AShooterCharacter::PlayKnifeAttackAnimation()
{
	if (KnifeAttackMontage && KnifeAttackMontage2)
	{
		bool Rand = FMath::RandBool();
		if (Rand)
		{
			PlayAnimMontage(KnifeAttackMontage);
		}
		else
		{
			PlayAnimMontage(KnifeAttackMontage2);
		}
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AShooterCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AShooterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("PrimaryAction", EInputEvent::IE_Released, this, &AShooterCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("Aiming", EInputEvent::IE_Pressed, this, &AShooterCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aiming", EInputEvent::IE_Released, this, &AShooterCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AShooterCharacter::ReloadButtonPressed);
	PlayerInputComponent->BindAction("Knife", EInputEvent::IE_Pressed, this, &AShooterCharacter::KnifeButtonPressed);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AShooterCharacter::Turn);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AShooterCharacter::LookUp);
}

bool AShooterCharacter::IsWeaponEquipped() const
{
	return (Combat && Combat->EquippedWeapon);
}
