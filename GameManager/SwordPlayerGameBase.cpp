// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordPlayerGameBase.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacter/CharacterWidget/SwordCharacterWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Widget.h"
#include "../MainCharacter/SwordPlayController.h"
#include "Components/InputComponent.h"
#include "../MainCharacter/CharacterWidget/FadeInOutWidget.h"
#include "StatGameInstance.h"
#include "../MainCharacter/CharacterWidget/LoadingWidget.h"
#include "../MainCharacter/CharacterWidget/MapNameUI.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "StatGameInstance.h"
#include "ProjectSaveGame.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "../MainCharacter/MainWidget.h"
#include "Components/CanvasPanel.h"
#include "EngineUtils.h"
#include "MediaTexture.h"
#include "MediaPlayer.h"
#include "../MainCharacter/CharacterWidget/DungeonClearPercentWidget.h"
#include "Components/AudioComponent.h"
#include "../MainCharacter/SwordCharacter.h"



ASwordPlayerGameBase::ASwordPlayerGameBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASwordPlayerGameBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (characterWidget)
	{
		player->updateUI();
	}
	if (myInstance && percentWidget)
	{
		float killCount = myInstance->skeletonDieSumCount;
		float nowPercent = killCount / 123.0f;
		nowPercent = FMath::Clamp(nowPercent, 0.0f, 1.0f);
		percentWidget->setPercent(nowPercent);
		percentWidget->setTextPercent(nowPercent);
	}
	if (timerOn)
	{
		myInstance->AddClearTime(DeltaTime);	
	}
	if (bBGMPlaying && currentBGM && currentBGM->IsPlaying())
	{
		saveBGMPlayTime += DeltaTime;
	}
}

