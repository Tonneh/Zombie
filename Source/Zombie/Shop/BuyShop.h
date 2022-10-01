// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuyShop.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ZOMBIE_API UBuyShop : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UButton* M16;

	UPROPERTY(meta = (BindWidget))
	UButton* DessertEagle;
};
