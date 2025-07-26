// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../../SpawnActor/TeleporterCristal.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Continue;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Exit;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HoverContinueAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* UnhoverContinueAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HoverExitAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* UnhoverExitAnim;

	UFUNCTION()
	void ContinueHoverButtonAnim();

	UFUNCTION()
	void ContinueUnHoverButtonAnim();

	UFUNCTION()
	void ExitHoverButtonAnim();
	
	UFUNCTION()
	void ExitUnHoverButtonAnim();

	UFUNCTION()
	void ContinueButtonClick();

	UFUNCTION()
	void EixButtonClick();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ATeleporterCristal> cristal;

	ATeleporterCristal* cristalInstance;
};
