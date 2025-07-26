// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "moveMapActor.generated.h"

UCLASS()
class BLASTERDREAM_API AmoveMapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AmoveMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* otherActor)override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess="true"))
	FString TransfromLevelName;

	UPROPERTY()
	class UBoxComponent* TransformVolume;


};
