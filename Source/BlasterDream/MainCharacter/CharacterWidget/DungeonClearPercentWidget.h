// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "DungeonClearPercentWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UDungeonClearPercentWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	void setPercent(float percent);
	void setTextPercent(float percent);

	UFUNCTION()
	void ClickQuitButton();

	UFUNCTION()
	void ClickHideLastPannel();

	UPROPERTY(meta = (BindWidget))
	UImage* Image_PercentCircle;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Percent;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Quit;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_ClearPannelRemove;

	UPROPERTY()
	UMaterialInstanceDynamic* dynamicMaterial;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UUserSelectorUI> selectorWidget;

	UUserSelectorUI* selectorWidgetInstance;

	//clear Widget
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel_Clear;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ClearTime;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_KillCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_AttackCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ReviveCount;

	UFUNCTION()
	void SetClearPanelNumber(int32 clearTime, int32 killCount, int32 attackCount, int32 reviveCount);

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ClearAnim;

	UFUNCTION()
	void playClearWidgetAnim();

};
