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

        // ���� Ŭ�� ����
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

        // �巡�� ���� ������ ���ÿ�
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UuserInvenSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>();
    DragDropOp->Payload = this;

    // �� �κ��� �ٽ�!
    if (DragVisualWidgetClass)
    {
        UdragVisual* DragVisual = CreateWidget<UdragVisual>(GetWorld(), DragVisualWidgetClass);
        if (DragVisual)
        {
            DragVisual->SetItemImage(currentItemImgPath, currentItemName);  // ���ڿ� ���
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
        // ���� ���� ���� ���
        FString TempName = currentItemName;
        FString TempImg = currentItemImgPath;

        // �Ϲ� �κ��丮 ������ ���
        if (!equipmentItem)
        {
            if (currentItemName == TEXT("NULL"))
            {
                // �� ������ �� �̵�
                slotImgSeting(SourceSlot->currentItemImgPath, SourceSlot->currentItemName);

                // �κ��丮 ���� ���� ó��
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
                // ��ü (swap)
                slotImgSeting(SourceSlot->currentItemImgPath, SourceSlot->currentItemName);
                SourceSlot->slotImgSeting(TempImg, TempName);
            }

            return true;
        }

        // ��� ������ ���
        else
        {
            if (!GI)
            {
                return false; // GameInstance �� ã���� ��
            }

            bool bCanEquip = GI->checkEquipment(SourceSlot->currentItemName);
            if (!bCanEquip)
            {
                return false; // ��� �Ұ� ������
            }

            if (currentItemName == TEXT("NULL"))
            {
                // �� ��� ���� �� ����
                slotImgSeting(SourceSlot->currentItemImgPath, SourceSlot->currentItemName);

                GI->RemoveItem(SourceSlot->currentItemName);

                SourceSlot->nullImg();
            }
            else
            {
                // ��� �� ��ü
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
            //�귯�� ���� �� ����
            FSlateBrush Brush;
            Brush.SetResourceObject(ItemTexture);
            //Brush.ImageSize = FVector2D(64,64); // �ʿ�� ũ�� ����

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
            //�귯�� ���� �� ����
            FSlateBrush Brush;
            Brush.SetResourceObject(ItemTexture);
            //Brush.ImageSize = FVector2D(64,64); // �ʿ�� ũ�� ����

            ItemSlotImg->SetBrush(Brush);

        }


    }
}



