// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordSlashActor.generated.h"

UCLASS()
class BLASTERDREAM_API ASwordSlashActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASwordSlashActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category="MySettings")
	class UNiagaraComponent* slashNA;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float slashSpeed = 2500.0f;

	FVector moveDirection;
	FVector targetLocation;
	FVector mouseWorldLocation;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* collisionBoxComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor>explosionActor;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	UParticleSystem* explosionParticle;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USoundBase* hitSound;


	UFUNCTION()
	void OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

};