void ASwordPlayerGameBase::BeginPlay()
{
	Super::BeginPlay();
	pCon = UGameplayStatics::GetPlayerController(this, 0);
	if (pCon)
	{
		player = Cast<ASwordCharacter>(pCon->GetPawn());
	}
	SetPlayerCameraArmLength();
	
	myInstance = Cast<UStatGameInstance>(GetGameInstance());
	//percentUI
	percentWidget = Cast<UDungeonClearPercentWidget>(percentWidgetAsset);
	

	//fade 
	if (pCon && bNeedFadeIn)
	{
		fadeWidgetInstance = CreateWidget<UFadeInOutWidget>(pCon, fadeWidget);
		if (fadeWidgetInstance)
		{
			fadeWidgetInstance->AddToViewport(15);

			if (fadeWidgetInstance->FadeIn)
			{
				FWidgetAnimationDynamicEvent endEvent;
				endEvent.BindDynamic(this, &ASwordPlayerGameBase::RemoveFadeOutWidget);
				fadeWidgetInstance->BindToAnimationFinished(fadeWidgetInstance->FadeIn, endEvent);

			}
		}
		fadeWidgetInstance->PlayFadeIn();
	}
	

	//loding -- 1

	if (bUseLoadingMap)
	{
		if (pCon && loadingWidget)
		{
			loadingWidgetInstance = CreateWidget<ULoadingWidget>(pCon, loadingWidget);
			if (loadingWidgetInstance)
			{
				loadingWidgetInstance->AddToViewport(25);
				loadingWidgetInstance->LoadingStart();
				loadingWidgetInstance->StartDotAnim();
			}
		}
	}

	if (UseMonsterWave)
	{
		FTimerHandle th_DelaySpawn;
		GetWorld()->GetTimerManager().SetTimer(th_DelaySpawn, [this]()
		{
			if (stage1)
			{
				currentTemplates = { skeleton1 };
				SpawnWave(currentTemplates, 9);
			}
			else if (stage2)
			{
				currentTemplates = { skeleton4 };
				SpawnWave(currentTemplates, 9);
			}
			else if (stage3)
			{
				currentTemplates = { skeleton4, skeleton1 };
				SpawnWave(currentTemplates, 7);
			}
		}, 5.0f, false);
	}

	//delegateBinding
	onSkeletonDieCountChanged.AddDynamic(this, &ASwordPlayerGameBase::waveGeneration);

	//Load
	LoadPlayerData();
	//modeSetting
	if (bCharacterAttackMode)
	{
		if (player)
		{
			player->bHasAttackMode;
		}
	}
	//sequence
	//if use dont check bNeedFadeIn
	if (bUseCharactersequence)
	{
		fadeWidgetInstance = CreateWidget<UFadeInOutWidget>(pCon, fadeWidget);
		if (fadeWidgetInstance)
		{
			fadeWidgetInstance->AddToViewport();
			player->equipmentUI->SetVisibility(ESlateVisibility::Collapsed);
			PlayCharacterSwordSequence();
			if (fadeWidgetInstance->FadeIn)
			{
				FWidgetAnimationDynamicEvent endEventD1;
				endEventD1.BindDynamic(this, &ASwordPlayerGameBase::OnFadeInFinishedCharacterUISetting);
				fadeWidgetInstance->BindToAnimationFinished(fadeWidgetInstance->FadeIn, endEventD1);


			}
		}
		fadeWidgetInstance->PlayFadeIn();
	}
	
	if (bUseEquipment)
	{
		FTimerHandle th_setEquipment;
		GetWorld()->GetTimerManager().SetTimer(th_setEquipment, [this]()
			{
				if (player)
				{
					player->equipmentUI->SetVisibility(ESlateVisibility::Visible);
				}
			},0.1f, false);
		
			
	}
	if (bUsePercentWidget)
	{
		ASwordPlayController* swordController = Cast<ASwordPlayController>(GetWorld()->GetFirstPlayerController());
		FTimerHandle th_userPercentWidget;
		GetWorld()->GetTimerManager().SetTimer(th_userPercentWidget, [this]()
		{
			SetPercentWidget();
			
		}, 0.1f, false);
		//int32 clearTimeInt = FMath::FloorToInt(myInstance->totalClearTime);
		
	}

	ASwordPlayController* swordController = Cast<ASwordPlayController>(GetWorld()->GetFirstPlayerController());
	switch (bgmIndex)
	{
	case EBGMIndex::Town:
		if (townBGM) PlayBGM(townBGM);
		break;
	case EBGMIndex::Library:
		if (libraryBGM) PlayBGM(libraryBGM);
		break;
	case EBGMIndex::Stage1:
		if (stage1BGM) PlayBGM(stage1BGM);
		break;
	case EBGMIndex::Stage2:
		swordController->InputTab();
		if (player)
		{
			player->playerCurrentForce = player->playerMaxForce;
		}
		if (stage2BGM) PlayBGM(stage2BGM);
		break;
	case EBGMIndex::Stage3:
		swordController->InputTab();
		if (player)
		{
			player->playerCurrentForce = player->playerMaxForce;
		}
		if (stage3BGM) PlayBGM(stage3BGM);
		break;
	default:
		break;
	}
	
}

void ASwordPlayerGameBase::PlayCharacterSwordSequence()
{
	ASwordPlayController* swordController = Cast<ASwordPlayController>(GetWorld()->GetFirstPlayerController());
	TWeakObjectPtr<ASwordCharacter> capturePlayer = Cast<ASwordCharacter>(pCon->GetPawn());
	FTimerHandle th_PlayerAttackMode;
	FTimerHandle th_PlayerPlaySwitchSwordAnim;
	FTimerHandle th_PlayerSwordHidden;
	FTimerHandle th_stopInputComponent;
	player->GetMesh()->SetVisibility(false);
	player->overlapSword->SetVisibility(false);
	//stopPlayer
	if (InputComponent && InputComponent->IsActive())
	{
		InputComponent->Deactivate();
	}
	GetWorld()->GetTimerManager().SetTimer(th_stopInputComponent, [swordController]()
		{
			if (swordController && swordController->InputComponent)
			{
				swordController->DisableInput(swordController);
			}
		}, 0.01f, false);
	GetWorld()->GetTimerManager().SetTimer(th_PlayerAttackMode, [this, swordController]()
		{
			if (player && player->characterWidget)
			{
				player->characterWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}, 0.01f, false);
	GetWorld()->GetTimerManager().SetTimer(th_PlayerSwordHidden, [capturePlayer]() {capturePlayer->overlapSword->SetVisibility(false);
		}, 0.1f, false);
	FMovieSceneSequencePlaybackSettings playSetting;
	ALevelSequenceActor* sequenceActor = nullptr;
	playSetting.bAutoPlay = false;
	ULevelSequencePlayer* swordSequensePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), characterSwordSequence, playSetting, sequenceActor);
	swordSequensePlayer->Play();
	//can move player call delegate
	swordSequensePlayer->OnFinished.AddDynamic(this, &ASwordPlayerGameBase::SwordSequenceFinish);
}

