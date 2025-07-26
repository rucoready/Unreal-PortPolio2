// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicSkeletonEnemy.h"
#include "SkeletonEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API USkeletonEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	UPROPERTY()
	ABasicSkeletonEnemy* skeletonEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	float groundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float direction;

	UFUNCTION()
	void AnimNotify_CollisionOn();

	UFUNCTION()
	void AnimNotify_CollisionOff();

	UFUNCTION()
	void AnimNotify_SpawnArrow();
	
	
};
