// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"  
#include "dragVisual.h"
#include "userInvenSlot.generated.h"
/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UuserInvenSlot : public UUserWidget
{
	GENERATED_BODY()
	
	FString slotItemName ;
	int clickCount = 0;
	//이 슬롯의 번호 즉 몇번째 있는지 확인용도
	float currentTime = 0;
	float LastClickTime = 0;
	const float DoubleClickThreshold = 0.45f;
	

	FString currentItemName = FString(TEXT("NULL"));
    FString currentItemImgPath;
	int32 currentCount;



public:

	//--
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	//--

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemSlotImg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragDrop")
	TSubclassOf<UdragVisual> DragVisualWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCountText;

	UPROPERTY(EditAnywhere, Category = "slot")
	bool equipmentItem = false;

	void DoubleClickEvnet();

	void nullImg();
	void slotImgSeting(const FString& ItemImg, const FString& ItemName);
	
	//인벤이 비였는지 확인용
	bool invenSlotNull();

	void SetItemCount(const int32& countItems);

	FString GetInvenInItemName();


	FTimerHandle timeHandle;

	

};
