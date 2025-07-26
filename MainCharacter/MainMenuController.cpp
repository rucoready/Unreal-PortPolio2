// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "../MainCharacter/CharacterWidget/StartMainMenuWidget.h"
#include "../MainCharacter/CharacterWidget/IntroWidget.h"
#include "MediaSoundComponent.h"
#include "EngineUtils.h"

AMainMenuController::AMainMenuController()
{

}

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	if (bUseMainMenu)
	{
		UWorld* mainWorld = GetWorld();
		for (TActorIterator<ACameraActor> it(mainWorld); it; ++it)
		{
			ACameraActor* mainCamera = *it;
			if (mainCamera->ActorHasTag(cameraActorTag))
			{
				SetViewTargetWithBlend(mainCamera, 0.f);
				break;
			}
		}
		startMainMenuWidget = CreateWidget<UStartMainMenuWidget>(this, startMainMenuWidgetClass);
		if (startMainMenuWidget)
		{
			startMainMenuWidget->AddToViewport();
			//FInputModeUIOnly InputMode;
			//SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
	else
	{
		introWidget = CreateWidget<UIntroWidget>(this, introWidgetClass);
		if (introWidget)
		{
			introWidget->AddToViewport();
			introSoundComponent = NewObject<UMediaSoundComponent>(this); 
			introSoundComponent->SetMediaPlayer(shareMediaPlayer);       
			introSoundComponent->RegisterComponent();
			bShowMouseCursor = true;
			
		}
	}
}


