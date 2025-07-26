// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API USaveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SaveText;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Pannel;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SaveUIAnim;
};
