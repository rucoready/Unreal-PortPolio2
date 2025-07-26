// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropActor.generated.h"

UCLASS()
class BLASTERDREAM_API ADropActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float absorptionArea;

	UFUNCTION()
	void OnBeginOverlapDropActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Money")
	bool bMoney;

	class UStatGameInstance* myInstance;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* moneySound;
};
