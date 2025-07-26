// Fill out your copyright notice in the Description page of Project Settings.


#include "dragVisual.h"
#include "Components/Image.h"

void UdragVisual::SetItemImage(const FString& ItemImg, const FString& ItemName)
{
    if (!DragImage)
    {
        return;
    }

    UTexture2D* ItemTexture = LoadObject<UTexture2D>(nullptr, *ItemImg);
    if (!ItemTexture)
    {
        return;
    }

    FSlateBrush Brush;
    Brush.SetResourceObject(ItemTexture);
    dragItemName = ItemName;
    DragImage->SetBrush(Brush);
}
