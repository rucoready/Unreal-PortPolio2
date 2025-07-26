// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapNameUI.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UMapNameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_MapName;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* MapNameAnim;

	UPROPERTY(EditAnywhere, Category="MySettings")
	FString mapName;

	UFUNCTION()
	void playMapNameWidget();
};
