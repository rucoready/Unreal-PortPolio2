// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonClearPercentWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UserSelectorUI.h"
#include "../../GameManager/StatGameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"


void UDungeonClearPercentWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (CanvasPanel_Clear)
	{
		CanvasPanel_Clear->SetVisibility(ESlateVisibility::Collapsed);
	}
	UMaterialInterface* materialInstance = Cast<UMaterialInterface>(Image_PercentCircle->GetBrush().GetResourceObject());
	if (materialInstance)
	{
		dynamicMaterial = UMaterialInstanceDynamic::Create(materialInstance, this);
		if (dynamicMaterial)
		{
			FSlateBrush newBrush = Image_PercentCircle->GetBrush();
			newBrush.SetResourceObject(dynamicMaterial);
			Image_PercentCircle->SetBrush(newBrush);
	
			dynamicMaterial->SetScalarParameterValue(FName("Percentage"), 0.0f);
		}
	}
	if (Button_Quit)
	{
		Button_Quit->OnClicked.AddDynamic(this, &UDungeonClearPercentWidget::ClickQuitButton);
	}

	if (Button_ClearPannelRemove)
	{
		Button_ClearPannelRemove->OnClicked.AddDynamic(this, &UDungeonClearPercentWidget::ClickHideLastPannel);
	}
	
}

void UDungeonClearPercentWidget::setPercent(float percent)
{
	if (dynamicMaterial)
	{
		dynamicMaterial->SetScalarParameterValue(FName("Percentage"), percent);
	}
}

void UDungeonClearPercentWidget::setTextPercent(float percent)
{
	int32 intPercent = FMath::RoundToInt(percent * 100.0f);
	FString percentString = FString::Printf(TEXT("%d%%"), intPercent);
	TextBlock_Percent->SetText(FText::FromString(percentString));
}

void UDungeonClearPercentWidget::ClickQuitButton()
{
	if (selectorWidget)
	{
		selectorWidgetInstance = CreateWidget<UUserSelectorUI>(GetWorld(), selectorWidget);
		selectorWidgetInstance->AddToViewport(10);
	}
	
}

void UDungeonClearPercentWidget::ClickHideLastPannel()
{
	if (CanvasPanel_Clear)
	{
		CanvasPanel_Clear->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDungeonClearPercentWidget::SetClearPanelNumber(int32 clearTime, int32 killCount, int32 attackCount, int32 reviveCount)
{
	if (CanvasPanel_Clear)
	{
		CanvasPanel_Clear->SetVisibility(ESlateVisibility::Visible);
	}
	if (TextBlock_ClearTime)
	{
		int32 hour = clearTime / 3600;
		int32 minute = (clearTime % 3600) / 60;
		int32 second = clearTime % 60;
		//nn:nn:nn
		FString timeString = FString::Printf(TEXT("%02d : %02d : %02d"), hour, minute, second);
		TextBlock_ClearTime->SetText(FText::FromString(timeString));
	}
	if (TextBlock_KillCount)
	{
		TextBlock_KillCount->SetText(FText::AsNumber(killCount));
	}

	if (TextBlock_AttackCount)
	{
		TextBlock_AttackCount->SetText(FText::AsNumber(attackCount));
	}

	if (TextBlock_ReviveCount)
	{
		TextBlock_ReviveCount->SetText(FText::AsNumber(reviveCount));
	}

}

void UDungeonClearPercentWidget::playClearWidgetAnim()
{
	PlayAnimation(ClearAnim);
}
