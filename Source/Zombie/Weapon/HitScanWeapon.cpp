// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Zombie/Character/ZombieCharacterBot.h"
#include "Zombie/Zombie.h"
#include "Zombie/Components/CombatComponent.h"
#include "Zombie/PlayerController/ShooterPlayerController.h"

void AHitScanWeapon::Fire()
{
	Super::Fire();
	// Get the hit target by performing a line trace 
	FHitResult HitResult;
	PerformLineTrace(HitResult);
	HitTarget = HitResult.ImpactPoint;

	// Perform a line trace from the socket to the hit target 
	if (const USkeletalMeshSocket* MuzzleFlashSocket = WeaponMesh->GetSocketByName("MuzzleFlash"))
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(WeaponMesh);
		FVector Start = SocketTransform.GetLocation();
		FVector End = Start + (HitTarget - Start) * 1.25f;

		UWorld* World = GetWorld();
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World, BeamParticles, Start, FRotator::ZeroRotator, true);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), End);
			}
		}
		if (World)
		{
			FHitResult FireHit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			Params.AddIgnoredActor(GetOwner());
			Params.bReturnPhysicalMaterial = true;
			World->LineTraceSingleByChannel(
				FireHit,
				Start,
				End,
				ECC_Bullet,
				Params
			);
			if (FireHit.bBlockingHit)
			{
				ShooterPlayerController = ShooterPlayerController == nullptr
					                          ? Cast<AShooterPlayerController>(
						                          UGameplayStatics::GetPlayerController(this, 0))
					                          : ShooterPlayerController;
				switch (FireHit.PhysMaterial->SurfaceType)
				{
				case SurfaceType_Default:
					break;
				case SurfaceType1: // headshot
					if (ShooterPlayerController)
						ShooterPlayerController->ShowHitMarkerHead(); 
					UGameplayStatics::ApplyDamage(FireHit.GetActor(), HeadShotDamage, GetInstigatorController(), this,
					                              UDamageType::StaticClass());
					break;
				case SurfaceType2: // bodyshot
					if (ShooterPlayerController)
						ShooterPlayerController->ShowHitMarkerBody();
					UGameplayStatics::ApplyDamage(FireHit.GetActor(), Damage, GetInstigatorController(), this,
					                              UDamageType::StaticClass());
					break;
				default:
					break;
				}

				// if it hits a character, it'll do different sound 
				if (FireHit.GetActor()->GetClass()->IsChildOf(ACharacter::StaticClass()))
				{
					if (BodyHitSound)
					{
						UGameplayStatics::PlaySoundAtLocation(World, BodyHitSound, FireHit.Location);
					}
					if (BodyImpactParticles)
					{
						UGameplayStatics::SpawnEmitterAtLocation(World, BodyImpactParticles, FireHit.ImpactPoint,
						                                         FireHit.ImpactNormal.Rotation()
						);
					}
				}
				else // if it hits anything else 
				{
					if (ImpactParticles)
					{
						UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint,
						                                         FireHit.ImpactNormal.Rotation()
						);
					}
					if (HitSound)
					{
						UGameplayStatics::PlaySoundAtLocation(World, HitSound, FireHit.Location);
					}
				}
			}
		}
	}
}

void AHitScanWeapon::PerformLineTrace(FHitResult& HitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Gets the crosshairs world position and direction from deprojectscreentoworld
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	// Performs a line trace from middle of crosshair 
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		Character = Character == nullptr ? Cast<AShooterCharacter>(GetOwner()) : Character;
		if (Character)
		{
			const float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		}

		const FVector End = Start + CrosshairWorldDirection * 80000;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Bullet, Params);
	}
}
