// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API AStartMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AStartMainMenuGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="sound")
	USoundBase* bgm;
};
