// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeInOutWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UFadeInOutWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Fade;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeOut;

	UFUNCTION()
	void PlayFadeIn();

	UFUNCTION()
	void PlayFadeOut();


};
