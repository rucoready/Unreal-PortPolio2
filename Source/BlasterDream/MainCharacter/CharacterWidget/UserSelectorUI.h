// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../../SpawnActor/TeleporterCristal.h"
#include "UserSelectorUI.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UUserSelectorUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Yes;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_No;

	UFUNCTION()
	void YesButtonClick();

	UFUNCTION()
	void NoButtonClick();

	UFUNCTION()
	void YesButtonHover();

	UFUNCTION()
	void NoButtonHover();

	UFUNCTION()
	void YesButtonUnHover();

	UFUNCTION()
	void NoButtonUnHover();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ButtonYesHoverAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ButtonNoHoverAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ButtonYesUnHoverAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ButtonNoUnHoverAnim;

	class ASwordPlayerGameBase* gameMode;

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<ATeleporterCristal> cristal;

	ATeleporterCristal* cristalInstance;
};
