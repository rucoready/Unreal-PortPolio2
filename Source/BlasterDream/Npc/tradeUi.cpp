// Fill out your copyright notice in the Description page of Project Settings.


#include "tradeUi.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "../GameManager/StatGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "itemDataTable.h"
#include "Engine/DataTable.h"

void UtradeUi::NativeConstruct()
{
    Super::NativeConstruct();

    if (Buy_button)
    {
        Buy_button->OnClicked.AddDynamic(this, &UtradeUi::buttonClickEvent);
    }
}



void UtradeUi::buyUi(const FString& itemname, const int32& itemPrice, const FString& itemImgs)
{
     itemNameText->SetText(FText::FromString(itemname));
     PriceText->SetText(FText::FromString(FString::Printf(TEXT("%d"), itemPrice)));
    //PriceText->SetText(FText::FromString(itemPrice));
     tradeItemName = itemname;
     price = itemPrice;

     ItemImg = Cast<UImage>(GetWidgetFromName(TEXT("ItemImg")));
     if (ItemImg)
     {
        UTexture2D* ItemTexture = LoadObject<UTexture2D>(nullptr, *itemImgs);

        if (ItemTexture)
        {
            //�귯�� ���� �� ����
            FSlateBrush Brush;
            Brush.SetResourceObject(ItemTexture);
            //Brush.ImageSize = FVector2D(64,64); // �ʿ�� ũ�� ����

            ItemImg->SetBrush(Brush);
        }
    }
}

void UtradeUi::buttonClickEvent()
{
    //���� �ִٸ� ToDo
    UStatGameInstance* GI = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
    bool tradeItems = false;
    if (GI)
    {
        tradeItems = GI->buyItemsMoney(price);
        if (tradeItems)
        {
            GI->AddItem(tradeItemName);
        }
        else
            return;
    }

}
