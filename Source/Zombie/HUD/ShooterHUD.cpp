// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUD.h"
#include "CharacterOverlay.h"
#include "Blueprint/UserWidget.h"
#include "Zombie/PlayerController/ShooterPlayerController.h"
#include "Zombie/Shop/BuyShop.h"

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	
	if (GEngine)
	{
		FVector2D ViewportSize;

		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		if (HUDPackage.CrosshairsCenter)
		{
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter);
		}
		if (HUDPackage.CrosshairsRight)
		{
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter);
		}
		if (HUDPackage.CrosshairsTop)
		{
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter);
		}
	}
}

void AShooterHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AddCharacterOverlay();
}

void AShooterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f),ViewportCenter.Y - (TextureHeight / 2.f));
	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight,
	            0.f, 0.f, 1.f, 1.f, FLinearColor::White);
}

void AShooterHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void AShooterHUD::AddShopOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && ShopOverlayClass)
	{
		ShopOverlay = CreateWidget<UBuyShop>(PlayerController, ShopOverlayClass);
		ShopOverlay->AddToViewport();
	}
}

void AShooterHUD::RemoveShopOverlay()
{
	if (ShopOverlay && ShopOverlayClass)
	{
		ShopOverlay->RemoveFromParent();
	}
}


