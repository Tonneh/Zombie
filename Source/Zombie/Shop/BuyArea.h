// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuyArea.generated.h"

class AShooterCharacter;
class UBuyShop;
class UWidgetComponent;
class UBoxComponent;
UCLASS()
class ZOMBIE_API ABuyArea : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuyArea();
	
	void OpenShop(AShooterCharacter* Character);
	void CloseShop(AShooterCharacter* Character); 
protected:
	virtual void BeginPlay() override;

	UFUNCTION() 
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						 const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BuyBox;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UWidgetComponent* OpenShopWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BuyShopClass; 

public:	
	virtual void Tick(float DeltaTime) override;

};
