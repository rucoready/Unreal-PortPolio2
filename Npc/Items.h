// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items.generated.h"

UCLASS()
class BLASTERDREAM_API AItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItems();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* meshComp;


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FString ItemName;

	float RotSpeed = 20;
	void PickItems();
	void moveRotation(float time);


	bool impactPawn = false;

	UFUNCTION()
	void OnBeginOverlapDropActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float absorptionArea;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USoundBase* overlapSound;

};