void ASwordPlayerGameBase::SpawnMasterField()
{
	FVector masterFieldSpawnLoc(1540.0f, 630.0f, 160.0f);
	FRotator masterFieldSpawnRot = FRotator::ZeroRotator;
	FActorSpawnParameters mfSpawnParams;
	mfSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UWorld* dungeonWorld = GetWorld();
	AActor* SpawnedMasterField = dungeonWorld->SpawnActor<AActor>(masterFieldActor, masterFieldSpawnLoc, masterFieldSpawnRot, mfSpawnParams);
}

void ASwordPlayerGameBase::PlayWallSequence()
{
	if (player->characterWidget)
	{
		player->characterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	player->equipmentUI->SetVisibility(ESlateVisibility::Collapsed);
	percentWidget->SetVisibility(ESlateVisibility::Collapsed);

	FMovieSceneSequencePlaybackSettings playSetting;
	ALevelSequenceActor* sequenceActor = nullptr;
	playSetting.bAutoPlay = false;
	ULevelSequencePlayer* swordSequenseWall = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), wallBrokeSequence, playSetting, sequenceActor);
	swordSequenseWall->Play();
	//can move player call delegate
	swordSequenseWall->OnFinished.AddDynamic(this, &ASwordPlayerGameBase::wallSequenceFinish);

	//stopPlayer
	if (InputComponent && InputComponent->IsActive())
	{
		InputComponent->Deactivate();
	}
}









void ASwordPlayerGameBase::SetPlayerCameraArmLength()
{
	if (player)
	{
		player->originArmLength = playerCameraArmLength;
	}
	
}

