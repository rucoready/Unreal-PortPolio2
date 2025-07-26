// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../../MainCharacter/SwordPlayController.h"
#include "../../MainCharacter/SwordCharacter.h"
#include "EngineUtils.h" 

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Continue)
	{
		Button_Continue->OnHovered.AddDynamic(this, &UPauseWidget::ContinueHoverButtonAnim);
		Button_Continue->OnUnhovered.AddDynamic(this, &UPauseWidget::ContinueUnHoverButtonAnim);
		Button_Continue->OnClicked.AddDynamic(this, &UPauseWidget::ContinueButtonClick);
	}
	if (Button_Exit)
	{
		Button_Exit->OnHovered.AddDynamic(this, &UPauseWidget::ExitHoverButtonAnim);
		Button_Exit->OnUnhovered.AddDynamic(this, &UPauseWidget::ExitUnHoverButtonAnim);
		Button_Exit->OnClicked.AddDynamic(this, &UPauseWidget::EixButtonClick);
	}
}

void UPauseWidget::ContinueHoverButtonAnim()
{
	PlayAnimation(HoverContinueAnim);
}

void UPauseWidget::ContinueUnHoverButtonAnim()
{
	PlayAnimation(UnhoverContinueAnim);
}

void UPauseWidget::ExitHoverButtonAnim()
{
	PlayAnimation(HoverExitAnim);
}

void UPauseWidget::ExitUnHoverButtonAnim()
{
	PlayAnimation(UnhoverExitAnim);
}

void UPauseWidget::ContinueButtonClick()
{
	APlayerController* pC = UGameplayStatics::GetPlayerController(this, 0);
	ASwordCharacter* swordCharacter = Cast<ASwordCharacter>(pC->GetPawn());
	if (swordCharacter)
	{
		swordCharacter->SetPauseWidget();
	}
	RemoveFromParent();
	
}

void UPauseWidget::EixButtonClick()
{
	for (TActorIterator<ATeleporterCristal>it(GetWorld()); it; ++it)
	{
		if (it->IsA(cristal))
		{
			cristalInstance = Cast<ATeleporterCristal>(*it);
			break;
		}
	}
	if (cristalInstance)
	{
		cristalInstance->LevelMove();
	}
}
