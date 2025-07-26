// Fill out your copyright notice in the Description page of Project Settings.


#include "userInvenSlot.h"
#include "Components/Image.h"
#include "../MainCharacter/MainWidget.h"
#include "Input/Reply.h"   
#include "dragVisual.h"
#include "../GameManager/StatGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"



FReply UuserInvenSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) &&
        !(currentItemName == FString(TEXT("NULL"))) )
    {

        // 더블 클릭 판정
        if (clickCount == 0)
        {
            clickCount++;
            currentTime = GetWorld()->GetRealTimeSeconds();
        }     
        else
        {
            LastClickTime = GetWorld()->GetRealTimeSeconds() - currentTime;
            if (LastClickTime <= DoubleClickThreshold)
                DoubleClickEvnet();
            else
            {
                clickCount = LastClickTime = currentTime = 0;
            }
                
        }

        // 드래그 시작 감지도 동시에
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UuserInvenSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>();
    DragDropOp->Payload = this;

    // 이 부분이 핵심!
    if (DragVisualWidgetClass)
    {
        UdragVisual* DragVisual = CreateWidget<UdragVisual>(GetWorld(), DragVisualWidgetClass);
        if (DragVisual)
        {
            DragVisual->SetItemImage(currentItemImgPath, currentItemName);  // 문자열 경로
            DragDropOp->DefaultDragVisual = DragVisual;
            if (equipmentItem)
            {
                UStatGameInstance* GI = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
                GI->userInstallation(currentItemName, false);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("DragVisual widget fail"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("DragVisualWidgetClass Not assigned"));
    }

    OutOperation = DragDropOp;
}

bool UuserInvenSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UStatGameInstance* GI = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (UuserInvenSlot* SourceSlot = Cast<UuserInvenSlot>(InOperation->Payload))
    {
        // 현재 슬롯 정보 백업
        FString TempName = currentItemName;
        FString TempImg = currentItemImgPath;

        // 일반 인벤토리 슬롯일 경우
        if (!equipmentItem)
        {
            if (currentItemName == TEXT("NULL"))
            {
                // 빈 슬롯일 때 이동
                slotImgSeting(SourceSlot->currentItemImgPath, SourceSlot->currentItemName);

                // 인벤토리 수량 감소 처리
                GI->RemoveItem(SourceSlot->currentItemName);

                if (UMainWidget* OwnerWidget = Cast<UMainWidget>(GetOuter()))
                {
                    int32 SourceIndex = OwnerWidget->InvenSlots.Find(SourceSlot);
                    if (SourceIndex != INDEX_NONE)
                    {
                        GI->SlotItems.Remove(SourceIndex);
                    }
                }

                SourceSlot->nullImg();  
            }
            else
            {
                // 교체 (swap)
                slotImgSeting(SourceSlot->currentItemImgPath, SourceSlot->currentItemName);
                SourceSlot->slotImgSeting(TempImg, TempName);
            }

            return true;
        }

        // 장비 슬롯일 경우
        else
        {
            if (!GI)
            {
                return false; // GameInstance 못 찾았을 때
            }

            bool bCanEquip = GI->checkEquipment(SourceSlot->currentItemName);
            if (!bCanEquip)
            {
                return false; // 장비 불가 아이템
            }

            if (currentItemName == TEXT("NULL"))
            {
                // 빈 장비 슬롯 → 장착
                slotImgSeting(SourceSlot->currentItemImgPath, SourceSlot->currentItemName);

                GI->RemoveItem(SourceSlot->currentItemName);

                SourceSlot->nullImg();
            }
            else
            {
                // 장비 간 교체
                slotImgSeting(SourceSlot->currentItemImgPath, SourceSlot->currentItemName);
                SourceSlot->slotImgSeting(TempImg, TempName);
            }
            GI->userInstallation(currentItemName, true);

            return true;
        }
    }

    return false;
}




void UuserInvenSlot::slotImgSeting(const FString& ItemImg, const FString& ItemName)
{
	ItemSlotImg = Cast<UImage>(GetWidgetFromName(TEXT("ItemSlotImg")));
    currentItemName = ItemName;
    currentItemImgPath = ItemImg;
    ItemCountText->SetVisibility(ESlateVisibility::Visible);
    SetItemCount(1);
	if (ItemSlotImg)
	{
        UTexture2D* ItemTexture = LoadObject<UTexture2D>(nullptr, *ItemImg);

        if (ItemTexture)
        {
            //브러시 생성 후 설정
            FSlateBrush Brush;
            Brush.SetResourceObject(ItemTexture);
            //Brush.ImageSize = FVector2D(64,64); // 필요시 크기 지정

            ItemSlotImg->SetBrush(Brush);

        }
	}
}




void UuserInvenSlot::DoubleClickEvnet()
{
    UStatGameInstance* GI = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
    GI->usingItem(currentItemName);
}

bool UuserInvenSlot::invenSlotNull()
{
    if (currentItemName == TEXT("NULL"))
        return true;
    return false;
}

void UuserInvenSlot::SetItemCount(const int32& countItems)
{
    currentCount = countItems;
    ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), currentCount)));
}

FString UuserInvenSlot::GetInvenInItemName()
{
    return currentItemName;
}


void UuserInvenSlot::nullImg()
{
    ItemSlotImg = Cast<UImage>(GetWidgetFromName(TEXT("ItemSlotImg")));
    currentItemName = TEXT("NULL");
    currentItemImgPath = "";

    ItemCountText->SetVisibility(ESlateVisibility::Hidden);

    if (ItemSlotImg)
    {
        UTexture2D* ItemTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/DoveDove/UI/Img/NULLimg.NULLimg'"));

        if (ItemTexture)
        {
            //브러시 생성 후 설정
            FSlateBrush Brush;
            Brush.SetResourceObject(ItemTexture);
            //Brush.ImageSize = FVector2D(64,64); // 필요시 크기 지정

            ItemSlotImg->SetBrush(Brush);

        }


    }
}



