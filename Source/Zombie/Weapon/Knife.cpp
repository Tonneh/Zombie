// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Zombie/Zombie.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "Zombie/Character/ZombieCharacterBot.h"

AKnife::AKnife()
{
	PrimaryActorTick.bCanEverTick = true;

	KnifeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(KnifeMesh);
	KnifeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	KnifeMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	KnifeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKnife::KnifeSwing()
{
	Character = Character == nullptr ? Cast<AShooterCharacter>(GetOwner()) : Character;
	if (Character && Character->GetArrowComponent())
	{
		TArray<AActor*> HitActors;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(Character);
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		UClass* SeekClass = AZombieCharacterBot::StaticClass();
		FVector HitLocation = Character->GetArrowComponent()->GetComponentLocation();
		UKismetSystemLibrary::SphereOverlapActors(this, HitLocation, 100,
		                                          ObjectTypes, SeekClass, IgnoreActors,
		                                          HitActors);
		for (const auto HitActor : HitActors)
		{
			if (HitCounts >= AmountOfHitsPerSwing)
				break;
			if (AlreadyHitActors.Contains(HitActor)) return;
			AlreadyHitActors.AddUnique(HitActor);
			UGameplayStatics::ApplyDamage(HitActor, Damage, GetInstigatorController(), this,
			                              UDamageType::StaticClass());
			HitCounts++; 
			if (HitParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, HitActor->GetActorLocation(),
				                                         HitActor->GetActorRotation());
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitActor->GetActorLocation());
			}
		}
	}
}

void AKnife::BeginPlay()
{
	Super::BeginPlay();
}

void AKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("%d"), HitCounts);
}