void ASwordPlayerGameBase::SetPlayerShowUI()
{
	if (!player || !player->characterWidget)
	{
		return;
	}
	if (bShowPlayerSkillUI)
	{
		player->characterWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		player->characterWidget->CanvasPanel_Skill->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void ASwordPlayerGameBase::OnPlayPlayerWidgetReady()
{
	SetPlayerShowUI();
}

void ASwordPlayerGameBase::RemoveFadeOutWidget()
{
	if (fadeWidgetInstance)
	{
		fadeWidgetInstance->RemoveFromParent();
		fadeWidgetInstance = nullptr;
	}
	OnFadeInFinished();
}

void ASwordPlayerGameBase::HideAllPlayerUI()
{
	
	if (pCon && bUnActivatePlayerWidget)
	{
		player->characterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASwordPlayerGameBase::OnLevelLoaded()
{
	
	UStatGameInstance* gameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance)
	{
		UGameplayStatics::OpenLevel(this, gameInstance->targetLevelToLoad);
	}
}

void ASwordPlayerGameBase::OnFadeInFinished()
{
	mapNampeWidgetInstance = CreateWidget<UMapNameUI>(pCon, mapNameWidget);
	if (mapNampeWidgetInstance) 
	{
		mapNampeWidgetInstance->mapName = UIMapName;
		mapNampeWidgetInstance->AddToViewport();
		mapNampeWidgetInstance->playMapNameWidget();
		
		if (mapNampeWidgetInstance->MapNameAnim)
		{
			FWidgetAnimationDynamicEvent mapEndEvent;
			mapEndEvent.BindDynamic(this, &ASwordPlayerGameBase::RemoveMapNameWidget);
			mapNampeWidgetInstance->BindToAnimationFinished(mapNampeWidgetInstance->MapNameAnim, mapEndEvent);
		}
	}
	
}

void ASwordPlayerGameBase::OnFadeInFinishedCharacterUISetting()
{
	mapNampeWidgetInstance = CreateWidget<UMapNameUI>(pCon, mapNameWidget);
	if (mapNampeWidgetInstance)
	{
		mapNampeWidgetInstance->mapName = UIMapName;
		mapNampeWidgetInstance->AddToViewport();
		mapNampeWidgetInstance->playMapNameWidget();
		if (mapNampeWidgetInstance->MapNameAnim)
		{
			FWidgetAnimationDynamicEvent mapEndEventCV;
			mapEndEventCV.BindDynamic(this, &ASwordPlayerGameBase::RemoveMapNameWidgetAndShowWidget);
			mapNampeWidgetInstance->BindToAnimationFinished(mapNampeWidgetInstance->MapNameAnim, mapEndEventCV);
		}
	}
}

void ASwordPlayerGameBase::RemoveMapNameWidget()
{
	
	if (mapNampeWidgetInstance)
	{
		mapNampeWidgetInstance->RemoveFromParent();
		mapNampeWidgetInstance = nullptr;
	}
}

void ASwordPlayerGameBase::RemoveMapNameWidgetAndShowWidget()
{
	player->equipmentUI->SetVisibility(ESlateVisibility::Visible);
	if (mapNampeWidgetInstance)
	{
		mapNampeWidgetInstance->RemoveFromParent();
		mapNampeWidgetInstance = nullptr;
	}
}

void ASwordPlayerGameBase::waveGeneration(int32 newCount)
{
	if (stage1)
	{
		if (newCount == wave2NeedCount)
		{
			currentTemplates = { skeleton1, skeleton2 };
			SpawnWave(currentTemplates, wave2SpawnCount);
		}
		else if (newCount == wave3NeedCount)
		{
			currentTemplates = { skeleton3 };
			SpawnWave(currentTemplates, wave3SpawnCount);
		}
		else if (newCount == wave4NeedCount)
		{
			currentTemplates = { skeleton2, skeleton3 };
			SpawnWave(currentTemplates, wave4SpawnCount);
		}
		else if (newCount == 32)
		{
			SpawnGate();
		}
	}
	if (stage2)
	{
		if (newCount == wave2NeedCount)
		{
			currentTemplates = { skeleton4, skeleton5, skeleton1, skeleton2};
			SpawnWave(currentTemplates, wave2SpawnCount);
		}
		else if (newCount == wave3NeedCount)
		{
			currentTemplates = { skeleton6 };
			SpawnWave(currentTemplates, wave3SpawnCount);
		}
		else if (newCount == wave4NeedCount)
		{
			currentTemplates = { skeleton5, skeleton6,skeleton1, skeleton2 };
			SpawnWave(currentTemplates, wave4SpawnCount);
		}
		else if (newCount == wave5NeedCount)
		{
			currentTemplates = { skeleton4, skeleton6,skeleton1 };
			SpawnWave(currentTemplates, wave5SpawnCount);
		}
		else if (newCount == 45)
		{
			SpawnGate();
		}
	}
	if (stage3)
	{
		if (newCount == 5)
		{
			currentTemplates = { skeleton4, skeleton5, skeleton1, skeleton2 };
			SpawnWave(currentTemplates, 18);
		}
		else if (newCount == 16)
		{
			currentTemplates = { skeleton6 };
			SpawnWave(currentTemplates, 4);
		}
		else if (newCount == 24)
		{
			currentTemplates = { skeleton1, skeleton2, skeleton4, skeleton5, skeleton6 };
			SpawnWave(currentTemplates, 7);
		}
		else if (newCount == 36)
		{
			SpawnMasterField();
			PlayWallSequence();
			currentTemplates = { strongSkeleton1, strongSkeleton2, strongSkeleton3 };
			SpawnLastWave(currentTemplates, 10);
		}
		else if(newCount == 46)
		{
			
			int32 clearTimeInt = FMath::FloorToInt(myInstance->totalClearTime);
			int32 SumkillCount = myInstance->skeletonDieSumCount;
			int32 hitCount = myInstance->skeletonHitCounting;
			int32 reviveCount = myInstance->reviveCount;
			percentWidget->SetClearPanelNumber(clearTimeInt, SumkillCount, hitCount, reviveCount);
			percentWidget->playClearWidgetAnim();
			SpawnGate();
		}
	}
	
}

void ASwordPlayerGameBase::increaseSkeletonDieCount()
{
	skeletonDieCount++;
	onSkeletonDieCountChanged.Broadcast(skeletonDieCount);
}

void ASwordPlayerGameBase::SpawnWave(TArray<TSubclassOf<ABasicSkeletonEnemy>>& availableTemplates, int32 countToSpawn)
{
	float spawnArea = 600.f;
	UWorld* dungeonWorld = GetWorld();
	int32 spawnedCount = 0;

	while (spawnedCount < countToSpawn)
	{
		FVector spawnLoc;
		float distanceToPlayer = 0.f;
		int32 maxTry = 100;
		int32 tryCount = 0;

		while (tryCount++ < maxTry)
		{
			float randAngle = FMath::FRandRange(0.0f, 2 * PI);
			float randDistance = FMath::FRandRange(0.0f, spawnArea);
			FVector offset = FVector(FMath::Cos(randAngle), FMath::Sin(randAngle), 0.f) * randDistance;
			spawnLoc = centerLoc + offset;

			distanceToPlayer = FVector::Dist(spawnLoc, player->GetActorLocation());
			if (distanceToPlayer >= 300.f)
			{
				break;
			}
		}

		if (tryCount >= maxTry)
		{
			continue;
		}

		int32 randomChoiceIndex = FMath::RandRange(0, availableTemplates.Num() - 1);
		TSubclassOf<ABasicSkeletonEnemy> chosenTemplate = availableTemplates[randomChoiceIndex];

		FRotator spawnRot(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.f);

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABasicSkeletonEnemy* SpawnedEnemy = dungeonWorld->SpawnActor<ABasicSkeletonEnemy>(
			chosenTemplate, spawnLoc, spawnRot, spawnParams
		);

		if (SpawnedEnemy)
		{
			++spawnedCount;
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("Finished spawning wave with count: %d"), spawnedCount);
}

void ASwordPlayerGameBase::SpawnLastWave(TArray<TSubclassOf<ABasicSkeletonEnemy>>& availableTemplates, int32 countToSpawn)
{
	
	float spawnArea = 600.f;
	UWorld* dungeonWorld = GetWorld();
	int32 spawnedCount = 0;
	if (!player)
	{
		return;
	}

	while (spawnedCount < countToSpawn)
	{
		FVector spawnLoc;
		float distanceToPlayer = 0.f;
		int32 maxTry = 100;
		int32 tryCount = 0;

		while (tryCount++ < maxTry)
		{
			float randAngle = FMath::FRandRange(0.0f, 2 * PI);
			float randDistance = FMath::FRandRange(0.0f, spawnArea);
			FVector offset = FVector(FMath::Cos(randAngle), FMath::Sin(randAngle), 0.f) * randDistance;
			spawnLoc = LastSpawnLoc + offset;

			distanceToPlayer = FVector::Dist(spawnLoc, player->GetActorLocation());
			if (distanceToPlayer >= 300.f)
			{
				break;
			}
		}

		if (tryCount >= maxTry)
		{
			continue;
		}

		int32 randomChoiceIndex = FMath::RandRange(0, availableTemplates.Num() - 1);
		TSubclassOf<ABasicSkeletonEnemy> chosenTemplate = availableTemplates[randomChoiceIndex];

		FRotator spawnRot(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.f);

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABasicSkeletonEnemy* SpawnedEnemy = dungeonWorld->SpawnActor<ABasicSkeletonEnemy>(
			chosenTemplate, spawnLoc, spawnRot, spawnParams
		);

		if (SpawnedEnemy)
		{
			++spawnedCount;
		}
	}
}

void ASwordPlayerGameBase::SpawnGate()
{
	FRotator spawnRot = FRotator::ZeroRotator;
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(gate, centerLoc, spawnRot, spawnParams);
}

void ASwordPlayerGameBase::SavePlayerData()
{
	UProjectSaveGame* saveGameInstance = Cast<UProjectSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectSaveGame::StaticClass()));
	UStatGameInstance* gameInstance = Cast<UStatGameInstance>(GetGameInstance());
	if (saveGameInstance)
	{
		//save data
		saveGameInstance->saveSlotName = "PlayerSaveData";
		saveGameInstance->saveIndex = 0;

		saveGameInstance->savePlayerHp = gameInstance->playerSaveHp;
		saveGameInstance->savePlayerMp = gameInstance->playerSaveMp;
		UGameplayStatics::SaveGameToSlot(saveGameInstance, saveGameInstance->saveSlotName, saveGameInstance->saveIndex);
	}
}

void ASwordPlayerGameBase::LoadPlayerData()
{
	UProjectSaveGame* loadGameInstance = Cast<UProjectSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectSaveGame::StaticClass()));
	UStatGameInstance* gameInstance = Cast<UStatGameInstance>(GetGameInstance());
	if (loadGameInstance && gameInstance)
	{
		//load data
		loadGameInstance->saveSlotName = "PlayerSaveData";
		loadGameInstance->saveIndex = 0;

		loadGameInstance = Cast<UProjectSaveGame>(UGameplayStatics::LoadGameFromSlot(loadGameInstance->saveSlotName, loadGameInstance->saveIndex));
		if (loadGameInstance)
		{
			gameInstance->playerSaveHp = loadGameInstance->savePlayerHp;
			gameInstance->playerSaveMp = loadGameInstance->savePlayerMp;
		}
	}
}

