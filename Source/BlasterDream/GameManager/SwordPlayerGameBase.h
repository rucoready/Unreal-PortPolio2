// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../MainCharacter/SwordCharacter.h"
#include "../MainCharacter/CharacterWidget/LoadingWidget.h"
#include "../MainCharacter/CharacterWidget/MapNameUI.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "../MainCharacter/CharacterWidget/SwordCharacterWidget.h"
#include "../MainCharacter/CharacterWidget/DungeonClearPercentWidget.h"
#include "SwordPlayerGameBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkeletonDieCountChanged, int32, newCount);
/**
 * 
 */

 //enum
UENUM(BlueprintType)
enum class EBGMIndex : uint8
{
	None    UMETA(DisplayName = "None"),
	Town    UMETA(DisplayName = "Town BGM"),
	Library UMETA(DisplayName = "Library BGM"),
	Stage1  UMETA(DisplayName = "Stage 1 BGM"),
	Stage2  UMETA(DisplayName = "Stage 2 BGM"),
	Stage3  UMETA(DisplayName = "Stage 3 BGM")
};

UCLASS()
class BLASTERDREAM_API ASwordPlayerGameBase : public AGameModeBase
{
	GENERATED_BODY()
	ASwordPlayerGameBase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	
	class ASwordCharacter* player;
	class APlayerController* pCon;
	// setting camera arm
	void SetPlayerCameraArmLength();
	// Setting PlayerUI
	void SetPlayerShowUI();

	

	UFUNCTION()
	void OnPlayPlayerWidgetReady();

	UPROPERTY(EditAnywhere, Category="MySettings")
	float playerCameraArmLength;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bShowPlayerSkillUI;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bBlockSkillKey;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bBlockAllKeyUIMode;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bNeedFadeIn;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bNeedFirstCharacterFadeIn;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bUseLoadingMap = false;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bUnActivatePlayerWidget = false;

	//fade
	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UFadeInOutWidget> fadeWidget;

	UFadeInOutWidget* fadeWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FString targetLevelPathLast;

	UFUNCTION()
	void RemoveFadeOutWidget();

	UFUNCTION()
	void HideAllPlayerUI();

	UFUNCTION()
	void OnLevelLoaded();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UUserWidget> loadingWidget;

	ULoadingWidget* loadingWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UMapNameUI> mapNameWidget;

	UMapNameUI* mapNampeWidgetInstance;

	UFUNCTION()
	void OnFadeInFinished();

	UFUNCTION()
	void OnFadeInFinishedCharacterUISetting();
	
	UFUNCTION()
	void RemoveMapNameWidget();

	UFUNCTION()
	void RemoveMapNameWidgetAndShowWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString UIMapName;

	//WAVE
	int32 skeletonDieCount;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave2NeedCount = 7;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave3NeedCount = 15;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave4NeedCount = 20;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave5NeedCount = 28;

	UPROPERTY()
	FOnSkeletonDieCountChanged onSkeletonDieCountChanged;

	UFUNCTION()
	void waveGeneration(int32 newCount);

	UFUNCTION()
	void increaseSkeletonDieCount();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool UseMonsterWave = false;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> skeleton1;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> skeleton2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> skeleton3;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> skeleton4;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> skeleton5;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> skeleton6;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> strongSkeleton1;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> strongSkeleton2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABasicSkeletonEnemy> strongSkeleton3;


	UFUNCTION()
	void SpawnWave(TArray<TSubclassOf<ABasicSkeletonEnemy>>& availableTemplates, int32 countToSpawn);

	UFUNCTION()
	void SpawnLastWave(TArray<TSubclassOf<ABasicSkeletonEnemy>>& availableTemplates, int32 countToSpawn);

	//gate-----
	UFUNCTION()
	void SpawnGate();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor> gate;
	//----------------------------
	UPROPERTY()
	TArray<TSubclassOf<class ABasicSkeletonEnemy>> currentTemplates;

	int32 spawnCount;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave1SpawnCount = 10;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave2SpawnCount = 7;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave3SpawnCount = 5;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave4SpawnCount = 11;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 wave5SpawnCount = 13;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool stage1;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool stage2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool stage3;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FVector centerLoc;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FVector LastSpawnLoc;

	//instance
	UStatGameInstance* myInstance;

	int32 savedCount;
	int sumCount;

	//-----------------------------------------------------
	//Save And Load

	UFUNCTION()
	void SavePlayerData();

	UFUNCTION()
	void LoadPlayerData();



	//sequence
	// character
	UPROPERTY(EditAnywhere, Category = "Sequence")
	bool bCharacterAttackMode;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	bool bUseCharactersequence;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	class ULevelSequence* characterSwordSequence;

	UFUNCTION()
	void SwordSequenceFinish();

	UFUNCTION()
	void wallSequenceFinish();

	UFUNCTION()
	void PlayCharacterSwordSequence();

	UPROPERTY(EditAnywhere, Category = "SequenceAfter")
	TSubclassOf<USwordCharacterWidget> characterWidgetAsset;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	class USwordCharacterWidget* characterWidget;

	//ai
	UPROPERTY(EditAnywhere, Category = "Eqipment")
	bool bUseEquipment;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	TSubclassOf<AActor> masterFieldActor;

	UFUNCTION()
	void SpawnMasterField();

	UFUNCTION()
	void PlayWallSequence();

	UPROPERTY(EditAnywhere, Category = "Sequence")
	class ULevelSequence* wallBrokeSequence;

	//percentChecker
	UPROPERTY(EditAnywhere, Category = "Percent")
	bool bUsePercentWidget;

	UPROPERTY(EditAnywhere, Category = "Percent")
	TSubclassOf<UDungeonClearPercentWidget> percentWidgetAsset;

	class UDungeonClearPercentWidget* percentWidget;

	UFUNCTION()
	void SetPercentWidget();

	UFUNCTION()
	void InsertNumberClearWidget();

	UPROPERTY(EditAnywhere, Category = "Percent")
	bool timerOn;

	UPROPERTY(EditAnywhere, Category = "Percent")
	bool bHidePotionWidget;

	//sound
	UPROPERTY(EditAnywhere, Category = "sound")
	USoundBase* townBGM;

	UPROPERTY(EditAnywhere, Category = "sound")
	USoundBase* libraryBGM;

	UPROPERTY(EditAnywhere, Category = "sound")
	USoundBase* stage1BGM;

	UPROPERTY(EditAnywhere, Category = "sound")
	USoundBase* stage2BGM;

	UPROPERTY(EditAnywhere, Category = "sound")
	USoundBase* stage3BGM;

	UPROPERTY(EditAnywhere, Category = "Sound")
	EBGMIndex bgmIndex = EBGMIndex::None;  

	//bgm repactoring code
	UPROPERTY()
	UAudioComponent* currentBGM;

	UFUNCTION()
	void PlayBGM(USoundBase* bgm);

	UFUNCTION()
	void SetBGMMute(bool bMute);

	float saveBGMPlayTime;
	bool bBGMPlaying = false;

	//loading
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UMediaPlayer* katanaLoadingPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UMediaSource* katanaMediaSource;



};
