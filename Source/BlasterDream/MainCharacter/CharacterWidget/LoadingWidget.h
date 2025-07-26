// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Dot1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Dot2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Dot3;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Dot1Animation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Dot2Animation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Dot3Animation;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Loading;

	//loading screen
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_LoadingKatana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UMediaPlayer* katanaLoadingPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UMediaSource* katanaMediaPlayer;


	UFUNCTION()
	void LoadingStart();

	UFUNCTION()
	void UpdateProgressBar();
	FTimerHandle th_Progressbar;
	float elapsedTime = 0.0f;
	float totalTime = 1.0f;

	UFUNCTION()
	void StartDotAnim();

	FTimerHandle th_DotAnimation;

	UPROPERTY(EditAnywhere, Category="MySettings")
	FString lastDestination;


};
