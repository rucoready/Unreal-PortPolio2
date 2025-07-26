// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyUIDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UEnemyUIDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_HPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_HPDisplay;

	void updateHpBar(FString name, float currentHp, float maxHp);

	
	
};
