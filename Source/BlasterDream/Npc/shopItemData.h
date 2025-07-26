// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "shopItemData.generated.h"

USTRUCT(BlueprintType)
struct FshopItemData
{
    GENERATED_BODY()

    //������ �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString shopItemName;

    //������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 itemPrice;

    //������ �ּ�(�̹���)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString itemImagePath;

    FshopItemData()
        : itemPrice(0)
    {}

};
