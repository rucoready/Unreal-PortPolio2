// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMainMenuWidget.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "Camera/CameraActor.h"
#include "../../MainCharacter/MainMenuController.h"
#include "Kismet/GameplayStatics.h"
#include "../../GameManager/StatGameInstance.h"
#include "FadeInOutWidget.h"
#include "EngineUtils.h"

void UStartMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CanvasPanel_SecondPannel->SetVisibility(ESlateVisibility::Collapsed);
	CanvasPanel_NoSav->SetVisibility(ESlateVisibility::Collapsed);
	if (Button_GameStart)
	{
		Button_GameStart->OnHovered.AddDynamic(this, &UStartMainMenuWidget::OnHoverGameStartButton);
		Button_GameStart->OnUnhovered.AddDynamic(this, &UStartMainMenuWidget::OnUnHoverGameStartButton);
		Button_GameStart->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OnClickGameStartButton);
	}
	if (Button_GameEnd)
	{
		Button_GameEnd->OnHovered.AddDynamic(this, &UStartMainMenuWidget::OnHoverGameEndButton);
		Button_GameEnd->OnUnhovered.AddDynamic(this, &UStartMainMenuWidget::OnUnHoverGameEndButton);
		Button_GameEnd->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OnClickGameEndButton);
	}

	//secondPannel
	if (Button_NewGame)
	{
		Button_NewGame->OnHovered.AddDynamic(this, &UStartMainMenuWidget::OnHoverNewGame);
		Button_NewGame->OnUnhovered.AddDynamic(this, &UStartMainMenuWidget::OnUnHoverNewGame);
		Button_NewGame->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OnClickNewGameButton);
	}
	if (Button_Continue)
	{
		Button_Continue->OnHovered.AddDynamic(this, &UStartMainMenuWidget::OnHoverContinue);
		Button_Continue->OnUnhovered.AddDynamic(this, &UStartMainMenuWidget::OnUnHoverContinue);
		Button_Continue->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OnClickContinueGameButton);
	}
	if (Button_GoFirstPannel)
	{
		Button_GoFirstPannel->OnHovered.AddDynamic(this, &UStartMainMenuWidget::OnHoverGoFirst);
		Button_GoFirstPannel->OnUnhovered.AddDynamic(this, &UStartMainMenuWidget::OnUnHoverGoFist);
		Button_GoFirstPannel->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OnClickGoFist);
	}

	if (Button_NoSavEnter)
	{
		Button_NoSavEnter->OnHovered.AddDynamic(this, &UStartMainMenuWidget::OnHoverNoSavEnter);
		Button_NoSavEnter->OnUnhovered.AddDynamic(this, &UStartMainMenuWidget::OnUnHoverNoSavEnter);
		Button_NoSavEnter->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OnClickNoSavEnter);
	}
	FName targetLevelName = FName(targetLoadingMap);
	FLatentActionInfo latentInfo;
	latentInfo.CallbackTarget = this;
	latentInfo.ExecutionFunction = FName("OnLevelLoaded");
	latentInfo.Linkage = 0;
	latentInfo.UUID = __LINE__;

	UGameplayStatics::LoadStreamLevel(this, targetLevelName, true, false, latentInfo);
}

void UStartMainMenuWidget::OnClickGameStartButton()
{
	FMovieSceneSequencePlaybackSettings playSetting;
	ALevelSequenceActor* sequenceActor = nullptr;
	playSetting.bAutoPlay = false;
	ULevelSequencePlayer* sequenceToGamePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), gameStartButtonClickSequence, playSetting, sequenceActor);

	CanvasPanel_Main->SetVisibility(ESlateVisibility::Collapsed);

	sequenceToGamePlayer->OnFinished.AddDynamic(this, &UStartMainMenuWidget::OnMove2PannelSequenceFinished);
	sequenceToGamePlayer->Play();
}

void UStartMainMenuWidget::OnClickGameEndButton()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	UKismetSystemLibrary::QuitGame(this, playerController, EQuitPreference::Quit, false);
}

void UStartMainMenuWidget::OnHoverGameStartButton()
{
	PlayAnimation(GameStartAnimHover);
}

void UStartMainMenuWidget::OnHoverGameEndButton()
{
	PlayAnimation(GameEndAnimHover);
}

void UStartMainMenuWidget::OnUnHoverGameStartButton()
{
	PlayAnimation(GameStartAnimUnHover);
}

void UStartMainMenuWidget::OnUnHoverGameEndButton()
{
	PlayAnimation(GameEndAnimUnHover);
}

