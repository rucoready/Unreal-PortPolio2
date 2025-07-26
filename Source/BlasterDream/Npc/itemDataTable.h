// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "itemDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString itemImagePath;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemType;

    //����ϰ�� ġ��Ÿ Ȯ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Probability;

    //�Ǹ��ϴ� NPC �з�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString salesperson;

    FItemData()
        : Price(0)
        , Probability(0.f)
    {}

};


