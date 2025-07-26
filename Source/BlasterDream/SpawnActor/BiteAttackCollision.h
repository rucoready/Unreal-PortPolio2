// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BiteAttackCollision.generated.h"

UCLASS()
class BLASTERDREAM_API ABiteAttackCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	ABiteAttackCollision();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* collisionBox;

	class AHitTestCrow* crow;
	class ABasicSkeletonEnemy* skeleton;
	float calculateDamage;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USoundBase* biteSound;

	UFUNCTION()
	void OnBeginOverlapBoxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
