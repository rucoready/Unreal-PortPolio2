// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Damage;

	UFUNCTION()
	void SetDamageText(float damage);

	UFUNCTION()
	void HideWidget();

	UFUNCTION()
	void PlayNormalDamageAnimation(float damage);

	UFUNCTION()
	void PlayCriticalDamageAnimation(float damage);

	UPROPERTY(Editanywhere, Category = "MySettings", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* damageAnimNormalDamage;

	UPROPERTY(Editanywhere, Category = "MySettings", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* damageAnimCriticalDamage;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* damageContainer;

	
};
