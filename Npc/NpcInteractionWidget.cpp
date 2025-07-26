// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcInteractionWidget.h"
#include "Components/TextBlock.h"
#include "NpcUits.h"
#include "Components/Button.h"

void UNpcInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Exit)
	{
		Button_Exit->OnHovered.AddDynamic(this, &UNpcInteractionWidget::HoverExitButton);
		Button_Exit->OnUnhovered.AddDynamic(this, &UNpcInteractionWidget::UnHoverExitButton);
		Button_Exit->OnClicked.AddDynamic(this, &UNpcInteractionWidget::ClickExitButton);
	}
}

void UNpcInteractionWidget::SetInteractionText(FString& npcText)
{
	TextBlock_Interaction->SetText(FText::FromString(npcText));
}

void UNpcInteractionWidget::HoverExitButton()
{
	PlayAnimation(ExitHoverAnim);
}

void UNpcInteractionWidget::UnHoverExitButton()
{
	PlayAnimation(ExitUnhoverAnim);
}

void UNpcInteractionWidget::ClickExitButton()
{
	if (ownNpc)
	{
		ownNpc->EndInteractionCamera();
	}
	RemoveFromParent();
}
