// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "StartMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UStartMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_GameStart;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_GameEnd;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel_Main;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GameStartAnimHover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GameStartAnimUnHover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GameEndAnimHover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GameEndAnimUnHover;

	UFUNCTION()
	void OnClickGameStartButton();

	UFUNCTION()
	void OnClickGameEndButton();

	UFUNCTION()
	void OnHoverGameStartButton();

	UFUNCTION()
	void OnHoverGameEndButton();

	UFUNCTION()
	void OnUnHoverGameStartButton();

	UFUNCTION()
	void OnUnHoverGameEndButton();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class ULevelSequence* gameStartButtonClickSequence;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FName cameraActorTag;

	UFUNCTION()
	void OnMove2PannelSequenceFinished();

	//secondPannel
	UPROPERTY(meta = (BindWidget))
	UButton* Button_NewGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Continue;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_GoFirstPannel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel_SecondPannel;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* NewGameHover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* NewGameUnHover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ContinueHover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ContrinueUnhover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GoFirstHover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* GoFirstUnHover;

	UFUNCTION()
	void OnClickNewGameButton();

	UFUNCTION()
	void OnClickContinueGameButton();

	UFUNCTION()
	void OnHoverNewGame();

	UFUNCTION()
	void OnUnHoverNewGame();

	UFUNCTION()
	void OnHoverContinue();

	UFUNCTION()
	void OnUnHoverContinue();

	UFUNCTION()
	void OnHoverGoFirst();

	UFUNCTION()
	void OnUnHoverGoFist();

	UFUNCTION()
	void OnClickGoFist();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class ULevelSequence* goFirstPannelSequence;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FName cameraActorTagSecond;

	UFUNCTION()
	void OnMoveGoFirstSequenceFinish();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UFadeInOutWidget> fadeWidget;

	UFadeInOutWidget* fadeWidgetInstance;

	UFUNCTION()
	void openLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString targetLoadingMap;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel_NoSav;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_NoSavEnter;

	UFUNCTION()
	void OnHoverNoSavEnter();

	UFUNCTION()
	void OnUnHoverNoSavEnter();

	UFUNCTION()
	void OnClickNoSavEnter();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* NoSaveEnterHover;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* NoSaveEnterUnHover;

};
