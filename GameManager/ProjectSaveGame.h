// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProjectSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UProjectSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	//Save Game Data File Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString saveSlotName;

	//Save Game Data File Index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 saveIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	float savePlayerHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float savePlayerMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString lastMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FVector playerLastLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FRotator playerLastRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool saveBGaze14;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float saveCurrentSoulGaze;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool saveBCanPoly;

	//items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item")
	float userC;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item")
	int32 playerMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item")
	TMap<int32, FString> userGear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item")
	TMap<int32, FString> slotItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item")
	TMap<FString, int32> itemInventory;
};
