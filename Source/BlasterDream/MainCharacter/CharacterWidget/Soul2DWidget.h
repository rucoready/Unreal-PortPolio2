// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Soul2DWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API USoul2DWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Soul;

	virtual void NativeConstruct() override;


};
