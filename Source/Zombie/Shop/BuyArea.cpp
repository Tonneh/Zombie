// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyArea.h"

#include "BuyShop.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Zombie/Character/ShooterCharacter.h"
#include "Zombie/PlayerController/ShooterPlayerController.h"

ABuyArea::ABuyArea()
{
	PrimaryActorTick.bCanEverTick = true;

	BuyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Buy Area"));
	SetRootComponent(BuyBox);
	BuyBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BuyBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OpenShopWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OpenShopWidget"));
	OpenShopWidget->SetupAttachment(RootComponent);
}

void ABuyArea::BeginPlay()
{
	Super::BeginPlay();
	BuyBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BuyBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	BuyBox->OnComponentBeginOverlap.AddDynamic(this, &ABuyArea::OnBoxOverlap);
	BuyBox->OnComponentEndOverlap.AddDynamic(this, &ABuyArea::OnBoxEndOverlap);

	if (OpenShopWidget)
		OpenShopWidget->SetVisibility(false);
}


void ABuyArea::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if (ShooterCharacter && OpenShopWidget)
	{
		OpenShopWidget->SetVisibility(true);
		ShooterCharacter->SetOverlappingBuyArea(this);
	}
}

void ABuyArea::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
	if (ShooterCharacter && OpenShopWidget)
	{
		OpenShopWidget->SetVisibility(false);
		if (ShooterCharacter->IsShopping())
			ShooterCharacter->CloseShop();
		ShooterCharacter->SetOverlappingBuyArea(nullptr);
	}
}

void ABuyArea::OpenShop(AShooterCharacter* Character)
{
	if (Character)
	{
		AShooterPlayerController* Controller = Character->GetShooterPlayerController();
		Controller = Controller == nullptr
			             ? Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0))
			             : Controller;
		if (Controller)
		{
			Controller->ShowShop();
		}
	}
}

void ABuyArea::CloseShop(AShooterCharacter* Character)
{
	AShooterPlayerController* Controller = Character->GetShooterPlayerController();
	Controller = Controller == nullptr
					 ? Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0))
					 : Controller;
	if (Controller)
	{
		Controller->RemoveShop();
	}
}

void ABuyArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
