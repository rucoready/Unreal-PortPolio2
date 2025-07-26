// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "SoulActor.generated.h"

UCLASS()
class BLASTERDREAM_API ASoulActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoulActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* soulWidget;

	APlayerController* playerCon;
	FVector2D targetScreenPosition;
	FVector2D currentScreenPosition;
	float elasedTime;
	float movingDuration = 1.f;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* getSoulSound;
	


};
