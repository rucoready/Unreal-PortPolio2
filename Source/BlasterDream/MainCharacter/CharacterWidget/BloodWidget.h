// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BloodWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UBloodWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BloodAnim;

	UFUNCTION()
	void PlayBloodAnimation();
};
