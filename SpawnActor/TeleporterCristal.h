// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../MainCharacter/CharacterWidget/FadeInOutWidget.h"
#include "../GameManager/SwordPlayerGameBase.h"
#include "../MainCharacter/CharacterWidget/SaveWidget.h"
#include "TeleporterCristal.generated.h"

UCLASS()
class BLASTERDREAM_API ATeleporterCristal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporterCristal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxCollision;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;

	UFUNCTION()
	void OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString targetLoadingMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString TestString;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UFadeInOutWidget> fadeWidget;

	UFadeInOutWidget* fadeWidgetInstance;

	UFUNCTION()
	void OpenLevel();



	//selector
	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UUserSelectorUI> selectorWidget;

	UUserSelectorUI* selectorWidgetInstance;

	UFUNCTION()
	void LevelMove();


	//UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<UUserWidget> setLodingWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool usedPortal;

	//Save
	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<USaveWidget> saveWidgetClass;

	USaveWidget* saveWidgetInstance;

	//sound
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* overlapSound;

	//Level Streaming
	UPROPERTY(EditAnywhere, Category = "LevelStreaming")
	FString lastTargetLevel;

	UFUNCTION()
	void OnMediaOpened(FString openedUrl);
	
};
