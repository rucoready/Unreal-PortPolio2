// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSelectorUI.h"
#include "Components/Button.h"
#include "../../GameManager/SwordPlayerGameBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "../../SpawnActor/TeleporterCristal.h"
#include "EngineUtils.h" 


void UUserSelectorUI::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_No)
    {
        Button_No->OnClicked.AddDynamic(this, &UUserSelectorUI::NoButtonClick);
    }
    if (Button_Yes)
    {
        Button_Yes->OnClicked.AddDynamic(this, &UUserSelectorUI::YesButtonClick);
    }

    //hovered
    if (Button_No)
    {
        Button_No->OnHovered.AddDynamic(this, &UUserSelectorUI::NoButtonHover);
    }
    if (Button_Yes)
    {
        Button_Yes->OnHovered.AddDynamic(this, &UUserSelectorUI::YesButtonHover);
    }

    //unhovered
    if (Button_No)
    {
        Button_No->OnUnhovered.AddDynamic(this, &UUserSelectorUI::NoButtonUnHover);
    }
    if (Button_Yes)
    {
        Button_Yes->OnUnhovered.AddDynamic(this, &UUserSelectorUI::YesButtonUnHover);
    }
    gameMode = Cast<ASwordPlayerGameBase>(UGameplayStatics::GetGameMode(this));
}

void UUserSelectorUI::YesButtonClick()
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

void UUserSelectorUI::NoButtonClick()
{
    RemoveFromParent();
    APlayerController* pC = UGameplayStatics::GetPlayerController(this, 0);
    if (pC)
    {
        pC->EnableInput(pC);
    }
}

void UUserSelectorUI::YesButtonHover()
{
    PlayAnimation(ButtonYesHoverAnim);
}

void UUserSelectorUI::NoButtonHover()
{
    PlayAnimation(ButtonNoHoverAnim);
}

void UUserSelectorUI::YesButtonUnHover()
{
    PlayAnimation(ButtonYesUnHoverAnim);
}

void UUserSelectorUI::NoButtonUnHover()
{
    PlayAnimation(ButtonNoUnHoverAnim);
}
