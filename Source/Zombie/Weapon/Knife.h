// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knife.generated.h"

class AShooterCharacter;
class USphereComponent;
UCLASS()
class ZOMBIE_API AKnife : public AActor
{
	GENERATED_BODY()

public:
	AKnife();

	void KnifeSwing();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AShooterCharacter* Character;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* KnifeMesh;

	UPROPERTY(EditAnywhere)
	float Damage = 100.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* SwingSound;

public:
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE USoundBase* GetSwingSound() const { return SwingSound; }
};
