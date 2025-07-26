// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../MainCharacter/CharacterWidget/StartMainMenuWidget.h"
#include "../MainCharacter/CharacterWidget/IntroWidget.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainMenuController();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bUseMainMenu;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FName cameraActorTag;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<UStartMainMenuWidget> startMainMenuWidgetClass;

	UStartMainMenuWidget* startMainMenuWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<UIntroWidget> introWidgetClass;

	UIntroWidget* introWidget;

	//widget sound
	UPROPERTY(EditAnywhere, Category = "MySettings")
	UMediaPlayer* shareMediaPlayer;

	UPROPERTY()
	UMediaSoundComponent* introSoundComponent;

	

};
