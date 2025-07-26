// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "tradeUi.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UtradeUi : public UUserWidget
{
	GENERATED_BODY()

	int32 price;
	FString tradeItemName;

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* itemNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PriceText;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImg;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Buy_button;

	UFUNCTION(BlueprintCallable)
	void buyUi(const FString& itemname, const int32& itemPrice, const FString& itemImgs);

	UFUNCTION()
	void buttonClickEvent();

	
};
