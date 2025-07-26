// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimapActor.generated.h"

UCLASS()
class BLASTERDREAM_API AMinimapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMinimapActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class USpringArmComponent* minimapCameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class USceneCaptureComponent2D* minimapCapture;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class UPaperSpriteComponent* minimapSprite;
};
