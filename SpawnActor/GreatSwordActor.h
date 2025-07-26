// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GreatSwordActor.generated.h"

UCLASS()
class BLASTERDREAM_API AGreatSwordActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGreatSwordActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class USceneComponent* sceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class UStaticMeshComponent* swordComp;

	void SwingSword();

	float currentRot;
	bool bIsRotatingNow;
	float totalRot;
	//rotation time!
	float rotationDuration;
	float elapsedTime;
	float slowExponent;


	

	
	

};
