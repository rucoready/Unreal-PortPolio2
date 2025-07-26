// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Image.h"
#include "../Npc/tradeUi.h"
#include "../Npc/userInvenSlot.h"
#include "../GameManager/StatGameInstance.h"
#include "Kismet/GameplayStatics.h"



void UMainWidget::NativeConstruct()
{
    visbleInven();
    visbleUserInstal();
    closeShop();

    

    UStatGameInstance* GameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        SlotItes = GameInstance->GetSlotItems();
        InstalItems = GameInstance->GetInstalItems();
        changeMoney(GameInstance->playerMoney);
    }

    InvenSlots.Empty();  // 기존 슬롯 초기화


    for (int32 i = 0; i < 25; ++i)
    {
        FString SlotName = FString::Printf(TEXT("inVen_%d"), i + 1);

        if (UuserInvenSlot* FoundSlot = Cast<UuserInvenSlot>(GetWidgetFromName(FName(*SlotName))))
        {
            InvenSlots.Add(FoundSlot);

            if (SlotItes.Contains(i))
            {
                const FString& ItemName = SlotItes[i];
                FString ImgPath = GameInstance->CallItemImg(ItemName);
                FoundSlot->slotImgSeting(ImgPath, ItemName);
            }
            else
                FoundSlot->nullImg();

        }
    }

    InstalSlots.Empty();

    for (int32 i = 0; i < 3; ++i)
    {
        FString SlotName = FString::Printf(TEXT("instalItems_%d"), i);

        if (UuserInvenSlot* FoundSlot = Cast<UuserInvenSlot>(GetWidgetFromName(FName(*SlotName))))
        {
            InstalSlots.Add(FoundSlot);

            if (InstalItems.Contains(i))
            {
                const FString& ItemName = InstalItems[i];
                FString ImgPath = GameInstance->CallItemImg(ItemName);
                FoundSlot->slotImgSeting(ImgPath, ItemName);
            }
            else
                FoundSlot->nullImg();

        }
    }

    bIsInitialized = true;

    visblegKey(false);

    SetInventoryUI();
    changeMoney(1300);
}

void UMainWidget::OpenShop()
{
    Trade_ScrollBox->ClearChildren(); 
    Shop_visible = true;
    visblegKey(false);
    trade_Canvas->SetVisibility(ESlateVisibility::Visible);
}

void UMainWidget::closeShop()
{
    trade_Canvas->SetVisibility(ESlateVisibility::Hidden);
    Trade_ScrollBox->ClearChildren();
    Shop_visible = false;
}

void UMainWidget::visbleInven()
{
    inven_visible = !inven_visible;
    if (inven_visible)
    {
        inven_Canvas->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        inven_Canvas->SetVisibility(ESlateVisibility::Hidden);
    }

}

