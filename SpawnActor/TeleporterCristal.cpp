// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleporterCristal.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacter/CharacterWidget/FadeInOutWidget.h"
#include "../GameManager/StatGameInstance.h"
#include "../MainCharacter/CharacterWidget/UserSelectorUI.h"
#include "TeleporterCristal.h"
#include "../GameManager/SwordPlayerGameBase.h"
#include "../MainCharacter/SwordPlayController.h"
#include "../MainCharacter/SwordCharacter.h"
#include "../MainCharacter/CharacterWidget/SaveWidget.h"
#include "../MainCharacter/CharacterWidget/SwordCharacterWidget.h"
#include "MediaPlayer.h"
#include "Components/BoxComponent.h"

// Sets default values
ATeleporterCristal::ATeleporterCristal()
{
	PrimaryActorTick.bCanEverTick = true;
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	RootComponent = sceneComp;
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	boxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeleporterCristal::BeginPlay()
{
	Super::BeginPlay();
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeleporterCristal::OnBeginOverlapCollision);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &ATeleporterCristal::OnEndOverlapCollision);
}

// Called every frame
void ATeleporterCristal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ATeleporterCristal::OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString hitActorName = OtherActor->GetName();
	ASwordCharacter* swordChar = Cast<ASwordCharacter>(OtherActor);
	APlayerController* pC = UGameplayStatics::GetPlayerController(this, 0);
	if (hitActorName.Contains(TEXT("SwordCharacter")))
	{
		if (pC)
		{
			if (saveWidgetInstance)
			{
				saveWidgetInstance->RemoveFromParent();
				saveWidgetInstance = nullptr;
			}
			saveWidgetInstance = CreateWidget<USaveWidget>(pC, saveWidgetClass);
			saveWidgetInstance->AddToViewport();
		
			FTimerHandle th_RemoveSaveWidget;
			USaveWidget* captureSaveWidget = saveWidgetInstance;

			if (captureSaveWidget)
			{
				UWorld* world = GetWorld();
				if (world)
				{
					TWeakObjectPtr<ATeleporterCristal> weakTeleportRemove(this);

					world->GetTimerManager().SetTimer(th_RemoveSaveWidget, [weakTeleportRemove, captureSaveWidget]()
						{
							if (weakTeleportRemove.IsValid() && IsValid(captureSaveWidget))
							{
								captureSaveWidget->RemoveFromParent();
							}
						}, 1.5f, false);
				}
			}
		
		}
		
		UStatGameInstance* gameInstance = Cast<UStatGameInstance>(GetGameInstance());
		ASwordCharacter* player = Cast<ASwordCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		ASwordPlayController* swordCon = Cast<ASwordPlayController>(pC);
		if (gameInstance)
		{
			gameInstance->playerSaveHp = player->playerCurrentHp;
			gameInstance->playerSaveMp = player->playerCurrentMp;
			gameInstance->instanceBGaze14 = player->characterWidget->bGaze14;
			gameInstance->instanceCurrentSoulGaze = player->characterWidget->currentSoulGaze;
			gameInstance->SavePlayerData(GetWorld());
		}
		
		if (usedPortal)
		{
			LevelMove();
		}
		else
		{
			if (pC)
			{
				selectorWidgetInstance = CreateWidget<UUserSelectorUI>(pC, selectorWidget);
				if (selectorWidgetInstance)
				{
					selectorWidgetInstance->AddToViewport(10);
					UGameplayStatics::PlaySoundAtLocation(this, overlapSound, GetActorLocation());
				}

			}
		}
	}
}

void ATeleporterCristal::OnEndOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FString hitActorName = OtherActor->GetName();
	if (hitActorName.Contains(TEXT("SwordCharacter")))
	{
		if (selectorWidgetInstance && selectorWidgetInstance->IsInViewport())
		{
			selectorWidgetInstance->RemoveFromParent();
			selectorWidgetInstance = nullptr;
		}

		APlayerController* pC = UGameplayStatics::GetPlayerController(this, 0);
		if (pC)
		{
			EnableInput(pC); 
		}
	}
}

void ATeleporterCristal::LevelMove()
{
	//fade out
	APlayerController* pC = UGameplayStatics::GetPlayerController(this, 0);
	fadeWidgetInstance = CreateWidget<UFadeInOutWidget>(pC, fadeWidget);
	if (fadeWidgetInstance)
	{
		fadeWidgetInstance->AddToViewport(11);
		FWidgetAnimationDynamicEvent endEvent;
		endEvent.BindDynamic(this, &ATeleporterCristal::OpenLevel);
		fadeWidgetInstance->BindToAnimationFinished(fadeWidgetInstance->FadeOut, endEvent);
	}
	fadeWidgetInstance->PlayFadeOut();
}

void ATeleporterCristal::OnMediaOpened(FString openedUrl)
{
	UStatGameInstance* gameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance)
	{
		gameInstance->katanaMediaPlayer->OnMediaOpened.RemoveDynamic(this, &ATeleporterCristal::OnMediaOpened);

		if (!targetLoadingMap.IsEmpty())
		{
			UGameplayStatics::OpenLevel(this, FName(*targetLoadingMap));
		}
	}
}


void ATeleporterCristal::OpenLevel()
{
	//미리 로딩
	UStatGameInstance* gameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance)
	{
		gameInstance->lastTargetLevel = lastTargetLevel;
	}

	//로딩맵으로 이동
	if (!targetLoadingMap.IsEmpty())
	{
		//UGameplayStatics::OpenLevel(this, FName(*targetLoadingMap));


		if (gameInstance && gameInstance->katanaMediaPlayer && gameInstance->katanaMediaSource)
		{
			// 바인딩: Media 열리면 레벨 이동
			if (!gameInstance->katanaMediaPlayer->OnMediaOpened.IsAlreadyBound(this, &ATeleporterCristal::OnMediaOpened))
			{
				gameInstance->katanaMediaPlayer->OnMediaOpened.AddDynamic(this, &ATeleporterCristal::OnMediaOpened);
			}
			gameInstance->katanaMediaPlayer->OpenSource(gameInstance->katanaMediaSource);
		}
	}
}





