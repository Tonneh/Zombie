// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammobox.generated.h"

class UWidgetComponent;
class AWeapon;
class USphereComponent;
UCLASS()
class ZOMBIE_API AAmmobox : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmobox();

	void FillAmmo(AWeapon* EquippedWeapon); 
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UStaticMeshComponent* Mesh; 
		
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere)
	USoundBase* RefillSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UWidgetComponent* RefillAmmoWidget;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UWidgetComponent* WeaponAmmoFullWidget;
public:	
	virtual void Tick(float DeltaTime) override;

};
