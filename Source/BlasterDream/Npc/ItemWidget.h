// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
	int hpItemCount = 0;
	int mpItemCount = 0;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HP_TextBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MP_TextBlock;


	void NativeConstruct();

	UFUNCTION()
	void UpdateItemCount(const FString& itemName, int32 itemCount);

};
