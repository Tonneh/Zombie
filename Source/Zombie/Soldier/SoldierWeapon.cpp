// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierWeapon.h"
#include "Components/SkeletalMeshComponent.h" 
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Zombie/Zombie.h"

void ASoldierWeapon::Fire()
{
	Super::Fire();

	if (const USkeletalMeshSocket* MuzzleFlashSocket = WeaponMesh->GetSocketByName("MuzzleFlash"))
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(WeaponMesh);
		UWorld* World = GetWorld();
		FHitResult FireHit;
		FVector ShotDirection;
		FVector Start = SocketTransform.GetLocation();
		
		PerformLineTrace(FireHit, ShotDirection);
		
		if (BeamParticles)
		{
			if (UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World, BeamParticles, Start, FRotator::ZeroRotator, true))
			{
				Beam->SetVectorParameter(FName("Target"), FireHit.Location);
			}
		}
		if (World)
		{
			if (FireHit.bBlockingHit)
			{
				UGameplayStatics::ApplyDamage(FireHit.GetActor(), Damage, GetInstigatorController(), this,
				                              UDamageType::StaticClass());

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
				if (MuzzleFlash)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
				}
			}
		}
	}
}

void ASoldierWeapon::PerformLineTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	OwnerPawn = OwnerPawn == nullptr ? Cast<APawn>(GetOwner()) : OwnerPawn;
	if (OwnerPawn == nullptr) return; 
	const AController* OwnerController = OwnerPawn->GetController(); 
	if (OwnerController == nullptr) return; 
	FVector Location; 
	FRotator Rotation; 
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector(); 
	const FVector End = Location + Rotation.Vector() * 80000; 
	FCollisionQueryParams Params; 
	Params.AddIgnoredActor(this); 
	Params.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECC_Bullet, Params);
}
