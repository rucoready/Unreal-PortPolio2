// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DragonWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UDragonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	UFUNCTION()
	void updateSpaceCountDown(float coolTime);

	UFUNCTION()
	void updateQCountDown(float coolTime);

	UFUNCTION()
	void updateWCountDown(float coolTime);

	UFUNCTION()
	void updateSpaceCoolTimeText();

	UFUNCTION()
	void updateQCoolTimeText();

	UFUNCTION()
	void updateWCoolTimeText();

	

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* GlideAnimation;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Space;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* QAnimation;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Q;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* WAnimation;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_WSkill;

	FTimerHandle th_skillSpaceCoolTime;
	float currentSpaceCoolTime;

	FTimerHandle th_skillQCoolTime;
	float currentQCoolTime;

	FTimerHandle th_skillWCoolTime;
	float currentWCoolTime;

	//dragon orb
	UPROPERTY(meta = (BindWidget))
	UImage* Image_DragonEnergyOrb;

	UPROPERTY()
	UMaterialInstanceDynamic* dynamicMaterial;

	UFUNCTION()
	void SetPercent(float percent);

	void StartPercentageOrbCountDown();
	void UpdatePercentageOrb();
	FTimerHandle th_OrbPercentage;
	float orbElapsedTime = 0.f;
	float duration = 60.f;
	float timeInterval = 0.01;

	//owner cast
	class ADragonCharacter* dragonCharacter;
	
};