void UStartMainMenuWidget::OnMove2PannelSequenceFinished()
{
	UWorld* mainWorld = GetWorld();
	APlayerController* playerCon = UGameplayStatics::GetPlayerController(mainWorld, 0);
	AMainMenuController* mainMenuCon = Cast<AMainMenuController>(playerCon);

	for (TActorIterator<ACameraActor> it(mainWorld); it; ++it)
	{
		ACameraActor* mainCamera = *it;
		if (mainCamera->ActorHasTag(cameraActorTag))
		{
			mainMenuCon->SetViewTargetWithBlend(mainCamera, 0.f);
			break;
		}
	}
	CanvasPanel_SecondPannel->SetVisibility(ESlateVisibility::Visible);
}

void UStartMainMenuWidget::OnClickNewGameButton()
{
	FString SlotName = TEXT("PlayerSaveData");
	int32 UserIndex = 0;

	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex);
	}

	if (!targetLoadingMap.IsEmpty())
	{
		UGameplayStatics::OpenLevel(this, FName(*targetLoadingMap));
	}
}

//이어하기
void UStartMainMenuWidget::OnClickContinueGameButton()
{
	APlayerController* pC = UGameplayStatics::GetPlayerController(this, 0);
	fadeWidgetInstance = CreateWidget<UFadeInOutWidget>(pC, fadeWidget);
	//no save data
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveData"), 0))
	{
		CanvasPanel_NoSav->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	if (fadeWidgetInstance)
	{
		fadeWidgetInstance->AddToViewport();
		FWidgetAnimationDynamicEvent endEventSW;
		endEventSW.BindDynamic(this, &UStartMainMenuWidget::openLevel);
		fadeWidgetInstance->BindToAnimationFinished(fadeWidgetInstance->FadeOut, endEventSW);
	}
	fadeWidgetInstance->PlayFadeOut();
}

void UStartMainMenuWidget::OnClickGoFist()
{
	FMovieSceneSequencePlaybackSettings playSetting2;
	ALevelSequenceActor* sequenceActor2 = nullptr;
	playSetting2.bAutoPlay = false;
	ULevelSequencePlayer* sequenceToGoFirstPannel = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), goFirstPannelSequence, playSetting2, sequenceActor2);

	CanvasPanel_SecondPannel->SetVisibility(ESlateVisibility::Collapsed);
	sequenceToGoFirstPannel->OnFinished.AddDynamic(this, &UStartMainMenuWidget::OnMoveGoFirstSequenceFinish);
	sequenceToGoFirstPannel->Play();
	
}

void UStartMainMenuWidget::OnMoveGoFirstSequenceFinish()
{
	UWorld* mainWorld = GetWorld();
	APlayerController* playerCon = UGameplayStatics::GetPlayerController(mainWorld, 0);
	AMainMenuController* mainMenuCon = Cast<AMainMenuController>(playerCon);

	for (TActorIterator<ACameraActor> it(mainWorld); it; ++it)
	{
		ACameraActor* mainCamera = *it;
		if (mainCamera->ActorHasTag(cameraActorTagSecond))
		{
			mainMenuCon->SetViewTargetWithBlend(mainCamera, 0.f);
			break;
		}
	}
	CanvasPanel_Main->SetVisibility(ESlateVisibility::Visible);
}

void UStartMainMenuWidget::openLevel()
{
	UStatGameInstance* gameInstance = Cast<UStatGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		gameInstance->LoadPlayerData();
	}
}

void UStartMainMenuWidget::OnHoverNoSavEnter()
{
	PlayAnimation(NoSaveEnterHover);
}

void UStartMainMenuWidget::OnUnHoverNoSavEnter()
{
	PlayAnimation(NoSaveEnterUnHover);
}

void UStartMainMenuWidget::OnClickNoSavEnter()
{
	CanvasPanel_NoSav->SetVisibility(ESlateVisibility::Collapsed);
}

void UStartMainMenuWidget::OnHoverNewGame()
{
	PlayAnimation(NewGameHover);
}

void UStartMainMenuWidget::OnUnHoverNewGame()
{
	PlayAnimation(NewGameUnHover);
}

void UStartMainMenuWidget::OnHoverContinue()
{
	PlayAnimation(ContinueHover);
}

void UStartMainMenuWidget::OnUnHoverContinue()
{
	PlayAnimation(ContrinueUnhover);
}

void UStartMainMenuWidget::OnHoverGoFirst()
{
	PlayAnimation(GoFirstHover);
}

void UStartMainMenuWidget::OnUnHoverGoFist()
{
	PlayAnimation(GoFirstUnHover);
}


