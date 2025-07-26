// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "NpcInteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UNpcInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetInteractionText(FString& npcText);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Interaction;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Exit;

	UFUNCTION()
	void HoverExitButton();

	UFUNCTION()
	void UnHoverExitButton();

	UFUNCTION()
	void ClickExitButton();

	UPROPERTY()
	FString npcInteractionTalk;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ExitHoverAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ExitUnhoverAnim;

	UPROPERTY()
	class ANpcUits* ownNpc;

	void SetOwnNpc(ANpcUits* inNpc) { ownNpc = inNpc; }
};
