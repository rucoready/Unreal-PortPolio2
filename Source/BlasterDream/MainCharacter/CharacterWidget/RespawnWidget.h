// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RespawnWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API URespawnWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UButton* Button_Respawn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_TimerText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DieLogoAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RespawnHoverAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RespawnUnHoverAnim;

	UFUNCTION()
	void ShowButton();

	UFUNCTION()
	void ClickRespawnButton();

	UFUNCTION()
	void HoverRespawnButton();

	UFUNCTION()
	void UnhoverRespawnButton();

	UFUNCTION()
	void UpdateTimerText();

	UFUNCTION()
	void CountdownText();

	UPROPERTY(EditAnywhere, Category="MySettings")
	int32 countValue = 5;

	FTimerHandle th_showButton;
};
