// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "dragVisual.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UdragVisual : public UUserWidget
{
	GENERATED_BODY()
	FString dragItemName;
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* DragImage;

public:
    UFUNCTION(BlueprintCallable)
    void SetItemImage(const FString& ItemImg , const FString& ItemName);


	
};
