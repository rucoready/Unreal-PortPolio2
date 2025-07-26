// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManager/StatGameInstance.h"
#include "Components/TextBlock.h"


void UItemWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UStatGameInstance* GameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        GameInstance->OItemUpdate.AddDynamic(this, &UItemWidget::UpdateItemCount);
    }
}

void UItemWidget::UpdateItemCount(const FString& itemName, int32 itemCount)
{
    if (itemName == "HP_potion")
    {
        HP_TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), itemCount)));
    }
    else if (itemName == "MP_potion")
    {
        MP_TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), itemCount)));
    }

}