void ASwordPlayerGameBase::SwordSequenceFinish()
{
	player->characterWidget->SetVisibility(ESlateVisibility::Visible);
	player->equipmentUI->SetVisibility(ESlateVisibility::Visible);
	player->GetMesh()->SetVisibility(true);
	player->overlapSword->SetVisibility(true);
	ASwordPlayController* swordController = Cast<ASwordPlayController>(GetWorld()->GetFirstPlayerController());
	swordController->SetViewTargetWithBlend(player, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic);
	USwordCharacterWidget* playerMainUI = Cast<USwordCharacterWidget>(swordController->swordCharacterWidget);
	swordController->EnableInput(swordController);
	swordController->InputTab();
	if (swordController)
	{
		player->InGameModeUserWidgetSet = true;
	}
	SetPercentWidget();

}

void ASwordPlayerGameBase::wallSequenceFinish()
{
	player->equipmentUI->SetVisibility(ESlateVisibility::Visible);
	percentWidget->SetVisibility(ESlateVisibility::Visible);
	if (player)
	{
		if (player->characterWidget)
		{
			player->characterWidget->SetVisibility(ESlateVisibility::Visible);
		}

		if (player->equipmentUI)
		{
			player->equipmentUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
void ASwordPlayerGameBase::SetPercentWidget()
{
	APlayerController* swordController = UGameplayStatics::GetPlayerController(this, 0);
	if (!swordController)
	{
		return;
	}

	if (!percentWidgetAsset)
	{
		return;
	}

	percentWidget = CreateWidget<UDungeonClearPercentWidget>(swordController, percentWidgetAsset);
	if (percentWidget == nullptr)
	{
		return;
	}
	percentWidget->AddToViewport(9);
}

void ASwordPlayerGameBase::InsertNumberClearWidget()
{

	//percentWidget->SetClearPanelNumber()
}

void ASwordPlayerGameBase::PlayBGM(USoundBase* bgm)
{
	if (!IsValid(currentBGM) || !currentBGM->IsRegistered())
	{
		currentBGM = NewObject<UAudioComponent>(this, UAudioComponent::StaticClass(), TEXT("BGM_AudioComponent"));
		if (!currentBGM) return;

		currentBGM->bAutoActivate = false;
		currentBGM->bIsUISound = false;
		currentBGM->bAllowSpatialization = false;
		currentBGM->bOverridePriority = true;
		currentBGM->Priority = 100.0f;

		currentBGM->RegisterComponentWithWorld(GetWorld());
		currentBGM->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (currentBGM->IsPlaying())
	{
		currentBGM->Stop();
	}

	currentBGM->SetSound(bgm);
	float fadeBGMInDuration = 2.0f;
	float bgmVolume = 1.0f;
	float bgmStartTime = 0.0f;
	currentBGM->FadeIn(fadeBGMInDuration, bgmVolume, bgmStartTime);

	//save할떄 쓰던 상태값들
	saveBGMPlayTime = 0.f;
	bBGMPlaying = true;
}

void ASwordPlayerGameBase::SetBGMMute(bool bMute)
{
	if (currentBGM)
	{
		if (bMute)
		{
			currentBGM->Stop();
			bBGMPlaying = false;
		}
		else
		{
			currentBGM = UGameplayStatics::SpawnSound2D(this, currentBGM->Sound, 1.0f, 1.0f, saveBGMPlayTime);
			bBGMPlaying = true;
		}
	}
}








