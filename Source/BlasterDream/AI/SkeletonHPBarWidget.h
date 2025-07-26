// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkeletonHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API USkeletonHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* pb_healthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* pb_healthBar_DelayBar;

	UFUNCTION()
	void updateHpBar(float currentHp, float maxHp);

	FTimerHandle th_DelayBar;
	float targetDelayBarPercent = 1.0;

	UFUNCTION()
	void UpdateDelayBar();

	float currentDelayPercent;
	
};
