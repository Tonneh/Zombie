// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyArea.h"

#include "Components/BoxComponent.h"

ABuyArea::ABuyArea()
{
	PrimaryActorTick.bCanEverTick = true;

	BuyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Buy Area"));
	BuyBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BuyBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABuyArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuyArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

