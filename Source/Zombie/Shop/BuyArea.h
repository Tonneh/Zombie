// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuyArea.generated.h"

class UBoxComponent;
UCLASS()
class ZOMBIE_API ABuyArea : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuyArea();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BuyBox; 
public:	
	virtual void Tick(float DeltaTime) override;

};