void UMainWidget::visbleUserInstal()
{
    UserInstal_visible = !UserInstal_visible;
    if (UserInstal_visible)
    {
        userInstal->SetVisibility(ESlateVisibility::Visible);
    }

    else
        userInstal->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidget::visblegKey(const bool& onKeyUi)
{
    if(onKeyUi && !Shop_visible)
        gKey_Canves->SetVisibility(ESlateVisibility::Visible);
    else
        gKey_Canves->SetVisibility(ESlateVisibility::Hidden);
}




void UMainWidget::showShopBox(const FString& ItemName, const int32& ImagePrice, const FString& itemImage)
{
    UtradeUi* TradeWidget = CreateWidget<UtradeUi>(GetWorld(), tradeUiClass);
    if (TradeWidget)
    {
        TradeWidget->buyUi(ItemName, ImagePrice, itemImage);
        Trade_ScrollBox->AddChild(TradeWidget);
    }
}


void UMainWidget::InvenIn(const FString& ItemName, const FString& ImageFind)
{
    for (int num = 0; num < InvenSlots.Num(); num++)
    {
        if (!InvenSlots[num])
        {
            UE_LOG(LogTemp, Error, TEXT("InvenSlots[%d] is null"), num);
            continue; // 또는 return;
        }

        if (InvenSlots[num]->invenSlotNull())
        {
            InvenSlots[num]->slotImgSeting(ImageFind, ItemName);
            break;
        }
    }
}

void UMainWidget::InvenInCount(const FString& ItemName, const int32& ItemCount)
{
    for (int num = 0; num < InvenSlots.Num(); num++)
    {
        if (InvenSlots[num]->GetInvenInItemName() == ItemName)
        {
            InvenSlots[num]->SetItemCount(ItemCount);
        }
    }
}

void UMainWidget::InvenRemove(const FString& ItemName, const int32& itemCount)
{
    FString removeItemName;
    for (int num = 0; num < InvenSlots.Num(); num++)
    {
        removeItemName = InvenSlots[num]->GetInvenInItemName();
        if (removeItemName == ItemName && itemCount == 0)
            InvenSlots[num]->nullImg();
    }
}


TMap<int32, FString> UMainWidget::SlotSave()
{
    SlotItes.Empty();

    for (int32 i = 0; i < InvenSlots.Num(); ++i)
    {
        FString ItemName = InvenSlots[i]->GetInvenInItemName();
        if (ItemName != TEXT("NULL"))
        {
            SlotItes.Add(i, ItemName);
        }
    }

    return SlotItes;
}

TMap<int32, FString> UMainWidget::InstalSlotSave()
{
    InstalItems.Empty();

    for (int32 i = 0; i < InstalSlots.Num(); ++i)
    {
        FString ItemName = InstalSlots[i]->GetInvenInItemName();
        if (ItemName != TEXT("NULL"))
        {
            InstalItems.Add(i, ItemName);
        }
    }

    return InstalItems;
}

void UMainWidget::changeMoney(int32 getMoney)
{
    money = getMoney;
    UserMoney->SetText(FText::FromString(FString::Printf(TEXT("%d"), money)));
}

void UMainWidget::SetInventoryUI()
{
    UStatGameInstance* GameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInstance) return;

    const TMap<int32, FString>& SavedInven = GameInstance->GetSlotItems();
    const TMap<int32, FString>& SavedEquip = GameInstance->GetInstalItems();
    const TMap<FString, int32>& ItemInventory = GameInstance->ItemInventory;

    changeMoney(GameInstance->playerMoney);

    // 인벤토리 슬롯 초기화
    if (InvenSlots.Num() != 25)
    {
        InvenSlots.Empty();
        for (int32 i = 0; i < 25; ++i)
        {
            FString SlotName = FString::Printf(TEXT("inVen_%d"), i + 1);
            if (UuserInvenSlot* FoundSlot = Cast<UuserInvenSlot>(GetWidgetFromName(FName(*SlotName))))
            {
                InvenSlots.Add(FoundSlot);
            }
        }
    }

    for (int32 i = 0; i < InvenSlots.Num(); ++i)
    {
        if (!InvenSlots[i]) continue;

        if (SavedInven.Contains(i))
        {
            const FString& ItemName = SavedInven[i];
            FString ImgPath = GameInstance->CallItemImg(ItemName);

            int32 Count = 1;
            if (ItemInventory.Contains(ItemName))
            {
                Count = ItemInventory[ItemName];
            }

            InvenSlots[i]->slotImgSeting(ImgPath, ItemName);
            InvenSlots[i]->SetItemCount(Count);
        }
        else
        {
            InvenSlots[i]->nullImg();
        }
    }

    // 장비 슬롯 초기화
    if (InstalSlots.Num() != 3)
    {
        InstalSlots.Empty();
        for (int32 i = 0; i < 3; ++i)
        {
            FString SlotName = FString::Printf(TEXT("instalItems_%d"), i);
            if (UuserInvenSlot* FoundSlot = Cast<UuserInvenSlot>(GetWidgetFromName(FName(*SlotName))))
            {
                InstalSlots.Add(FoundSlot);
            }
        }
    }

    for (int32 i = 0; i < InstalSlots.Num(); ++i)
    {
        if (!InstalSlots[i]) continue;

        if (SavedEquip.Contains(i))
        {
            const FString& ItemName = SavedEquip[i];
            FString ImgPath = GameInstance->CallItemImg(ItemName);

            InstalSlots[i]->slotImgSeting(ImgPath, ItemName);
            InstalSlots[i]->SetItemCount(1); // 장비칸은 기본적으로 1개만
        }
        else
        {
            InstalSlots[i]->nullImg();
        }
    }

    // 좌상단 포션 개수 최신화
    if (GameInstance)
    {
        int32 HpCount = 0;
        int32 MpCount = 0;

        for (const auto& Elem : ItemInventory)
        {
            if (Elem.Key == TEXT("HP_potion"))
            {
                HpCount = Elem.Value;
            }
            else if (Elem.Key == TEXT("MP_potion"))
            {
                MpCount = Elem.Value;
            }
        }

        GameInstance->OItemUpdate.Broadcast(TEXT("HP_potion"), HpCount);
        GameInstance->OItemUpdate.Broadcast(TEXT("MP_potion"), MpCount);
    }
    
}




