// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "shopItemData.generated.h"

USTRUCT(BlueprintType)
struct FshopItemData
{
    GENERATED_BODY()

    //아이템 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString shopItemName;

    //아이템 가격
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 itemPrice;

    //아이템 주소(이미지)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString itemImagePath;

    FshopItemData()
        : itemPrice(0)
    {}

};
