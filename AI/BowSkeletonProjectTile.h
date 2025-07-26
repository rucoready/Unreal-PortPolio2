// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BowSkeletonProjectTile.generated.h"

UCLASS()
class BLASTERDREAM_API ABowSkeletonProjectTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABowSkeletonProjectTile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	UCapsuleComponent* collisionComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USkeletalMeshComponent* projectTileComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	UProjectileMovementComponent* projectileMovement;

	class ASwordCharacter* swordPlayer;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float projectTileDamage = 500.0f;
};
