// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Npc/shopItemData.h"
#include "../Npc/tradeUi.h"
#include "../Npc/userInvenSlot.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "../Npc/ItemWidget.h"
#include "MainWidget.generated.h"


UCLASS()

class BLASTERDREAM_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category="Inven")
    bool inven_visible = true;

    bool UserInstal_visible = true;

    bool Shop_visible = false;


public:
    virtual void NativeConstruct() override;

    void OpenShop();
    void closeShop();

    void visbleInven();

    void visbleUserInstal();

    void visblegKey(const bool& onKeyUi);

    UPROPERTY(meta = (BindWidget))
    UItemWidget* BP_ItemWidget;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* Trade_ScrollBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trade")
    TSubclassOf<UtradeUi> tradeUiClass;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* trade_Canvas;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* inven_Canvas;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* userInstal;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* MainCanasPanel;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* UserMoney;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* gKey_Canves;


    //인벤토리 
    UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidgetOptional))
    TArray<UuserInvenSlot*> InvenSlots;

    UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (BindWidgetOptional))
    TArray<UuserInvenSlot*> InstalSlots;

    UPROPERTY()
    TMap<int32, FString> SlotItes;

    UPROPERTY()
    TMap<int32, FString> InstalItems;


    UFUNCTION(BlueprintCallable)
    void showShopBox(const FString& ItemName, const int32& ImagePrice, const FString& itemImage);

    
    UFUNCTION(BlueprintCallable)
    void InvenIn(const FString& ItemName, const FString& ImageFind);

    UFUNCTION(BlueprintCallable)
    void InvenInCount(const FString& ItemName, const int32& ItemCount);


    UFUNCTION(BlueprintCallable)

    void InvenRemove(const FString& ItemName, const int32& itemCount);

    
     TMap<int32 ,FString> SlotSave();

     TMap<int32, FString> InstalSlotSave();

     UFUNCTION(BlueprintCallable)
     void changeMoney(int32 getMoney);

     bool bIsInitialized = false;

     int32 money;

     UFUNCTION()
     void SetInventoryUI();
};
